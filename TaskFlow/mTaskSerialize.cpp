#include "mTaskSerialize.h"
#include "Utils/models.h"
#include "flowleftwidget/mFlowButtonWidget.h"
#include "flowleftwidget/mFlowIfWidget.h"
#include "flowleftwidget/mFlowLoopWidget.h"
#include "TaskFlow/pageCustomtaskFlowControl.h"
#include "mFcModels.h"
#include <QVBoxLayout>
#include <QDebug>
MTaskSerialize::MTaskSerialize(PageCustomTaskFlowControl *parent) : QObject(parent),m_fc(parent){

    QStringList programList;
    QVBoxLayout *boxLayout =m_fc->boxLayout;
    for (int i = 0; i < boxLayout->count(); ++i) {
        QWidget *widget = boxLayout->itemAt(i)->widget();
        QString  widgetName = widget->metaObject()->className();
        if ("MFlowLoopWidget"==widgetName) {
             MFlowLoopWidget *loopWidget= static_cast<MFlowLoopWidget *>(widget);
             programList << serializeLoopWidget(loopWidget);
        }else if ("MFlowIfWidget"==widgetName) {
            MFlowIfWidget *ifWidget= static_cast<MFlowIfWidget *>(widget);
            programList << serializeIfWidget(ifWidget);
        }else if ("MFlowButtonWidget"==widgetName) {
            MFlowButtonWidget *btnWidget= static_cast<MFlowButtonWidget *>(widget);
            programList<< serializeButtonWidget(btnWidget);
        }
    }
    qDebug()<<"任务序列化开始";
    m_fc->task->program = programList.join(",").toUtf8().toBase64();
    qDebug()<<"任务序列化结束";
}
QString MTaskSerialize::serializeButtonWidget(MFlowButtonWidget *btnWidget){
    QString program;

    if(MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn==btnWidget->data->control->menuType){
      MFlowStepParamsOpenWeb *params = static_cast<MFlowStepParamsOpenWeb*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn==btnWidget->data->control->menuType){
      MFlowStepParamsClickEle *params = static_cast<MFlowStepParamsClickEle*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn==btnWidget->data->control->menuType){
      MFlowStepParamsExtract *params = static_cast<MFlowStepParamsExtract*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_InputBtn==btnWidget->data->control->menuType){
      MFlowStepParamsInput *params = static_cast<MFlowStepParamsInput*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_CaptchaBtn==btnWidget->data->control->menuType){
      MFlowStepParamsCaptcha *params = static_cast<MFlowStepParamsCaptcha*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_SwitchBtn==btnWidget->data->control->menuType){
      MFlowStepParamsSwitch *params = static_cast<MFlowStepParamsSwitch*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_MouseBtn==btnWidget->data->control->menuType){
      MFlowStepParamsMouse *params = static_cast<MFlowStepParamsMouse*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_EndLoopBtn==btnWidget->data->control->menuType){
      MFlowStepParamsEndLoop *params = static_cast<MFlowStepParamsEndLoop*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }else if(MCONSTANT_FLOW_MENU_TYPE_EndBtn==btnWidget->data->control->menuType){
      MFlowStepParamsEnd *params = static_cast<MFlowStepParamsEnd*>(btnWidget->data->stepparams);
      program = params->getProgram();
    }

    return btnWidget->data->control->menuType+program;
}
QString MTaskSerialize::serializeLoopWidget(MFlowLoopWidget *loopWidget){
    QStringList programList;
    MFlowStepParamsLoop *params = static_cast<MFlowStepParamsLoop*>(loopWidget->data->stepparams);
    QVBoxLayout *boxLayout =loopWidget->boxLayout;

    for (int i = 0; i < boxLayout->count(); ++i) {
        QWidget *widget = boxLayout->itemAt(i)->widget();
        QString  widgetName = widget->metaObject()->className();

        if ("MFlowLoopWidget"==widgetName) {
            MFlowLoopWidget *loopWidget= static_cast<MFlowLoopWidget *>(widget);
            programList<< serializeLoopWidget(loopWidget);
        }else if ("MFlowIfWidget"==widgetName) {
            MFlowIfWidget *ifWidget= static_cast<MFlowIfWidget *>(widget);
            programList<< serializeIfWidget(ifWidget);
        }else if ("MFlowButtonWidget"==widgetName) {
            MFlowButtonWidget *btnWidget= static_cast<MFlowButtonWidget *>(widget);
            programList<< serializeButtonWidget(btnWidget);
        }
    }
    return params->menuType+QString("%1%2%3%4").arg(
                    params->getProgram(),
                    SEPARATOR_SUB_LEFT,
                    programList.join(SEPARATOR_SUB_AMONG),
                    SEPARATOR_SUB_RIGHT
                ).toUtf8().toBase64();

}
QString MTaskSerialize::serializeIfWidget(MFlowIfWidget *ifWidget){

    QStringList programList;
    MFlowStepParamsIf *params = static_cast<MFlowStepParamsIf*>(ifWidget->data->stepparams);

    QVBoxLayout *boxLayout =ifWidget->boxLayout;
    for (int i = 0; i < boxLayout->count(); ++i) {
        QWidget *widget = boxLayout->itemAt(i)->widget();
        QString  widgetName = widget->metaObject()->className();
        if ("MFlowLoopWidget"==widgetName) {
            MFlowLoopWidget *loopWidget= static_cast<MFlowLoopWidget *>(widget);
            programList<< serializeLoopWidget(loopWidget);
        }else if ("MFlowIfWidget"==widgetName) {
            MFlowIfWidget *ifWidget= static_cast<MFlowIfWidget *>(widget);
            programList<< serializeIfWidget(ifWidget);
        }else if ("MFlowButtonWidget"==widgetName) {
            MFlowButtonWidget *btnWidget= static_cast<MFlowButtonWidget *>(widget);
            programList<< serializeButtonWidget(btnWidget);
        }
    }
    return params->menuType+QString("%1%2%3%4").arg(
                    params->getProgram(),
                    SEPARATOR_SUB_LEFT,
                    programList.join(SEPARATOR_SUB_AMONG),
                    SEPARATOR_SUB_RIGHT
                ).toUtf8().toBase64();
}
