#include "RunPipline.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "TaskFlow/mFcModels.h"
#include "RunHelper.h"
#include <QHash>
#include <QDebug>
#include <QtCore5Compat>

static int64_t getCurTimestamp()// 获取毫秒级时间戳（13位）
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).
        count();

}
static int getCurTimestamp_second()// 获取秒级时间戳（10位）
{
    int64_t curTimestamp = getCurTimestamp();

    return int(curTimestamp/1000);
}

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
RunPipline::RunPipline(RunHelper *helper,QObject *parent) : QThread(parent),m_helper(helper){

    // step1 m_temps
    int index = 0;
    for (int i = 0; i < m_helper->extracts.length(); ++i) {
        MFlowStepParamsExtract * params = m_helper->extracts[i];
        if(i==0){
            QVector<MExtractSingleData*> groupData;
            m_temps.append(groupData);
        }else{
             if(m_helper->extracts[i-1]->carryDeep > params->carryDeep ){
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
RunPipline::~RunPipline(){
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
void RunPipline::sendStartCommand(){
    m_fpSet.clear();
    jobFinished = false;
    jobCommandState= true;
    m_num = 0;
    m_repeatNum = 0;
    m_startDate = QDateTime::currentDateTime();
//    m_startStamp = m_startDate.toTime_t();sunqt6
    m_startStamp = getCurTimestamp_second();

    start();
}

void RunPipline::sendStopCommand(){
    m_fpSet.clear();
    jobCommandState= false;
}

QString RunPipline::getfp(const QStringList& values){

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
void RunPipline::run(){

    Database::getInstance()->delTaskData(m_helper->task->code,true);

    if(!Database::getInstance()->createTable(m_helper->task->code,m_helper->fields)){
        emit this->notifyInfo(11,"创建数据表失败");
        jobFinished = true;
    }else{

        QString format = "yyyy.MM.dd hh:mm:ss";

        while (jobCommandState|| m_dataQueue.isEmpty()==false) {
            if(m_dataQueue.isEmpty()==false){

                MExtractSingleData singleData = m_dataQueue.takeFirst();
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
                    QString fingerprint = getfp(values);
                    bool fpContains = m_fpSet.contains(fingerprint);

                    if(fpContains){
                        m_repeatNum +=1;
                    }else{
                        m_fpSet.insert(fingerprint);
                    }
                    Database::getInstance()->async_addTaskData(
                        QString("insert into %1 (%2,isRepeat) values (%3,%4);").
                                    arg(m_helper->task->code).
                                    arg(names.join(",")).
                                    arg(valueFields.join(",")).
                                    arg(fpContains));

                    m_num +=1;
                    emit this->notifyPipline(m_num,names,values);
                    valueFields.clear();
                    MTaskData taskData(m_helper->task->code,
                                         m_num,m_repeatNum,
                                         MTaskData::FINISH,
                                         m_startDate.toString(format),
                                         QDateTime::currentDateTime().toString(format));

                    Database::getInstance()->async_updateTaskData(taskData);
                }
            }else{
                sleep(1);
            }

            // 更新计数
            int mem = Database::getInstance()->mem_ratio();
            if(mem > 92){
                emit this->notifyInfo(11,"内存使用率超过上限92%，采集被强制停止");
                jobFinished = true;
            }else{
//                int seconds = QDateTime::currentDateTime().toTime_t() - m_startStamp;sunqt6

                int seconds = getCurTimestamp_second() - m_startStamp;
                float speed = m_num * 60 / seconds;

                QString info = QString("已采集%1条（重复%2条） 已用时：%3秒 平均速度：%4条/分钟  内存（%5%）").
                                  arg(m_num).
                                  arg(m_repeatNum).
                                  arg(seconds).
                                  arg(speed).arg(mem);

                emit notifyInfo(1,info);

            }
        }

        jobFinished = true;
    }

}
void RunPipline::onPushData(QString stepID,QString res){
//    qDebug()<<"onPushData "<<stepID<<res;
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
                qDebug()<<"onPushData Error："<<res<<"attres"<<attrs;
            }

        }
        m_dataQueue.enqueue(singleData);
    }
    stepID.clear();
    res.clear();
}
