#include "pageCustomtaskFlowControl.h"
#include "flowleftwidget/mFlowButtonWidget.h"
#include "flowleftwidget/mFlowLoopWidget.h"
#include "flowleftwidget/mFlowIfWidget.h"
#include "flowleftwidget/mFlowMenuToolbutton.h"
#include "flowleftwidget/mDownArrow.h"
#include "mFcHelper.h"
#include "mFcModels.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>


PageCustomTaskFlowControl::PageCustomTaskFlowControl(MTask *task,QWidget *parent) : QWidget(parent),
    m_fcHelper(new MFcHelper(this)){
    this->task = task;

    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".PageCustomTaskFlowControl {background-color:rgb(255,255,255);}");
    setAcceptDrops(true);

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,5,0,200);
    boxLayout->setSpacing(1);

    // 初始化控件
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(Database::getInstance()->read_file(":/res/images/control/controls.json").toUtf8(), &json_error));
    if(json_error.error == QJsonParseError::NoError){
        QJsonObject rootObj = jsonDoc.object();
        QStringList keys = rootObj.keys();
        for(int i = 0; i < keys.size(); i++){
             QString menuType = keys.at(i);
             QJsonObject obj = rootObj.value(menuType).toObject();

             controls.insert(menuType,new MControl(menuType,obj.value("tip").toString(),obj.value("icon").toString(),obj.value("flowWidgetName").toString()));
        }
    }

    //添加开始控件
    QPushButton *start = new QPushButton(this);
    start->setIcon(QIcon(":/res/images/icon/flow/start.png"));
    start->setStyleSheet("QPushButton{border:0px;}");
    start->setIconSize(QSize(30,30));
    boxLayout->addWidget(start,0,Qt::AlignCenter);
   // 添加默认箭头
    MDownArrow *arrow = new MDownArrow(this);
    boxLayout->addWidget(arrow,0,Qt::AlignCenter);
    m_fcHelper->arrows.append(arrow);
   //添加结束控件
    QPushButton *end = new QPushButton(this);
    end->setIcon(QIcon(":/res/images/icon/flow/pause.png"));
    end->setStyleSheet("QPushButton{border:0px;}");
    end->setIconSize(QSize(22,22));

    boxLayout->addWidget(end,0,Qt::AlignCenter);
}
PageCustomTaskFlowControl::~PageCustomTaskFlowControl(){
    QHash<QString,MControl*>::const_iterator it;
    for (it=controls.constBegin();it!=controls.constEnd();++it) {
        delete it.value();
    }

}
void PageCustomTaskFlowControl::serialize(){
    m_fcHelper->serialize();
}
void PageCustomTaskFlowControl::deserialize(){
    m_fcHelper->deserialize();
}
void PageCustomTaskFlowControl::setFlowAddressList(const QStringList &addressList, bool isFirst){
    // 流程第一步
    if(isFirst){
        bool isLoop = false;

        if(addressList.length()>1){
            isLoop = true;
            NextStepLocation nslocation = getNextStepLocation();

            MFlowLoopWidget *loopWidget = new MFlowLoopWidget(this,this->controls[MCONSTANT_FLOW_MENU_TYPE_LoopBtn],
                                                          new MDownArrow(this),
                                                          new MDownArrow(this),nullptr,true);
            m_fcHelper->arrows.append(loopWidget->arrow);
            m_fcHelper->arrows.append(loopWidget->innerDefaultArrow);
            m_fcHelper->allstepwidgets.append(loopWidget);

            connect(loopWidget,&MFlowLoopWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

            nslocation.layout->insertWidget(nslocation.target,loopWidget,0,Qt::AlignCenter);
            nslocation.layout->insertWidget(nslocation.target,loopWidget->arrow,0,Qt::AlignCenter);

            MFlowStepParamsLoop *params = static_cast<MFlowStepParamsLoop*>(loopWidget->data->stepparams);

            params->loopTypeName = MCONSTANT_RADIO_SITEADDRESS;
            params->loopTypeValue = addressList.join(",");

            params->loopTextList = addressList;

            m_fcHelper->setCurrentStepData(loopWidget->data);

        }

        // 首次添加该步骤，才会新建openweb窗体
        QTimer::singleShot(50,this,[=](){
            this->addFlowOpenWebStep(addressList[0],isLoop);
        });
    }else {
        qDebug()<<"addFlowAddressListStep isFirst == false 暂无处理";
    }

}
void PageCustomTaskFlowControl::addFlowOpenWebStep(const QString &url,bool isLoop){
    // 添加打开网页步骤窗体
    NextStepLocation nslocation = getNextStepLocation();

    MFlowButtonWidget *openWebWidget = new MFlowButtonWidget(this,
                                                      this->controls.value(MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn),
                                                      new MDownArrow(this));
    openWebWidget->currentParentWidget = nslocation.widget;
    connect(openWebWidget,&MFlowButtonWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);
    m_fcHelper->arrows.append(openWebWidget->arrow);
    m_fcHelper->allstepwidgets.append(openWebWidget);

    MFlowStepParamsOpenWeb *params = static_cast<MFlowStepParamsOpenWeb*>(openWebWidget->data->stepparams);
    params->stepURL = url;
    params->url = url;
    params->isLoop = isLoop;

    nslocation.layout->insertWidget(nslocation.target,openWebWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,openWebWidget->arrow,0,Qt::AlignCenter);


    openWebWidget->data->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;

    m_fcHelper->setCurrentStepData(openWebWidget->data);

    emit this->notificationToFlow(m_fcHelper->getCurrentStepData());

}
void PageCustomTaskFlowControl::addFlowLoopAndExtractStep(MParamsField *field,
                                                          const QString &loopTypeName,
                                                          const QString &loopTypeValue,
                                                          const QStringList &textList){
    // 新增（循环+提取数据）步骤

    NextStepLocation nslocation = getNextStepLocation();
    MFlowLoopWidget *loopWidget = new MFlowLoopWidget(this,this->controls[MCONSTANT_FLOW_MENU_TYPE_LoopBtn],
                                                  new MDownArrow(this),
                                                  new MDownArrow(this),nullptr,true);
    m_fcHelper->arrows.append(loopWidget->arrow);
    m_fcHelper->arrows.append(loopWidget->innerDefaultArrow);
    m_fcHelper->allstepwidgets.append(loopWidget);
    connect(loopWidget,&MFlowLoopWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);
    nslocation.layout->insertWidget(nslocation.target,loopWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,loopWidget->arrow,0,Qt::AlignCenter);

    MFlowStepParamsLoop *params = static_cast<MFlowStepParamsLoop*>(loopWidget->data->stepparams);

    params->stepURL = nslocation.ruleUrlNewStep;
    params->loopTypeName = loopTypeName;
    params->loopTypeValue = loopTypeValue;
    params->loopTextList = textList;


    m_fcHelper->setCurrentStepData(loopWidget->data);

    // 新增循环窗口中的提取字段窗口
    QTimer::singleShot(50,this,[=](){
        this->addFlowExtractStep(field,true);

    });

}
void PageCustomTaskFlowControl::addFlowLoopAndRollClickStep(){

    // 新增（循环+下拉刷新）步骤
    NextStepLocation nslocation = getNextStepLocation();


    MFlowLoopWidget *loopWidget = new MFlowLoopWidget(this,this->controls[MCONSTANT_FLOW_MENU_TYPE_LoopBtn],
                                                  new MDownArrow(this),
                                                  new MDownArrow(this),nullptr,true);
    m_fcHelper->arrows.append(loopWidget->arrow);
    m_fcHelper->arrows.append(loopWidget->innerDefaultArrow);
    m_fcHelper->allstepwidgets.append(loopWidget);

    connect(loopWidget,&MFlowLoopWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

    nslocation.layout->insertWidget(nslocation.target,loopWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,loopWidget->arrow,0,Qt::AlignCenter);
    MFlowStepParamsLoop *params = static_cast<MFlowStepParamsLoop*>(loopWidget->data->stepparams);

    params->stepURL = nslocation.ruleUrlNewStep;
    params->loopTypeName = MCONSTANT_RADIO_SINGLE;
    params->loopTypeValue = "下拉刷新";
    params->loopTextList = QStringList()<<"下拉刷新";

    m_fcHelper->setCurrentStepData(loopWidget->data);

    QTimer::singleShot(50,this,[=](){

        addFlowClickStep(params->stepURL,"nodeXpath","nodeText","下拉刷新",true,false,true);
    });

}
void PageCustomTaskFlowControl::addFlowLoopAndClickStep(const QString &nodeRuleUrl,
                                                        const QString &nodeXpath,
                                                        const QString &nodeText,
                                                        const QString &loopTypeName,
                                                        const QString &loopTypeValue,
                                                        const QStringList &textList,
                                                        const QString &clickName,
                                                        bool isNewTab,bool isTurnPage){
    // 新增（循环+点击）步骤
    NextStepLocation nslocation = getNextStepLocation();


    MFlowLoopWidget *loopWidget = new MFlowLoopWidget(this,this->controls[MCONSTANT_FLOW_MENU_TYPE_LoopBtn],
                                                  new MDownArrow(this),
                                                  new MDownArrow(this),nullptr,true);
    m_fcHelper->arrows.append(loopWidget->arrow);
    m_fcHelper->arrows.append(loopWidget->innerDefaultArrow);
    m_fcHelper->allstepwidgets.append(loopWidget);

    connect(loopWidget,&MFlowLoopWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

    nslocation.layout->insertWidget(nslocation.target,loopWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,loopWidget->arrow,0,Qt::AlignCenter);
    MFlowStepParamsLoop *params = static_cast<MFlowStepParamsLoop*>(loopWidget->data->stepparams);

    params->stepURL = nslocation.ruleUrlNewStep;

    params->loopTypeName = loopTypeName;
    params->loopTypeValue = loopTypeValue;
    params->loopTextList = textList;

    m_fcHelper->setCurrentStepData(loopWidget->data);

    QTimer::singleShot(50,this,[=](){

        addFlowClickStep(nodeRuleUrl,nodeXpath,nodeText,clickName,true,isNewTab,false,isTurnPage);
    });

}
void PageCustomTaskFlowControl::addFlowClickStep(const QString &ruleUrl,const QString &nodeXpath,const QString &nodeText,const QString &name,bool isLoop,bool isNewTab,bool isRoll,bool isTurnPage){

    NextStepLocation nslocation = getNextStepLocation();
    if(isLoop==false){
        if(nslocation.ruleUrlParentInnerLastStep==nslocation.ruleUrlParent){
            // 上一步ruleUrl 和 所属循环ruleUrl相等，则表示该click 也处于所属循环之中
            isLoop = true;
            isNewTab = true;
        }
    }


    // 新增提取数据的步骤

    MFlowButtonWidget *clickWidget = new MFlowButtonWidget(this,this->controls.value(MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn),new MDownArrow(this));

    m_fcHelper->arrows.append(clickWidget->arrow);
    m_fcHelper->allstepwidgets.append(clickWidget);
    clickWidget->currentParentWidget = nslocation.widget;
    connect(clickWidget,&MFlowButtonWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);


    nslocation.layout->insertWidget(nslocation.target,clickWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,clickWidget->arrow,0,Qt::AlignCenter);
    MFlowStepParamsClickEle *params =static_cast<MFlowStepParamsClickEle*>(clickWidget->data->stepparams);
    if(name!=""){
        clickWidget->title->setText(name);
        params->name = name;
    }
    params->stepURL = ruleUrl;
    params->eleXpath = nodeXpath;
    params->eleText = nodeText;
    params->isLoop = isLoop;
    params->isNewTab = isNewTab;
    params->isRoll = isRoll;
    params->isTurnPage = isTurnPage;

    clickWidget->data->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;
    m_fcHelper->setCurrentStepData(clickWidget->data);
    emit this->notificationToFlow(m_fcHelper->getCurrentStepData());


    m_fcHelper->updateBackground();
    if(!nslocation.noUpdateSize){
        m_fcHelper->updateFixedSize();
    }


}
void PageCustomTaskFlowControl::addFlowExtractStep(MParamsField *field,bool isLoop){

    NextStepLocation nslocation = getNextStepLocation();
    bool isMergeExtract = false;

    if(m_fcHelper->getCurrentStepData()->control->menuType == MCONSTANT_FLOW_MENU_TYPE_ExtractBtn){
        // 如果当前按钮窗体为提取字段，则考虑合并
        isMergeExtract = true;
    }

    if(isMergeExtract){
        //  CompareXpathResult res= MFlowTools::compareXpath(field->fieldXpath,
        //  m_fcHelper->getCurrentStepData()->m_xpath);
         MFlowStepParamsExtract *params = static_cast<MFlowStepParamsExtract*>(m_fcHelper->getCurrentStepData()->stepparams);

         params->pfs.append(field);
//         params->isLoop = isLoop// 合并进来的字段，不需要设置该参数

         m_fcHelper->getCurrentStepData()->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;

         emit this->notificationToFlow(m_fcHelper->getCurrentStepData());
    }else{
        // 新增提取数据的步骤

        MFlowButtonWidget *extractWidget = new MFlowButtonWidget(this,
            this->controls.value(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn),
            new MDownArrow(this));

        m_fcHelper->arrows.append(extractWidget->arrow);
        m_fcHelper->allstepwidgets.append(extractWidget);
        extractWidget->currentParentWidget = nslocation.widget;
        connect(extractWidget,&MFlowButtonWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);


        nslocation.layout->insertWidget(nslocation.target,extractWidget,0,Qt::AlignCenter);
        nslocation.layout->insertWidget(nslocation.target,extractWidget->arrow,0,Qt::AlignCenter);

        MFlowStepParamsExtract *params = static_cast<MFlowStepParamsExtract*>(extractWidget->data->stepparams);

        params->stepURL = nslocation.ruleUrlNewStep;
        params->pfs.append(field);
        params->isLoop = isLoop;


        extractWidget->data->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;

        // 将新增窗体步骤设置为当前窗体步骤
        m_fcHelper->setCurrentStepData(extractWidget->data);


        emit this->notificationToFlow(m_fcHelper->getCurrentStepData());


        m_fcHelper->updateBackground();
        if(!nslocation.noUpdateSize){
            m_fcHelper->updateFixedSize();
        }

    }


}

void PageCustomTaskFlowControl::addFlowInputStep(const QString &nodeXpath, const QString &nodeText, const QString text, bool isLoop){

    NextStepLocation nslocation = getNextStepLocation();
//    if(isLoop==false){
//        if(nslocation.ruleUrlParentInnerLastStep==nslocation.ruleUrlParent){
//            // 上一步ruleUrl 和 所属循环ruleUrl相等，则表示该click 也处于所属循环之中
//            isLoop = true;
//        }
//    }

    MFlowButtonWidget *inputWidget = new MFlowButtonWidget(this,this->controls.value(MCONSTANT_FLOW_MENU_TYPE_InputBtn),new MDownArrow(this));

    m_fcHelper->arrows.append(inputWidget->arrow);
    m_fcHelper->allstepwidgets.append(inputWidget);
    inputWidget->currentParentWidget = nslocation.widget;
    connect(inputWidget,&MFlowButtonWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);


    nslocation.layout->insertWidget(nslocation.target,inputWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,inputWidget->arrow,0,Qt::AlignCenter);
    MFlowStepParamsInput *params =static_cast<MFlowStepParamsInput*>(inputWidget->data->stepparams);

    params->stepURL = nslocation.ruleUrlNewStep;
    params->eleXpath = nodeXpath;
    params->eleText = nodeText;
    params->isLoop = isLoop;
    params->inputText = text;


    inputWidget->data->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;
    m_fcHelper->setCurrentStepData(inputWidget->data);
    emit this->notificationToFlow(m_fcHelper->getCurrentStepData());

    m_fcHelper->updateBackground();
    if(!nslocation.noUpdateSize){
        m_fcHelper->updateFixedSize();
    }


}
void PageCustomTaskFlowControl::addFlowMouseStep(const QString &nodeXpath, const QString &nodeText, bool isLoop){

    NextStepLocation nslocation = getNextStepLocation();
//    if(isLoop==false){
//        if(nslocation.ruleUrlParentInnerLastStep==nslocation.ruleUrlParent){
//            // 上一步ruleUrl 和 所属循环ruleUrl相等，则表示该click 也处于所属循环之中
//            isLoop = true;
//        }
//    }

    MFlowButtonWidget *mouseWidget = new MFlowButtonWidget(this,this->controls.value(MCONSTANT_FLOW_MENU_TYPE_MouseBtn),new MDownArrow(this));

    m_fcHelper->arrows.append(mouseWidget->arrow);
    m_fcHelper->allstepwidgets.append(mouseWidget);
    mouseWidget->currentParentWidget = nslocation.widget;
    connect(mouseWidget,&MFlowButtonWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);


    nslocation.layout->insertWidget(nslocation.target,mouseWidget,0,Qt::AlignCenter);
    nslocation.layout->insertWidget(nslocation.target,mouseWidget->arrow,0,Qt::AlignCenter);
    MFlowStepParamsMouse *params =static_cast<MFlowStepParamsMouse*>(mouseWidget->data->stepparams);

    params->stepURL = nslocation.ruleUrlNewStep;
    params->eleXpath = nodeXpath;
    params->eleText = nodeText;
    params->isLoop = isLoop;


    mouseWidget->data->notificationType = MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;
    m_fcHelper->setCurrentStepData(mouseWidget->data);
    emit this->notificationToFlow(m_fcHelper->getCurrentStepData());

    m_fcHelper->updateBackground();
    if(!nslocation.noUpdateSize){
        m_fcHelper->updateFixedSize();
    }

}
QString PageCustomTaskFlowControl::getStepURL(QWidget *widget) const{
    QString ruleUrl;
    QString widgetName = widget->metaObject()->className();

    if("MFlowButtonWidget"==widgetName){
        MFlowButtonWidget *buttonWidget = static_cast<MFlowButtonWidget *>(widget);
        ruleUrl = buttonWidget->data->stepparams->stepURL;
    }else if ("MFlowLoopWidget"==widgetName) {
        MFlowLoopWidget *loopWidget = static_cast<MFlowLoopWidget *>(widget);
        ruleUrl = loopWidget->data->stepparams->stepURL;
    }else if ("MFlowIfWidget"==widgetName) {
        MFlowIfWidget *ifWidget = static_cast<MFlowIfWidget *>(widget);
        ruleUrl = ifWidget->data->stepparams->stepURL;
    }

    return ruleUrl;
}
PageCustomTaskFlowControl::NextStepLocation PageCustomTaskFlowControl::getNextStepLocation(){

    int parentTypeMain = 11;
    int parentTypeLoop = 12;
    int parentTypeIf = 13;


    QWidget *parentWidget = this;
    QVBoxLayout *parentLayout = this->boxLayout;
    int parentType = parentTypeMain;
    int target =parentLayout->count() - 2; // 默认位置在主窗口的顶部


    NextStepLocation nslocation;

    // 如果当前按下的窗体存在 && 未被删除
    if(m_fcHelper->getCurrentStepData() && !m_fcHelper->getCurrentStepData()->widgetIsDelete){

         if(m_fcHelper->getCurrentStepData()->control->menuType == MCONSTANT_FLOW_MENU_TYPE_LoopBtn){
            // 如果 当前按下的窗体菜单类型为loop框，需要判断loop的类型，再决定添加的位置
               MFlowLoopWidget *currentLoop = static_cast<MFlowLoopWidget *>(m_fcHelper->getCurrentStepData()->widget);
               parentWidget =currentLoop;
               parentLayout = currentLoop->boxLayout;
               parentType = parentTypeLoop;

               MFlowStepParamsLoop *loopParams = static_cast<MFlowStepParamsLoop*>(currentLoop->data->stepparams);
               nslocation.ruleUrlParent = loopParams->stepURL;

               int innerCount = parentLayout->count();

               if(innerCount==1){// 空循环窗体
                   target = innerCount - 1;
               }else{
                   if(loopParams->loopTypeName==MCONSTANT_RADIO_SINGLE){// 单个元素类型的循环，翻页点击需要放在最后一步
                        target = innerCount - 1 - 2;
                   }else{
                        target = innerCount - 1;
                   }

               }


         }else if(m_fcHelper->getCurrentStepData()->control->menuType == MCONSTANT_FLOW_MENU_TYPE_JudgeBtn){
             // 如果 当前按下的窗体菜单类型为if框，直接在if框内顶部添加
               MFlowIfWidget *currentIf = static_cast<MFlowIfWidget *>(m_fcHelper->getCurrentStepData()->widget);
               parentWidget = currentIf;
               parentLayout = currentIf->boxLayout;
               parentType = parentTypeIf;

               nslocation.ruleUrlParent = currentIf->data->stepparams->stepURL;
               target = parentLayout->count() - 1;


         }else{
             // 如果 当前按下的窗体为按钮窗体，首先为当前的按钮窗体找到父窗体
             QWidget *currentParentWindow=static_cast<MFlowButtonWidget *>(m_fcHelper->getCurrentStepData()->widget)->currentParentWidget;
             QString currentParentWindowName = currentParentWindow->metaObject()->className();
             if("MFlowLoopWidget"==currentParentWindowName){
                 MFlowLoopWidget *currentLoop = static_cast<MFlowLoopWidget *>(currentParentWindow);
                 parentWidget =currentLoop;
                 parentLayout = currentLoop->boxLayout;
                 parentType = parentTypeLoop;

                 MFlowStepParamsLoop *loopParams = static_cast<MFlowStepParamsLoop*>(currentLoop->data->stepparams);
                 nslocation.ruleUrlParent = loopParams->stepURL;

                 int innerCount = parentLayout->count();
                 if(innerCount==1){// 空循环窗体
                     target = innerCount - 1;
                 }else{
                     if(loopParams->loopTypeName==MCONSTANT_RADIO_SINGLE){// 单个元素类型的循环
                          target = innerCount - 1 - 2;
                     }else{
                          target = innerCount - 1;
                     }
                 }


             }else if ("MFlowIfWidget"==currentParentWindowName) {
                 MFlowIfWidget *currentIf = static_cast<MFlowIfWidget *>(currentParentWindow);
                 parentWidget = currentIf;
                 parentLayout = currentIf->boxLayout;
                 parentType = parentTypeLoop;
                 nslocation.ruleUrlParent = currentIf->data->stepparams->stepURL;
                 target = parentLayout->count() - 1;
             }
         }
    }

    if(parentTypeMain==parentType){
        // 如果在主窗口，则起始步骤的 target = 1
        if(target>1){
            // 不在起始位置，存在上一步widget
            nslocation.ruleUrlParentInnerLastStep = getStepURL(parentLayout->itemAt(target-1)->widget());
        }

    }else{
        // 如果是非主窗口，则起始步骤的target = 0
        if(target>0){
            // 不在起始位置，存在上一步widget
            nslocation.ruleUrlParentInnerLastStep = getStepURL(parentLayout->itemAt(target-1)->widget());
        }
    }

    nslocation.widget = parentWidget;
    nslocation.layout = parentLayout;
    nslocation.target = target;
    nslocation.noUpdateSize = parentType == parentTypeMain ? true:false;

    if(nslocation.ruleUrlParentInnerLastStep!=""){// 如果本循环内上一步ruleUrl存在
        nslocation.ruleUrlNewStep = nslocation.ruleUrlParentInnerLastStep;//（场景：非循环内第一步）
    }else{
        nslocation.ruleUrlNewStep = nslocation.ruleUrlParent;// （场景：循环内第一步）
    }

    return nslocation;

}

void PageCustomTaskFlowControl::onNotificationFromChildWidget(MFlowStepData *data){

    if (MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT==data->notificationType){
       // 拖拽以成功的方式结束，需要更新布局
        m_fcHelper->updateFixedSize();
        m_fcHelper->setarrowsNormal();
        m_fcHelper->setCurrentStepData(data);
        m_fcHelper->updateBackground();

    }else if (MCONNSTANT_NOTIFICATION_TYPE_START==data->notificationType) {
       // 整个拖拽系统首次出现拖拽行为
        m_fcHelper->setarrowsPress();


    }else if (MCONNSTANT_NOTIFICATION_TYPE_OTHER_END==data->notificationType||
              MCONNSTANT_NOTIFICATION_TYPE_OTHER_END_SHOWCONTENT==data->notificationType) {
        // 所有拖拽行为结束，但是以取消的方式结束
        m_fcHelper->setarrowsNormal();
        m_fcHelper->setCurrentStepData(data);
        m_fcHelper->updateBackground();


    }else if (MCONNSTANT_NOTIFICATION_TYPE_DELETE_CURRENT_WIDGET) {
        // TODO 如果删除的widget是curentStepWidge。则需要重新寻找一个widget替代currentStepWidget
        m_fcHelper->removeStep(data->widget);
    }


    if(m_fcHelper->getCurrentStepData()->widgetIsDelete){
        qDebug()<<"stepwidget被删除";

    }else {
       if(MCONNSTANT_NOTIFICATION_TYPE_OTHER_END_SHOWCONTENT==data->notificationType ||
           MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT==data->notificationType){

            // 如果通知类型都包含显示右侧内容，则需要矫正stepData的状态类型
            m_fcHelper->getCurrentStepData()->notificationType = data->notificationType;
            emit this->notificationToFlow(m_fcHelper->getCurrentStepData());
            emit this->notificationToSmartAlertWidget(m_fcHelper->getCurrentStepData());
       }

    }

}




void PageCustomTaskFlowControl::dragEnterEvent(QDragEnterEvent *event)
{
    dragEnterEvery(event);

}
void PageCustomTaskFlowControl::dragEnterEvery(QDragEnterEvent *event){

//    qDebug()<<QString(m_this->metaObject()->className())<<"dragEnterEvent()";

    if (event->mimeData()->hasFormat("application/MFlowButtonWidget")) {
        event->acceptProposedAction();
    } else if (event->mimeData()->hasFormat("application/MFlowLoopWidget")) {
      event->acceptProposedAction();
    } else if (event->mimeData()->hasFormat("application/MFlowIfWidget")) {
            event->acceptProposedAction();
    }  else if (event->mimeData()->hasFormat("application/MFlowMenuToolButton")) {
         event->acceptProposedAction();
    } else {

        event->ignore();
        return;
    }

}
void PageCustomTaskFlowControl::dropEvery(QWidget *parentWidget,QVBoxLayout *boxLayout, QDropEvent *event){

//    qDebug()<<QString(m_this->metaObject()->className())<<"dropEvent()";

    QObject *tw = qobject_cast<QObject*>(parentWidget->childAt(event->pos()));
    if(!tw){
        return ;
    }
    if("MDownArrow"==QString(tw->metaObject()->className())){
        // 只有箭头区域才是合法的接收区域
        MDownArrow *targetArrow = qobject_cast<MDownArrow *>(tw);

        int targetIndex = boxLayout->indexOf(targetArrow);

        if (event->mimeData()->hasFormat("application/MFlowMenuToolButton")) {

            MControl *menu = qobject_cast<MFlowMenuToolButton *>(event->source())->data->control;

            if("MFlowButtonWidget"==menu->menuFlowWidgetName){

                MFlowButtonWidget *widget = new MFlowButtonWidget(this,
                                                                  menu,
                                                                  new MDownArrow(this));
                //如果是从侧边栏拖拽过来的新窗体，需要在这里重新覆盖掉
                m_fcHelper->setCurrentStepData(widget->data);
                m_fcHelper->arrows.append(widget->arrow);
                m_fcHelper->allstepwidgets.append(widget);
                widget->currentParentWidget = parentWidget;

                connect(widget,&MFlowButtonWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);


                boxLayout->insertWidget(targetIndex,widget,0,Qt::AlignCenter);
                boxLayout->insertWidget(targetIndex,widget->arrow,0,Qt::AlignCenter);

                event->setDropAction(Qt::MoveAction);
                event->accept();
            }else if ("MFlowLoopWidget"==menu->menuFlowWidgetName) {

                MFlowLoopWidget *widget = new MFlowLoopWidget(this,menu,
                                                              new MDownArrow(this),
                                                              new MDownArrow(this));
                //如果是从侧边栏拖拽过来的新窗体，需要在这里重新覆盖掉
                m_fcHelper->setCurrentStepData(widget->data);

                m_fcHelper->arrows.append(widget->arrow);
                m_fcHelper->arrows.append(widget->innerDefaultArrow);
                m_fcHelper->allstepwidgets.append(widget);
                widget->currentParentWidget = parentWidget;
                connect(widget,&MFlowLoopWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

                boxLayout->insertWidget(targetIndex,widget,0,Qt::AlignCenter);
                boxLayout->insertWidget(targetIndex,widget->arrow,0,Qt::AlignCenter);

                event->setDropAction(Qt::MoveAction);
                event->accept();


            }else if ("MFlowIfWidget"==menu->menuFlowWidgetName) {

                MFlowIfWidget *widget = new MFlowIfWidget(this,menu,
                                                          new MDownArrow(this),
                                                          new MDownArrow(this));
                //如果是从侧边栏拖拽过来的新窗体，需要在这里重新覆盖掉
                m_fcHelper->setCurrentStepData(widget->data);

                m_fcHelper->arrows.append(widget->arrow);
                m_fcHelper->arrows.append(widget->innerDefaultArrow);
                m_fcHelper->allstepwidgets.append(widget);
                widget->currentParentWidget = parentWidget;
                connect(widget,&MFlowIfWidget::notificationToParentWidget,this,&PageCustomTaskFlowControl::onNotificationFromChildWidget);

                boxLayout->insertWidget(targetIndex,widget,0,Qt::AlignCenter);
                boxLayout->insertWidget(targetIndex,widget->arrow,0,Qt::AlignCenter);

                event->setDropAction(Qt::MoveAction);
                event->accept();


            }else {
                event->ignore();
            }

        }else if (event->mimeData()->hasFormat("application/MFlowButtonWidget")) {
            QByteArray itemData = event->mimeData()->data("application/MFlowButtonWidget");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            QPoint offset;
            dataStream >>offset;
            MFlowButtonWidget *widget= static_cast<MFlowButtonWidget *>(event->source());
            boxLayout->insertWidget(targetIndex,widget,0,Qt::AlignCenter);
            boxLayout->insertWidget(targetIndex,widget->arrow,0,Qt::AlignCenter);

            widget->currentParentWidget = parentWidget;
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }else if(event->mimeData()->hasFormat("application/MFlowLoopWidget")) {
            QByteArray itemData = event->mimeData()->data("application/MFlowLoopWidget");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            QPoint offset;
            dataStream >>offset;
            MFlowLoopWidget *widget = static_cast<MFlowLoopWidget *>(event->source());

            boxLayout->insertWidget(targetIndex,widget,0,Qt::AlignCenter);
            boxLayout->insertWidget(targetIndex,widget->arrow,0,Qt::AlignCenter);

            widget->currentParentWidget = parentWidget;
            event->setDropAction(Qt::MoveAction);
            event->accept();

        }else if(event->mimeData()->hasFormat("application/MFlowIfWidget")) {
            QByteArray itemData = event->mimeData()->data("application/MFlowIfWidget");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            QPoint offset;
            dataStream >>offset;
            MFlowIfWidget *widget = static_cast<MFlowIfWidget *>(event->source());
            boxLayout->insertWidget(targetIndex,widget,0,Qt::AlignCenter);
            boxLayout->insertWidget(targetIndex,widget->arrow,0,Qt::AlignCenter);
            widget->currentParentWidget = parentWidget;
            event->setDropAction(Qt::MoveAction);
            event->accept();

        }else {
            event->ignore();
        }

    }

}

void PageCustomTaskFlowControl::dropEvent(QDropEvent *event){
    this->dropEvery(this,boxLayout,event);

}

