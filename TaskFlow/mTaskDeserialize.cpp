#include "mTaskDeserialize.h"
#include <QDebug>

MTaskDeserialize::MTaskDeserialize(const QString &program,QObject *parent) : QObject(parent){

    appendParams(0,QString(QByteArray::fromBase64(program.toUtf8()).data()));


    qDebug()<<"任务反序列化开始 ";
    for (int i = 0; i < steps.length(); ++i) {
        MFlowStepParams *params = steps[i];

        if(params->menuType==MCONSTANT_FLOW_MENU_TYPE_ExtractBtn){
             MFlowStepParamsExtract * pe= static_cast<MFlowStepParamsExtract *>(params);
             qDebug()<<QString("step = %1 ").arg(i)<<params->carryStepLocation<<params->name<<params->stepID<<params<<params->carryParent
                    <<"提取字段深度=="<<pe->carryDeep;

        }else {

            qDebug()<<QString("step = %1 ").arg(i)<<params->carryStepLocation<<params->name<<params->stepID<<params<<params->carryParent;

        }
    }
    qDebug()<<"任务反序列化结束";

}

MFlowStepParams::LocationType MTaskDeserialize::getStepLocationType(int start, int end, int index){
    if(index==end){
       return MFlowStepParams::LocationType::Last;
    }else if (index==start) {
       return MFlowStepParams::LocationType::Front;
    }else {
       return MFlowStepParams::LocationType::Middle;
    }
}
void MTaskDeserialize::appendParams(int deep,const QString &program, MFlowStepParams *parentParams){
    deep+=1;

    QStringList programList = program.split(SEPARATOR_SUB_AMONG);

    int start = 0;
    int end = programList.length()-1;

    for (int i = 0; i < programList.length(); ++i) {
        QString menuType = programList[i].mid(0,4);
        QString programBase64 = programList[i].mid(4);

         if(MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn==menuType){
             MFlowStepParamsOpenWeb *params = new MFlowStepParamsOpenWeb(QString(QByteArray::fromBase64(programBase64.toUtf8()).data()));
             params->carryParent = parentParams;
             params->carryStepLocation =getStepLocationType(start,end,i);
             steps.append(params);

         }else if(MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn==menuType){

             MFlowStepParamsClickEle *params = new MFlowStepParamsClickEle(QString(QByteArray::fromBase64(programBase64.toUtf8()).data()));
             params->carryParent = parentParams;
             params->carryStepLocation =getStepLocationType(start,end,i);
             steps.append(params);
         }else if(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn==menuType){
             MFlowStepParamsExtract *params = new MFlowStepParamsExtract(QString(QByteArray::fromBase64(programBase64.toUtf8()).data()));
             params->carryParent = parentParams;
             params->carryStepLocation =getStepLocationType(start,end,i);
             params->carryDeep = deep;
             steps.append(params);

         }else if(MCONSTANT_FLOW_MENU_TYPE_InputBtn==menuType){

             MFlowStepParamsInput *params = new MFlowStepParamsInput(QString(QByteArray::fromBase64(programBase64.toUtf8()).data()));
             params->carryParent = parentParams;
             params->carryStepLocation =getStepLocationType(start,end,i);
             steps.append(params);

         }else if(MCONSTANT_FLOW_MENU_TYPE_LoopBtn==menuType){
             appendLoopParams(deep,getStepLocationType(start,end,i),programBase64,parentParams);
         }else if(MCONSTANT_FLOW_MENU_TYPE_MouseBtn==menuType){
             MFlowStepParamsMouse *params = new MFlowStepParamsMouse(QString(QByteArray::fromBase64(programBase64.toUtf8()).data()));
             params->carryParent = parentParams;
             params->carryStepLocation =getStepLocationType(start,end,i);
             steps.append(params);

         }
    }
}
void MTaskDeserialize::appendLoopParams(int deep,MFlowStepParams::LocationType stepLocation,const QString &programBase64, MFlowStepParams *parentParams){
    // programsBase64  翻译后 loopBase64[step1Base64,step2Base64,,,]

    QString program = QString(QByteArray::fromBase64(programBase64.toUtf8()).data());
    int subStart = program.indexOf(SEPARATOR_SUB_LEFT);
    int subEnd = program.indexOf(SEPARATOR_SUB_RIGHT);
    QString loopProgramBase64 = program.mid(0,subStart);
    // 循环左侧
    MFlowStepParamsLoop *params = new MFlowStepParamsLoop(QString(QByteArray::fromBase64(loopProgramBase64.toUtf8()).data()));
    params->carryParent = parentParams;
    params->carryStepLocation =stepLocation;
    steps.append(params);

    //循环内部step
    QString subProgram = program.mid(subStart+1,subEnd-subStart-1);
    appendParams(deep,subProgram,params);//subProgram = step1Base64,step2Base64,step3Base64,,,

}
