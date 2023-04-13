#include "RunThreadPipline.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "TaskFlow/mFcModels.h"
#include "RunDialog.h"
#include "RunHelper.h"
#include <QHash>
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
#include <QtCore5Compat>
#endif
#include <QsLog.h>



MExtractSingleItem::MExtractSingleItem(){

}
MExtractSingleItem::MExtractSingleItem(const QString &name,const QString &value){
    this->name = name;
    this->value = value;
}
MExtractSingleData::MExtractSingleData(const QString &stepID){
    this->stepID = stepID;
}

MExtractParamsGroupLocation::MExtractParamsGroupLocation(){}
MExtractParamsGroupLocation::MExtractParamsGroupLocation(int group,int index){
    this->group = group;
    this->index = index;
}
RunThreadPipline::RunThreadPipline(QObject *parent) : QThread(parent){
    mRunHelper = ((RunDialog *)parent)->getHelper();

    // step1 m_temps
    int index = 0;
    for (int i = 0; i < mRunHelper->extracts.length(); ++i) {
        MFlowStepParamsExtract * params = mRunHelper->extracts[i];
        if(i==0){
            QVector<MExtractSingleData*> groupData;
            m_temps.append(groupData);
        }else{
             if(mRunHelper->extracts[i-1]->carryDeep > params->carryDeep ){
                // 前一步deep大于下一步deep，则初始化新分组
                 QVector<MExtractSingleData*> groupData;
                 m_temps.append(groupData);
                 index +=1;
             }
        }
        m_temps[index].append(new MExtractSingleData(params->stepID));
    }
    // step2 生成 m_locations
    for (int i = 0; i < m_temps.count(); ++i) {
        for (int j = 0; j < m_temps[i].count(); ++j) {
            m_locations.insert(m_temps[i][j]->stepID,new MExtractParamsGroupLocation(i,j));
        }
    }

}
RunThreadPipline::~RunThreadPipline(){
    // 删除临时变量的指针
    for (int i = 0; i < m_temps.count(); ++i) {
        for (int j = 0; j < m_temps[i].count(); ++j) {
            delete m_temps[i][j];
        }
    }

    QHash<QString,MExtractParamsGroupLocation*>::const_iterator it;
    for (it=m_locations.constBegin();it!=m_locations.constEnd();++it) {
        delete it.value();
    }

    terminate();
}
void RunThreadPipline::sendStartCommand(int jobPeriod){
    mDataFingerSet.clear();
    jobFinished = false;
    jobCommandState= true;
    mJobPeriod = jobPeriod;
    mJobCount = 0;
    mJobRepeatCount = 0;
    mJobStartDate = QDateTime::currentDateTime();
//    mJobStartStamp = mJobStartDate.toTime_t();sunqt6
    mJobStartStamp = mJobStartDate.toSecsSinceEpoch();

    if(1==mJobPeriod){
        mTotalJobCount = 0;
        mTotalJobRepeatCount = 0;
    }
    start();//开启线程函数 run();


}

void RunThreadPipline::sendStopCommand(){
    mDataFingerSet.clear();
    jobCommandState= false;
}

void RunThreadPipline::run(){
    QLOG_INFO()<<"RunThreadPipline::run()";

    if(1==mJobPeriod){
        //删除之前的任务数据表
        Database::getInstance()->delTaskData(mRunHelper->getTask()->code);
        //创建新的任务数据表
        QString msg;
        if(!Database::getInstance()->createTaskData(mRunHelper->getTask()->code,msg,mRunHelper->fields)){
            emit this->notifyAlert(11,msg);
            jobFinished = true;
            return;
        }

    }

    QString timeFmt = "yyyy.MM.dd hh:mm:ss";
    while (jobCommandState|| mDataQueue.isEmpty()==false) {
        if(mDataQueue.isEmpty()==false){

            MExtractSingleData singleData = mDataQueue.takeFirst();
            MExtractParamsGroupLocation *loc = m_locations[singleData.stepID];
            MExtractSingleData *tempData = m_temps[loc->group][loc->index];
            tempData->items.clear();
            for (int i = 0; i < singleData.items.length(); ++i) {
                tempData->items.push_back(singleData.items[i]);
            }

            if(loc->index==m_temps[loc->group].count()-1){

                QStringList names;
                QStringList values;
                QStringList valueFields;
                for(int f = 0; f < m_temps[loc->group].count();++f){
                    for (int g = 0; g < m_temps[loc->group][f]->items.count(); ++g) {
                        MExtractSingleItem item = m_temps[loc->group][f]->items[g];
                        names.append(item.name);
                        values.append(item.value);
                        valueFields.append("'"+item.value+"'");
                    }
                }
                QString fingerprint = calcuDataFinger(values);
                bool isRepeat = mDataFingerSet.contains(fingerprint);

                if(isRepeat){
                    ++mJobRepeatCount;
                    ++mTotalJobRepeatCount;
                }else{
                    mDataFingerSet.insert(fingerprint);
                }

                Database::getInstance()->asyncAddTaskData(
                    QString("insert into %1 (%2,period,isRepeat,addTime) values (%3,%4,%5,'%6');").
                                arg(mRunHelper->getTask()->code).
                                arg(names.join(",")).
                                arg(valueFields.join(",")).
                                arg(mJobPeriod).
                                arg(isRepeat).
                                arg(QDateTime::currentDateTime().toString(timeFmt)));

                ++mJobCount;
                ++mTotalJobCount;

                emit this->notifyUpdateData(mJobCount,names,values);
                valueFields.clear();
                MTaskData taskData(mRunHelper->getTask()->code,
                                     mTotalJobCount,
                                     mTotalJobRepeatCount,
                                     MTaskData::FINISH,
                                     mJobStartDate.toString(timeFmt),
                                     QDateTime::currentDateTime().toString(timeFmt));

                Database::getInstance()->asyncUpdateTaskData(taskData);
            }

            // 更新计数
            int mem = Database::getInstance()->mem_ratio();
            if(mem > 95){
                emit this->notifyAlert(11,"内存使用率超过上限95%，采集被强制停止");
                jobFinished = true;
            }else{
    //          int seconds = QDateTime::currentDateTime().toTime_t() - mJobStartStamp;sunqt6
                int seconds = QDateTime::currentDateTime().toSecsSinceEpoch() - mJobStartStamp;
                float speed = mJobCount * 60 / seconds;

                QString info = QString("所有周期采集条数:%1，当前周期:%2，采集%3条（重复%4条），已用时:%5秒，平均速度:%6条/分钟  内存（%7%）").
                        arg(mTotalJobCount).
                        arg(mJobPeriod).
                        arg(mJobCount).
                        arg(mJobRepeatCount).
                        arg(seconds).
                        arg(speed).arg(mem);

                emit notifyAlert(1,info);

            }

        }else{
            sleep(1);
        }
    }
    jobFinished = true;
    emit notifyJobFinished();

}
QString RunThreadPipline::calcuDataFinger(const QStringList& values){

    QString fingerprint;
    for (int i = 0; i < values.length(); ++i) {
        QString value = values[i];
        int l = value.length();
        fingerprint+=QString("%1%2%3").
                arg(value.mid(0,1)).
                arg(value.mid(l-1,1)).
                arg(l);
    }
    fingerprint = fingerprint.toUtf8().toBase64();
    return fingerprint;
}
void RunThreadPipline::onPushData(QString stepID,QString res){
//    QLOG_INFO()<<"onPushData "<<stepID<<res;
    if(""!=res){
        MExtractSingleData singleData(stepID);
        QStringList fields = res.split(SEPARATOR_WEBDATA_ARRAY);
        for (int i = 0; i < fields.length(); ++i) {
            QStringList attrs = fields[i].split(SEPARATOR_WEBDATA_ATTR);
            if(attrs.length()==2){
                QString name = attrs[0];
                QString value = attrs[1];
        //        QString value = QString(QByteArray::fromBase64(fieldItem[1].toUtf8().data()));

                MExtractSingleItem item(name,value);
                singleData.items.append(item);
            }else {
                QLOG_INFO()<<"onPushData Error："<<res<<"attres"<<attrs;
            }

        }
        mDataQueue.enqueue(singleData);
    }
    stepID.clear();
    res.clear();
}
