#include "mFcHelper.h"
#include "Utils/models.h"
#include "mFcModels.h"
#include "mTaskSerialize.h"
#include "mTaskDeserialize.h"
#include "TaskFlow/pageCustomtaskFlowControl.h"
#include "flowleftwidget/mFlowButtonWidget.h"
#include "flowleftwidget/mFlowIfWidget.h"
#include "flowleftwidget/mFlowLoopWidget.h"
#include "flowleftwidget/mDownArrow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
MFcHelper::MFcHelper(PageCustomTaskFlowControl *parent) : QObject(parent) {
    m_fc = parent;
}
MFcHelper::~MFcHelper(){
    qDebug()<<"MFcHelper::~MFcHelper()";
}
void MFcHelper::serialize(){
    MTaskSerialize taskser(m_fc);
}
void MFcHelper::deserialize(){

    MTaskDeserialize taskdeser(m_fc->task->program,this);

    for (int i = 0; i < taskdeser.steps.length(); ++i) {
        MFlowStepParams *params = taskdeser.steps[i];
        bool bgnormal = true;
        if(0==i){
            bgnormal = false;
        }
        if(MCONSTANT_FLOW_MENU_TYPE_LoopBtn==params->menuType){
            addLoopWidget(params,bgnormal);
        }else if(MCONSTANT_FLOW_MENU_TYPE_JudgeBtn==params->menuType){
            addIfWidget(params,bgnormal);
        }else{
            addButtonWidget(params,bgnormal);
        }
    }

    updateFixedSize();

    m_currentData->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;

    emit m_fc->notificationToFlow(m_currentData);
//    emit m_fc->notificationToSmartAlertWidget(this->m_currentData);


}

MFcHelper::Location MFcHelper::getLocation(MFlowStepParams *params){
    Location location;

    QWidget *widget = m_fc;
    QVBoxLayout *layout = m_fc->boxLayout;
    int target =layout->count() - 2; // 默认位置在主窗口的顶部

    if(params->carryParent!=nullptr){

        for (int i = this->allstepwidgets.count()-1; i >=0 ; --i) {

             QWidget *stepwidget = this->allstepwidgets.at(i);
             QString  stepwidgetname = stepwidget->metaObject()->className();

             if ("MFlowLoopWidget"==stepwidgetname) {
                 MFlowLoopWidget *step = qobject_cast<MFlowLoopWidget *>(stepwidget);
                 if(step->data->stepparams==params->carryParent){

                     widget = step;
                     layout = step->boxLayout;
                     target = step->boxLayout->count() - 1;

                     break;
                 }
             }else if ("MFlowIfWidget"==stepwidgetname) {
                 MFlowIfWidget *step = qobject_cast<MFlowIfWidget *>(stepwidget);
                 if(step->data->stepparams==params->carryParent){

                     widget = step;
                     layout = step->boxLayout;
                     target = step->boxLayout->count() - 1;
                     break;
                 }
             }


        }

    }

    location.widget = widget;
    location.layout = layout;
    location.target = target;

    return location;
}

void MFcHelper::addButtonWidget(MFlowStepParams *params,bool bgnormal){

    Location location = getLocation(params);
    MFlowButtonWidget *stepwidget = new MFlowButtonWidget(m_fc,
                                                      m_fc->controls.value(params->menuType),
                                                      new MDownArrow(m_fc),params,bgnormal);
    stepwidget->currentParentWidget = location.widget;
    connect(stepwidget,&MFlowButtonWidget::notificationToParentWidget,m_fc,&PageCustomTaskFlowControl::onNotificationFromChildWidget);
    this->arrows.append(stepwidget->arrow);
    this->allstepwidgets.append(stepwidget);
    stepwidget->title->setText(params->name);

    location.layout->insertWidget(location.target,stepwidget,0,Qt::AlignCenter);
    location.layout->insertWidget(location.target,stepwidget->arrow,0,Qt::AlignCenter);

    if(!bgnormal){
        m_currentData = stepwidget->data;
    }
}
void MFcHelper::addIfWidget(MFlowStepParams *params,bool bgnormal){
    Location location = getLocation(params);

    MFlowIfWidget *stepwidget = new MFlowIfWidget(m_fc, m_fc->controls.value(params->menuType),
                                                  new MDownArrow(m_fc),new MDownArrow(m_fc),
                                                  params,bgnormal);

    this->arrows.append(stepwidget->arrow);
    this->arrows.append(stepwidget->innerDefaultArrow);
    this->allstepwidgets.append(stepwidget);
    stepwidget->title->setText(params->name);

    connect(stepwidget,&MFlowIfWidget::notificationToParentWidget,m_fc,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

    location.layout->insertWidget(location.target,stepwidget,0,Qt::AlignCenter);
    location.layout->insertWidget(location.target,stepwidget->arrow,0,Qt::AlignCenter);
    if(!bgnormal){
        m_currentData = stepwidget->data;
    }
}
void MFcHelper::addLoopWidget(MFlowStepParams *params,bool bgnormal){

    Location location = getLocation(params);

    MFlowLoopWidget *stepwidget = new MFlowLoopWidget(m_fc, m_fc->controls.value(params->menuType),
                                                  new MDownArrow(m_fc),new MDownArrow(m_fc),
                                                  params,bgnormal);

    this->arrows.append(stepwidget->arrow);
    this->arrows.append(stepwidget->innerDefaultArrow);
    this->allstepwidgets.append(stepwidget);
    stepwidget->title->setText(params->name);

    connect(stepwidget,&MFlowLoopWidget::notificationToParentWidget,m_fc,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

    location.layout->insertWidget(location.target,stepwidget,0,Qt::AlignCenter);
    location.layout->insertWidget(location.target,stepwidget->arrow,0,Qt::AlignCenter);
    if(!bgnormal){
        m_currentData = stepwidget->data;
    }
}


void MFcHelper::removeStep(QWidget *stepwidget){

    //获取被删除的widget及其内部所有的widget，以及箭头，并清理出去
     QTimer::singleShot(10,this,[this,stepwidget](){
         m_removeWidgets.clear();
         updateDelWidgets(stepwidget);

         for (int i = 0; i < m_removeWidgets.count(); ++i) {
             //判断被删除的widget是否存在与 m_currentPressedData一致，如果存在，则需要置空
             QWidget *delWidget = m_removeWidgets.at(i);
             if(delWidget==m_currentData->widget){
                 m_currentData->widgetIsDelete = true;
             }
             delWidget->deleteLater();
         }
     });

    updateFixedSize();

}
void MFcHelper::updateFixedSize(){

    QTimer::singleShot(50,this,[this](){
        m_tempLoopTypeWidgetMaxDeep = 0;
        m_tempLoopTypeWidgets.clear();//清空所有层级中的窗体

        getLoopTypeWidgets(0,this->m_fc);

        for (int i=m_tempLoopTypeWidgetMaxDeep;i>=1;--i) {
            QList<QWidget *> values= m_tempLoopTypeWidgets.values(i);
            for (int j=0;j<values.count();++j) {
                QWidget *widget = values[j];
                QString widgetName = widget->metaObject()->className();

                if ("MFlowLoopWidget"==widgetName) {
                    MFlowLoopWidget *loopWidget= qobject_cast<MFlowLoopWidget *>(widget);
                    loopWidget->updateFixedSize();
                }else if ("MFlowIfWidget"==widgetName) {
                    MFlowIfWidget * ifWidget = qobject_cast<MFlowIfWidget *>(widget);
                    ifWidget->updateFixedSize();
                }
            }
        }

    });

}
void MFcHelper::updateDelWidgets(QWidget *widget){

    QBoxLayout *dataLayout = nullptr;

    QString widgetName = widget->metaObject()->className();

    // step 自身widget涉及节点的新增和移除
    if ("MFlowLoopWidget"==widgetName) {
        MFlowLoopWidget *loopWidget = static_cast<MFlowLoopWidget *>(widget);

        m_removeWidgets.append(loopWidget);
        m_removeWidgets.append(loopWidget->arrow);
        m_removeWidgets.append(loopWidget->innerDefaultArrow);

        this->arrows.removeOne(loopWidget->arrow);
        this->arrows.removeOne(loopWidget->innerDefaultArrow);
        this->allstepwidgets.removeOne(loopWidget);

        dataLayout = loopWidget->boxLayout;
    }else if ("MFlowIfWidget"==widgetName) {
        MFlowIfWidget *ifWidget = static_cast<MFlowIfWidget *>(widget);
        m_removeWidgets.append(ifWidget);
        m_removeWidgets.append(ifWidget->arrow);
        m_removeWidgets.append(ifWidget->innerDefaultArrow);

        this->arrows.removeOne(ifWidget->arrow);
        this->arrows.removeOne(ifWidget->innerDefaultArrow);
        this->allstepwidgets.removeOne(ifWidget);
        dataLayout = ifWidget->boxLayout;
    }else if ("MFlowButtonWidget"==widgetName) {
        MFlowButtonWidget *btnWidget = static_cast<MFlowButtonWidget *>(widget);
        m_removeWidgets.append(btnWidget);
        m_removeWidgets.append(btnWidget->arrow);
        this->arrows.removeOne(btnWidget->arrow);
        this->allstepwidgets.removeOne(btnWidget);
    }

    if(dataLayout!=nullptr){

        for (int i=0;i<dataLayout->count();++i) {

            QWidget *widget2 = dataLayout->itemAt(i)->widget();
            QString widget2Name=widget2->metaObject()->className();

            if ("MFlowLoopWidget"==widget2Name) {
                MFlowLoopWidget *widget2C = static_cast<MFlowLoopWidget *>(widget2);
                dataLayout->removeWidget(widget2C);

                m_removeWidgets.append(widget2C);
                m_removeWidgets.append(widget2C->arrow);
                m_removeWidgets.append(widget2C->innerDefaultArrow);
                this->arrows.removeOne(widget2C->arrow);
                this->arrows.removeOne(widget2C->innerDefaultArrow);
                this->allstepwidgets.removeOne(widget2C);
                updateDelWidgets(widget2C);
            }else if ("MFlowIfWidget"==widget2Name) {
                MFlowIfWidget *widget2C = static_cast<MFlowIfWidget *>(widget2);
                dataLayout->removeWidget(widget2C);

                m_removeWidgets.append(widget2C);
                m_removeWidgets.append(widget2C->arrow);
                m_removeWidgets.append(widget2C->innerDefaultArrow);
                this->arrows.removeOne(widget2C->arrow);
                this->arrows.removeOne(widget2C->innerDefaultArrow);
                this->allstepwidgets.removeOne(widget2C);
                updateDelWidgets(widget2C);
            }else if ("MFlowButtonWidget"==widget2Name) {
                MFlowButtonWidget *widget2C = static_cast<MFlowButtonWidget *>(widget2);
                dataLayout->removeWidget(widget2C);


                m_removeWidgets.append(widget2C);
                m_removeWidgets.append(widget2C->arrow);
                this->arrows.removeOne(widget2C->arrow);
                this->allstepwidgets.removeOne(widget2C);
            }
        }

    }


}
void MFcHelper::setarrowsPress(){
    for (int i=0;i<arrows.count();++i) {
         arrows.at(i)->setIconPress();
    }

}
void MFcHelper::setarrowsNormal(){
    for (int i=0;i<arrows.count();++i) {
         arrows.at(i)->setIconNormal();
    }

}
void MFcHelper::updateBackground(){
  for (int i = 0; i < allstepwidgets.count(); ++i) {
        QWidget *widget = allstepwidgets.at(i);
        if(m_currentData!=nullptr && m_currentData->widget==widget){
            widget->setStyleSheet(m_stylesheet_QWidget_flowleftwidget_blue_press);
        }else{
            widget->setStyleSheet(m_stylesheet_QWidget_flowleftwidget_blue_normal);

        }
    }
}
void MFcHelper::getLoopTypeWidgets(int deep, QWidget *widget){

    deep+=1;

    if(m_tempLoopTypeWidgetMaxDeep < deep){
       m_tempLoopTypeWidgetMaxDeep = deep;
    }
    QString widgetName = widget->metaObject()->className();
    QBoxLayout *dataLayout = nullptr;

    if ("MFlowLoopWidget"==widgetName) {
        MFlowLoopWidget *current = static_cast<MFlowLoopWidget *>(widget);
        dataLayout = current->boxLayout;
    }else if ("MFlowIfWidget"==widgetName) {
        MFlowIfWidget *current = static_cast<MFlowIfWidget *>(widget);
        dataLayout = current->boxLayout;

    }else if ("PageCustomTaskFlowControl"==widgetName) {
         PageCustomTaskFlowControl *current = static_cast<PageCustomTaskFlowControl *>(widget);
         dataLayout = current->boxLayout;
    }
    if(dataLayout!=nullptr){
        for (int i=0;i<dataLayout->count();++i) {

            QWidget *nextWidget = dataLayout->itemAt(i)->widget();
            // 下一个widget只有符合循环类型的控件才可以继续迭代
            if("MFlowLoopWidget"==QString(nextWidget->metaObject()->className())){
                m_tempLoopTypeWidgets.insert(deep,nextWidget);
                getLoopTypeWidgets(deep,nextWidget);
            }else if("MFlowIfWidget"==QString(nextWidget->metaObject()->className())){
                m_tempLoopTypeWidgets.insert(deep,nextWidget);
                getLoopTypeWidgets(deep,nextWidget);
            }

        }

    }
}

void MFcHelper::setCurrentStepData(MFlowStepData *data){
     // 在设置当前stepData时，如果是由侧边栏直接拖拽过来的data，则忽略
     // 因为侧边栏拖拽过来的data在dropEvent时，已经提前设置过。

   if ("MFlowMenuToolButton"!=QString(data->widget->metaObject()->className())) {
     this->m_currentData = data;
   }


}


MFlowStepData * MFcHelper::getCurrentStepData(){
    return this->m_currentData;
}
