#include "pageCustomtaskFlow.h"
#include "pageCustomtaskFlowControl.h"
#include "flowleftwidget/mFlowMenuToolbutton.h"
#include "flowright/mFlowRightOpenwebWidget.h"
#include "flowright/mFlowRightClickeleWidget.h"
#include "flowright/mFlowRightExtractWidget.h"
#include "flowright/mFlowRightInputWidget.h"
#include "flowright/mFlowRightLoopWidget.h"
#include "flowright/mFlowRightSwitchWidget.h"
#include "flowright/mFlowRightIfWidget.h"
#include "flowright/mFlowRightMouseWidget.h"
#include "flowright/mFlowRightTipWidget.h"
#include "Utils/ComSplitHWidget.h"
#include "mFcModels.h"
#include "style.h"
#include <QScrollArea>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>


PageCustomTaskFlow::PageCustomTaskFlow(MTask *task,QWidget *parent) : QWidget(parent),m_task(task){

    QHBoxLayout *boxHLayout= new QHBoxLayout(this);
    boxHLayout->setContentsMargins(0,0,0,0);
    boxHLayout->setSpacing(0);
    initLeftUi();
    initMenuUi();
    initRightUi();

    boxHLayout->addWidget(menuWidget);
    QWidget *line = new QWidget(this);
    line->setStyleSheet(".QWidget{background-color:rgb(233,233,233);}");
    line->setFixedWidth(1);
    boxHLayout->addWidget(line);

    boxHLayout->addWidget(leftWidget);
//    boxHLayout->addSpacing(6);
    // 水平拖拽
    ComSplitHWidget *splitHWidget = new ComSplitHWidget(this);
    splitHWidget->setFixedWidth(4);
    boxHLayout->addWidget(splitHWidget);

    connect(splitHWidget,&ComSplitHWidget::moveDistance,this,[this](int distance){
        int leftW = leftWidget->width()+distance;
        if(leftW>60){
            leftWidget->setFixedWidth(leftW);
        }
        m_leftratio = float(leftWidget->width()) / float(this->width());
    });

    boxHLayout->addWidget(rightWidget);

}
PageCustomTaskFlow::~PageCustomTaskFlow(){

}
void PageCustomTaskFlow::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    leftWidget->setFixedWidth(this->width() * m_leftratio);
}
void PageCustomTaskFlow::initLeftUi(){
    leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(0);

    QScrollArea *leftScroll = new QScrollArea(leftWidget);
    leftScroll->setStyleSheet(m_stylesheet_QScrollArea);

    QScrollBar *bar = new QScrollBar(leftScroll);
    leftScroll->setVerticalScrollBar(bar);
    fc = new PageCustomTaskFlowControl(m_task,this);
    connect(fc,&PageCustomTaskFlowControl::notificationToFlow,this,&PageCustomTaskFlow::onNotificationFromFlowControl);
    leftScroll->setWidget(fc);
    leftScroll->setWidgetResizable(true);
    leftLayout->addWidget(leftScroll);
}
void PageCustomTaskFlow::initMenuUi(){
    menuWidget = new QWidget(this);
    menuWidget->setAttribute(Qt::WA_StyledBackground,true);
    menuWidget->setStyleSheet("QWidget{background-color:rgb(242,243,245)}");
    menuWidget->setFixedWidth(38);
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setContentsMargins(0,0,0,0);
    menuLayout->setSpacing(0);

    QWidget *contraolsWidget = new QWidget(menuWidget);
    QVBoxLayout *controlsLayout = new QVBoxLayout(contraolsWidget);
    controlsLayout->setContentsMargins(0,0,0,0);
    controlsLayout->setSpacing(0);
    contraolsWidget->setFixedHeight(180); // 30 * 6 一个菜单项的高度30
    QStringList controls = QStringList()<<MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn<<MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn
          <<MCONSTANT_FLOW_MENU_TYPE_ExtractBtn<<MCONSTANT_FLOW_MENU_TYPE_InputBtn
          <<MCONSTANT_FLOW_MENU_TYPE_LoopBtn<<MCONSTANT_FLOW_MENU_TYPE_MouseBtn;
    for (int i = 0; i < controls.count(); ++i) {

        MFlowMenuToolButton *contr = new MFlowMenuToolButton(fc,fc->controls.value(controls[i]));
        connect(contr,&MFlowMenuToolButton::notificationToParentWidget,fc,&PageCustomTaskFlowControl::onNotificationFromChildWidget);
        controlsLayout->addWidget(contr,0,Qt::AlignCenter);
    }


    menuLayout->addWidget(contraolsWidget);
    menuLayout->addStretch(10);

}
void PageCustomTaskFlow::initRightUi(){
    // 右侧区域初始化
    rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSpacing(0);

    // rightScroll
    QScrollArea *rightScroll = new QScrollArea(rightWidget);
    rightScroll->setStyleSheet(m_stylesheet_QScrollArea);
    QScrollBar *bar = new QScrollBar(rightScroll);
    rightScroll->setVerticalScrollBar(bar);
    QWidget *rightScrollWidget = new QWidget(this);
    rightScrollWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);}");
    rightScrollWidgetLayout = new QVBoxLayout(rightScrollWidget);
    rightScrollWidgetLayout->setContentsMargins(0,0,0,0);
    rightScrollWidgetLayout->setSpacing(0);
    rightScroll->setWidget(rightScrollWidget);
    rightScroll->setWidgetResizable(true);
    rightLayout->addWidget(rightScroll);
}



void PageCustomTaskFlow::onNotificationFromFlowControl(MFlowStepData* data){


    if(MCONNSTANT_NOTIFICATION_TYPE_OTHER_END_SHOWCONTENT==data->notificationType ||
       MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT==data->notificationType){
        //        qDebug()<<"PageCustomTaskFlow::onNotificationFromFlowControl";

            for (int i=0;i<rightScrollWidgetLayout->count();++i) {
                delete rightScrollWidgetLayout->itemAt(i)->widget();
            }
    //        // 断开信号的总结：https://www.jianshu.com/p/986851c507da

            if(MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn==data->control->menuType){
                MFlowRightOpenWebWidget *r = new MFlowRightOpenWebWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn==data->control->menuType){

                MFlowRightClickEleWidget *r= new MFlowRightClickEleWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn==data->control->menuType){

                MFlowRightExtractWidget *r =new MFlowRightExtractWidget(this,data,
                            fc->task);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_InputBtn==data->control->menuType){

                MFlowRightInputWidget *r =new MFlowRightInputWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_JudgeBtn==data->control->menuType){

                MFlowRightIfWidget *r =new MFlowRightIfWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_LoopBtn==data->control->menuType){

                MFlowRightLoopWidget *r =new MFlowRightLoopWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_MouseBtn==data->control->menuType){

                MFlowRightMouseWidget *r =new MFlowRightMouseWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_SwitchBtn==data->control->menuType){

                MFlowRightSwitchWidget *r =new MFlowRightSwitchWidget(this,data);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);
            }else if(MCONSTANT_FLOW_MENU_TYPE_EndLoopBtn==data->control->menuType){
                MFlowRightTipWidget *r = new MFlowRightTipWidget("结束循环","无自定义参数",this);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);

            }else if(MCONSTANT_FLOW_MENU_TYPE_EndBtn==data->control->menuType){
                MFlowRightTipWidget *r = new MFlowRightTipWidget("结束","无自定义参数",this);
                r->setAttribute(Qt::WA_DeleteOnClose);
                rightScrollWidgetLayout->addWidget(r);

            }
    }

}


