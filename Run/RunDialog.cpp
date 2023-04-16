#include "RunDialog.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "TaskFlow/mFcModels.h"
#include "Task/TaskDataExportDialog.h"
#include "RunHelper.h"
#include "RunWebEngineView.h"
#include "RunWebEngineViewManager.h"
#include "RunThreadPipline.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QCloseEvent>
#include <QTimer>
#include <QWebEngineHttpRequest>
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
#include <QtCore5Compat>
#endif
#include <QsLog.h>


const static int WAIT_SLEEP_UNIT = 1000; // 执行前，休眠单位:毫秒
const static int PAGESIZE = 10; // 执行页面的数据分页数


RunDialog::RunDialog(MTask *task) : QDialog(nullptr), mTask(task){
    QLOG_INFO() << "RunDialog::RunDialog()";

    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinMaxButtonsHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    resize(1000,600);
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(QString(".RunDialog{background-color:%1;}").arg(m_rgb_basic));
    setWindowTitle(mTask->name);

    mHelper = new RunHelper(this);
    mThreadPipline = new RunThreadPipline(this);
    connect(mThreadPipline,&RunThreadPipline::notifyJobFinished,this,&RunDialog::onThreadPiplineJobFinished);
    connect(mThreadPipline,&RunThreadPipline::notifyUpdateData,this,&RunDialog::onThreadPiplineUpdateData,Qt::QueuedConnection);
    connect(mThreadPipline,&RunThreadPipline::notifyAlert,this,&RunDialog::onThreadPiplineAlert);

    connect(this,&RunDialog::notifyPiplinePushData,mThreadPipline,&RunThreadPipline::onPushData,Qt::QueuedConnection);
    mMessage = new RunMessage(this);
    // init UI
    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);
    boxLayout->setSpacing(0);

    initTopMenuUi();
    initWebViewUi();
    initBottomUi();

    mWebViewManager = new RunWebEngineViewManager(this);//默认打开一个webview
//     默认加载 '打开网页'
    addrLine->setText(mTask->addressList[0]);
    addrLine->setCursorPosition(0);
    mWebViewManager->getCurrentWebView()->load(QUrl(addrLine->text()));

//    // 默认以数据库的字段显示
    mFields = Database::getInstance()->getTableFields(task->code);

//    // 设置header（这里是数据库的字段名称为准）
    tableWidget->setColumnCount(mFields.length());
    tableWidget->setHorizontalHeaderLabels(mFields);

    exportBtn->show();
    startBtn->show();

    if(Database::getInstance()->getTaskData(mTask->code)){
        // 存在数据
        tableStateBtn->setArrowType(Qt::ArrowType::DownArrow);
        getData();
        tableWidget->show();
    }
    if(mTask->runParams.isLoop){
        //开启循环采集
        sendStartJobCommand();
    }

}
RunDialog::~RunDialog(){
    QLOG_INFO() << "RunDialog::~RunDialog()";

    if(mThreadPipline){
        delete mThreadPipline;
        mThreadPipline = nullptr;
    }
    if(mHelper){
        delete mHelper;
        mHelper = nullptr;
    }
    if(mMessage){
        delete mMessage;
        mMessage = nullptr;
    }
    if(mWebViewManager){
        delete mWebViewManager;
        mWebViewManager = nullptr;
    }
    if(mTask){
        delete mTask;
        mTask = nullptr;
    }

}
RunHelper *RunDialog::getHelper(){
    return mHelper;
}
MTask *RunDialog::getTask(){
    return mTask;
}
void RunDialog::closeEvent(QCloseEvent *event){
    if(mThreadPipline->jobFinished){
        event->accept();

    }else{
        event->ignore();
        ComMessageBox::error(this,"任务执行中，无法关闭窗口，需要先停止采集");
    }
}
void RunDialog::startNextStep(QString lastStepID){
    if(mThreadPipline->jobCommandState){

        MFlowStepParams *np = mHelper->getNextStepParams(lastStepID);
        lastStepID.clear();

        if(np){
            QLOG_INFO() << QString("RunDialog startNextStep() name=%1").arg(np->name);

            QString menuType = np->menuType;
            if(MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn==menuType){
                startOpenWeb(static_cast<MFlowStepParamsOpenWeb *>(np));
             }else if(MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn==menuType){
                startClickEle(static_cast<MFlowStepParamsClickEle *>(np));
             }else if(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn==menuType){
                startExtract(static_cast<MFlowStepParamsExtract *>(np));
             }else if(MCONSTANT_FLOW_MENU_TYPE_InputBtn==menuType){
                startInput(static_cast<MFlowStepParamsInput *>(np));
             }else if(MCONSTANT_FLOW_MENU_TYPE_LoopBtn==menuType){
                startLoopHead(static_cast<MFlowStepParamsLoop *>(np));
             }else if(MCONSTANT_FLOW_MENU_TYPE_MouseBtn==menuType){
                startMouse(static_cast<MFlowStepParamsMouse *>(np));
             }else{
                sendStopJobCommand(RunMessage::MessageType::WARNING,"暂不支持的窗体！");
                return ;
             }
        }else {
            sendStopJobCommand(RunMessage::MessageType::SUCCESS,"本次执行成功");
            return ;
        }


    }
}
void RunDialog::startOpenWeb(MFlowStepParamsOpenWeb *params){
     QString url = params->url;
     if(params->carryParent!=nullptr){//当前步骤存在父窗体
        if(params->carryParent->menuType==MCONSTANT_FLOW_MENU_TYPE_LoopBtn){
            MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);

            if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                 url = ploop->carryAddressList[ploop->carryAddressCurrent-1];
            }else{
                sendStopJobCommand(RunMessage::MessageType::ERROR,"打开网页所属循环必须是网址列表循环，否则循环无意义！");
                return ;
            }

        }else{
            sendStopJobCommand(RunMessage::MessageType::WARNING,"（01x）暂不支持if窗体！");
            return ;
        }
     }
     QTimer::singleShot(WAIT_SLEEP_UNIT*params->waitSleep,this,[this,url,params](){
         addrLine->setText(url);

         QWebEngineHttpRequest request;
         QUrl qurl(url);

         request.setUrl(qurl);
         request.setMethod(QWebEngineHttpRequest::Get);
     //    request.setHeader(QByteArray("Cache-Control"),QByteArray("no-cache"));
     //    request.setHeader(QByteArray("Pragma"),QByteArray("no-cache"));
     //    QWebEngineCookieStore* cookieStore = m_currentWebView->page()->profile()->cookieStore();
     //    cookieStore->deleteAllCookies();
     //    cookieStore->setCookie(QNetworkCookie("a","b"),qurl);
     //    cookieStore->setCookie(QNetworkCookie("c","d"),qurl);
         mWebViewManager->getCurrentWebView()->load(request);


         QTimer *timer = new QTimer();
         connect(timer,&QTimer::timeout,this,[this,timer,params](){
            if(mWebViewManager->pageIsFinished){
                timer->stop();
                delete timer;
                // 判断是否滚动
                if(params->isRoll){
                    mWebViewManager->getCurrentWebView()->page()->runJavaScript(QString("startRoll('%1','%2','%3')").arg(params->rollCount).arg(params->rollInterval).arg(params->rollTypeIndex),
                                                                           [this,params](const QVariant &v) {
                          QString rf = v.toString();
                          if(rf.startsWith("success")){
                              endOpenWeb(params);
                          }else{
                              sendStopJobCommand(RunMessage::MessageType::ERROR,"（01x）打开网页滚动反馈出错！");
                              return ;
                          }
                     });
                }else{
                    endOpenWeb(params);
                }
            }
         });
         timer->start(50);

     });

}
void RunDialog::startClickEle(MFlowStepParamsClickEle *params){

    QString iloopTypeName;
    QString iloopTypeValue;
    int     inum = 0;
    bool    iisLast = false;// false:不属于循环最后一步 true:属于循环最后一步

    if(params->carryParent!=nullptr){//当前步骤存在父窗体
         if(params->carryParent->menuType==MCONSTANT_FLOW_MENU_TYPE_LoopBtn){

             MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
             iloopTypeName = ploop->loopTypeName;
             iloopTypeValue = ploop->loopTypeValue;

             if(params->carryStepLocation==MFlowStepParams::LocationType::Last){//最后步骤
                 iisLast = true;
                 if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                     //循环翻页（一定是单独点击）
                    inum = 0;

                 }else if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                    // 网址列表的循环中，最后步骤的点击，一定是单独点击
                    QLOG_INFO() << QString("Run.cpp startClickEle() carryAddressCurrent=%1, carryAddressTotal=%2").arg(ploop->carryAddressCurrent).arg(ploop->carryAddressTotal);

                    inum = 0;

                 }else {
                    // 固定元素，非固定元素
                    QLOG_INFO() << QString("Run.cpp startClickEle() carryCurrent=%1, carryTotal=%2").arg(ploop->carryCurrent).arg(ploop->carryTotal);
                    inum = ploop->carryCurrent;
                }

             }else {// 非最后步骤
                 if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                    // 单个元素的循环中，存在中间步骤的点击，一定是单独点击
                    inum = 0;
                 }else if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                     // 网址列表的循环中，存在中间步骤的点击，一定是单独点击
                    inum = 0;
                 }else {
                    inum = ploop->carryCurrent;
                }
           }
         }else{
             sendStopJobCommand(RunMessage::MessageType::WARNING,"（02x）暂不支持if窗体！");
             return ;
         }
    }else {// 不属于循环的点击，一定是单独点击
         inum = 0;
    }
    if(params->eleXpath=="" && params->eleXpath==""){
        // 自定义拖拽的按钮，不需要执行网页点击，通常用于延迟和滚动至网页底部
        startClickEleRoll(params,"success:wait100,[自定义的点击按钮]");

    }else{
        QString script = QString("startClickEle('%1','%2','%3','%4','%5','%6','%7','%8','%9')").
           arg(iloopTypeName).arg(iloopTypeValue).arg(inum).arg(params->eleXpath).arg(params->eleText).arg(params->isLoop).arg(params->isNewTab).
           arg(iisLast).arg(params->isTurnPage);

        QTimer::singleShot(WAIT_SLEEP_UNIT*params->waitSleep,this,[this,script,params](){
             mWebViewManager->getCurrentWebView()->page()->runJavaScript(script,[this,params](const QVariant &v) {
                  QString feedback = v.toString();
                  QLOG_INFO() << QString("Run.cpp startClickEle() feedback=%1").arg(feedback);

                  if(feedback.startsWith("success")){
                      startClickEleRoll(params,feedback);
                  }else{
                      sendStopJobCommand(RunMessage::MessageType::ERROR,"（02x0001）点击元素失败，请确认规则后重试！");
                      return ;
                  }
             });
        });

    }

}
void RunDialog::startClickEleRoll(MFlowStepParamsClickEle *params, QString feedback){
      QStringList rps = feedback.mid(8).split(",");
      QString p1 = rps[0];// dropdown,wait100,wait300
      bool clickNext= rps[1]=="next"?true:false;// 点击元素之后后，是否存在next返回值（存在next表示：循环翻页存在下一页或者 下拉刷新存在下一页）
      feedback.clear();

      if("dropdown"==p1) {
          // 页面内下拉刷新
         if(params->isRoll){

             mWebViewManager->getCurrentWebView()->page()->runJavaScript(QString("startRoll('%1','%2','%3')").arg(params->rollCount).arg(params->rollInterval).arg(params->rollTypeIndex),
                                                                    [this,params](const QVariant &v) {
                   QString rf = v.toString();
                   QLOG_INFO() << QString("Run.cpp startClickEleRoll() dropdown.rf=%1").arg(rf);
                   if(rf.startsWith("success")){
                       bool clickRollNext = rf.mid(8).startsWith("next")?true:false; // next 滚动存在，bottom 滚动已经到底部
                       endClickEle(params,clickRollNext);
                   }else{
                       sendStopJobCommand(RunMessage::MessageType::ERROR,"（02x0015）点击元素滚动反馈出错！");
                       return ;
                   }
              });

         }else{
             sendStopJobCommand(RunMessage::MessageType::ERROR,"（02x0010）页面内下拉刷新规则错误！");
             return ;
         }
      }else{
          // wait100,wait200,,,

          int wait = p1.mid(4).toInt();// 休眠的毫秒

          QTimer::singleShot(wait,this,[this,params,clickNext](){

              QTimer *timer = new QTimer();
              connect(timer,&QTimer::timeout,this,[this,timer,params,clickNext](){
                 if(mWebViewManager->pageIsFinished){
                     timer->stop();
                     delete timer;

                     if(params->isRoll){
                         mWebViewManager->getCurrentWebView()->page()->runJavaScript(QString("startRoll('%1','%2','%3')").arg(params->rollCount).arg(params->rollInterval).arg(params->rollTypeIndex),
                                                                                [this,params,clickNext](const QVariant &v) {
                               QString rf = v.toString();
                               if(rf.startsWith("success")){
                                   endClickEle(params,clickNext);
                               }else{
                                   sendStopJobCommand(RunMessage::MessageType::ERROR,"（02x0016）点击元素滚动反馈出错！");
                                   return ;
                               }
                          });
                     }else{
                         endClickEle(params,clickNext);
                     }
                 }
              });
              timer->start(50);
          });
      }
}
void RunDialog::startExtract(MFlowStepParamsExtract * params){
    QString iloopTypeName;
    QString iloopTypeValue;
    int     inum=0;

    if(params->carryParent!=nullptr){//当前步骤存在父窗体
        if(params->carryParent->menuType==MCONSTANT_FLOW_MENU_TYPE_LoopBtn){
            MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
            iloopTypeName = ploop->loopTypeName;
            iloopTypeValue = ploop->loopTypeValue;

            if(params->carryStepLocation==MFlowStepParams::LocationType::Last){// 最后步骤
                if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                  sendStopJobCommand(RunMessage::MessageType::ERROR,"（03x）提取不能作单个元素循环的的最后一步，规则错误！");
                  return ;
                }else if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                   // 网址列表的循环中，最后步骤的提取，一定是单独提取
                  QLOG_INFO() << QString("Run.cpp startExtract() carryAddressCurrent=%1,carryAddressTotal=%2").arg(ploop->carryAddressCurrent).arg(ploop->carryAddressTotal);
                  inum = 0;
                }else { // 固定或非固定
                  QLOG_INFO() << QString("Run.cpp startExtract() carryCurrent=%1,carryTotal=%2").arg(ploop->carryCurrent).arg(ploop->carryTotal);
                  inum = ploop->carryCurrent;
                }
            }else {// 非最后步骤
                if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                    // 单个元素的循环中，存在中间步骤的提取，一定是单独提取
                    inum = 0;
                }else if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                    //  网址列表的循环中，存在中间步骤的提取，一定是单独提取
                    inum = 0;
                }else { // 固定或非固定
                    inum = ploop->carryCurrent;
                }
           }
        }else{
            sendStopJobCommand(RunMessage::MessageType::WARNING,"（03x）暂不支持if窗体！");
            return ;
        }
    }else {//循环之外
        inum = 0;
    }
    QString script = QString("startExtract('%1','%2','%3','%4','%5')").
            arg(iloopTypeName).arg(iloopTypeValue).arg(inum).arg(params->isLoop).arg(params->carryFields);

    QTimer::singleShot(WAIT_SLEEP_UNIT*params->waitSleep,this,[this,script,params](){
        mWebViewManager->getCurrentWebView()->page()->runJavaScript(script, [this,params](const QVariant &v) {
             QString feedback = v.toString();
             if(feedback.startsWith("success")){
                 endExtract(params,feedback);
             }else{
                 sendStopJobCommand(RunMessage::MessageType::ERROR,"提取字段失败，请确认规则后重试！");
                 return ;
             }
        });
    });


}
void RunDialog::startInput(MFlowStepParamsInput *params){

    QString iloopTypeName;
    QString iloopTypeValue;
    int     inum=0;

    if(params->carryParent!=nullptr){//当前步骤存在父窗体
        if(params->carryParent->menuType==MCONSTANT_FLOW_MENU_TYPE_LoopBtn){
            MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
            iloopTypeName = ploop->loopTypeName;
            iloopTypeValue = ploop->loopTypeValue;

            if(params->carryStepLocation==MFlowStepParams::LocationType::Last){// 最后步骤
                if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                  sendStopJobCommand(RunMessage::MessageType::ERROR,"输入文字不能作单个元素循环的的最后一步，规则错误！");
                  return ;
                }
            }
         }else{
            sendStopJobCommand(RunMessage::MessageType::WARNING,"（04x）暂不支持if窗体！");
            return ;
        }
   }
    QString script = QString("startInput('%1','%2','%3','%4','%5','%6')").arg(iloopTypeName).arg(iloopTypeValue).arg(inum).
           arg(params->eleXpath).arg(params->inputText).arg(params->isLoop);

    QTimer::singleShot(WAIT_SLEEP_UNIT*params->waitSleep,this,[this,script,params](){
        mWebViewManager->getCurrentWebView()->page()->runJavaScript(script,[this,params](const QVariant &v) {
            QString feedback = v.toString();
            QLOG_INFO() << QString("Run.cpp startInput() feedback=%1").arg(feedback);
            if(feedback.startsWith("success")){
                endInput(params);
            }else{
                sendStopJobCommand(RunMessage::MessageType::ERROR,"输入文字失败，请确认规则后重试！");
                return ;
            }
        });
    });


}

void RunDialog::startLoopHead(MFlowStepParamsLoop * params){
    QLOG_INFO() << "RunDialog startLoopHead() begin";

    if (params->loopTypeName==MCONSTANT_RADIO_SINGLE) {
        // 单个元素 => 循环翻页 直接进入下一步
        params->carrySingleCurrent = 1;
        startNextStep(params->stepID);

    }else if (params->loopTypeName==MCONSTANT_RADIO_SITEADDRESS) {
        // 网址列表 => 循环执行网址列表
        QStringList addressList = params->loopTypeValue.split(",");
        params->carryAddressList = addressList;
        params->carryAddressTotal =addressList.length();
        params->carryAddressCurrent = 1;
        startNextStep(params->stepID);

    }else if (params->loopTypeName==MCONSTANT_RADIO_FIXED || params->loopTypeName==MCONSTANT_RADIO_UNFIXED) {
        // 固定 非固定循环
         QString script = QString("startLoopHead('%1','%2')").arg(params->loopTypeName,params->loopTypeValue);
         QTimer::singleShot(WAIT_SLEEP_UNIT*params->waitSleep,this,[this,params,script](){
            mWebViewManager->getCurrentWebView()->page()->runJavaScript(script,[this,params](const QVariant &v) {
                QString feedback = v.toString();
                QLOG_INFO() << QString("Run.cpp startLoopHead() feedback=%1").arg(feedback);

                if(feedback.startsWith("success")){
                    int total = feedback.mid(8).toInt();
                    if(params->isEndLoopWithTimes){//限制循环次数，则以最小循环次数为准
                        if(params->endLoopTimes<total){
                            total = params->endLoopTimes;
                        }
                    }
                    params->carryTotal = total;
                    params->carryCurrent = 1;
                    params->carryCurrentWebView = mWebViewManager->getCurrentWebView();
                    startNextStep(params->stepID);
                }else{
                    sendStopJobCommand(RunMessage::MessageType::ERROR,"获取循环失败，请确认规则后重试！");
                    return ;
                }
            });
         });
    }

}
void RunDialog::startLoopTail(QString loopStepID,QString loopInnerLastStepID){
      QLOG_INFO() << "RunDialog startLoopTail() begin";

      MFlowStepParamsLoop * params = static_cast<MFlowStepParamsLoop *>(mHelper->m_stepsH[loopStepID]);

      if(params->carryParent!=nullptr){
            MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
            if(params->carryStepLocation==MFlowStepParams::LocationType::Last){
                if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                    sendStopJobCommand(RunMessage::MessageType::ERROR,"（07x）循环不能作（单个元素循环）的的最后一步，请修改规则！");
                    return ;
                }else if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                       if(ploop->carryAddressCurrent < ploop->carryAddressTotal){
                           ploop->carryAddressCurrent+=1;
                           //循环的外层循环周期 未结束，重新进入外层循环的第一步
                           startNextStep(ploop->stepID);
                       }else{//循环的外层循环周期 已结束，则判断是否有必要进入循环的外层循环tail
                           if(ploop->carryParent!=nullptr){
                                 startLoopTail(ploop->carryParent->stepID,ploop->stepID);
                           }else{
                                 startNextStep(loopInnerLastStepID);
                           }
                       }
                }else{
                      emit mWebViewManager->cleanWebViews(ploop->carryCurrentWebView);

                      if(ploop->carryCurrent < ploop->carryTotal){
                          ploop->carryCurrent+=1;
                          //循环的外层循环周期 未结束，重新进入外层循环的第一步
                          startNextStep(ploop->stepID);
                      }else{//循环的外层循环周期 已结束，则判断是否有必要进入循环的外层循环tail
                          if(ploop->carryParent!=nullptr){
                                startLoopTail(ploop->carryParent->stepID,ploop->stepID);
                          }else{
                                startNextStep(loopInnerLastStepID);
                          }
                      }
                }


            }else{
                //该循环所属的循环，不在最后一步，则继续向前即可
                startNextStep(loopInnerLastStepID);
            }
      }else{
          //该循环不在循环内，直接进入innerLast 的下一步
            startNextStep(loopInnerLastStepID);
      }

}
void RunDialog::startMouse(MFlowStepParamsMouse *params){

    QString iloopTypeName;
    QString iloopTypeValue;
    int     inum=0;

    if(params->carryParent!=nullptr){//当前步骤存在父窗体
        if(params->carryParent->menuType==MCONSTANT_FLOW_MENU_TYPE_LoopBtn){
            MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
            iloopTypeName = ploop->loopTypeName;
            iloopTypeValue = ploop->loopTypeValue;

            if(params->carryStepLocation==MFlowStepParams::LocationType::Last){// 最后步骤
                if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                  sendStopJobCommand(RunMessage::MessageType::ERROR,"移动鼠标不能作单个元素循环的的最后一步，规则错误！");
                  return ;
                }
            }
         }else{
            sendStopJobCommand(RunMessage::MessageType::WARNING,"（08x）暂不支持if窗体！");
            return ;
        }
   }
    QString script = QString("startMouse('%1','%2','%3','%4','%5')").arg(iloopTypeName).arg(iloopTypeValue).arg(inum).
           arg(params->eleXpath).arg(params->isLoop);

    QTimer::singleShot(WAIT_SLEEP_UNIT*params->waitSleep,this,[this,script,params](){
        mWebViewManager->getCurrentWebView()->page()->runJavaScript(script,[this,params](const QVariant &v) {
            QString feedback = v.toString();
            QLOG_INFO() << QString("Run.cpp startMouse() feedback=%1").arg(feedback);

            if(feedback.startsWith("success")){
                endMouse(params);
            }else{
                sendStopJobCommand(RunMessage::MessageType::ERROR,"移动鼠标失败，请确认规则后重试！");
                return ;
            }
        });
    });


}
void RunDialog::endOpenWeb(MFlowStepParamsOpenWeb *params){
    if(params->carryParent!=nullptr){
        if(params->carryStepLocation==MFlowStepParams::LocationType::Last){
            sendStopJobCommand(RunMessage::MessageType::ERROR,"打开网页不能作循环最后一步！");
            return ;
        }else {//第一步或者中间步骤，继续向前
            startNextStep(params->stepID);
        }
    }else {//循环之外
        startNextStep(params->stepID);
    }

}

void RunDialog::endClickEle(MFlowStepParamsClickEle * params,bool next ){

    QString lastStepID = params->stepID;

    if(params->carryParent!=nullptr){

        MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
        if(params->carryStepLocation==MFlowStepParams::LocationType::Last){
            if(ploop->loopTypeName==MCONSTANT_RADIO_SINGLE){
                if(next){// 点击翻页存在下一页 或 下拉刷新执行成功
                    ploop->carrySingleCurrent+=1;
                    if(ploop->isEndLoopWithTimes){
                        if(ploop->carrySingleCurrent>=ploop->endLoopTimes){
                           // 点击翻页满足循环次数结束
                            this->startLoopTail(ploop->stepID,params->stepID);
                            return ;
                        }
                    }
                    lastStepID = ploop->stepID;
                }else{
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
            }else if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                if(ploop->carryAddressCurrent<ploop->carryAddressTotal){
                    ploop->carryAddressCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
            }else {
                emit mWebViewManager->cleanWebViews(ploop->carryCurrentWebView);
                if(ploop->carryCurrent<ploop->carryTotal){
                    ploop->carryCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
           }
        }
    }
    startNextStep(lastStepID);


}
void RunDialog::endExtract(MFlowStepParamsExtract * params,QString feedback){

    QString res = feedback.mid(8);
    emit this->notifyPiplinePushData(params->stepID,res);
    feedback.clear();

    QString lastStepID = params->stepID;

    if(params->carryParent!=nullptr){
        MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
        if(params->carryStepLocation==MFlowStepParams::LocationType::Last){
            // 提取不能作为循环的最后一步
            if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                if(ploop->carryAddressCurrent<ploop->carryAddressTotal){
                    ploop->carryAddressCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
            }else {
                emit mWebViewManager->cleanWebViews(ploop->carryCurrentWebView);

                if(ploop->carryCurrent<ploop->carryTotal){
                    ploop->carryCurrent+=1;
                    lastStepID = ploop->stepID;
                 }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                 }
            }
        }
    }

   startNextStep(lastStepID);

}
void RunDialog::endInput(MFlowStepParamsInput *params){

    QString lastStepID = params->stepID;

    if(params->carryParent!=nullptr){

        MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
        if(params->carryStepLocation==MFlowStepParams::LocationType::Last){
            // 文本输入不能作为单元素循环的最后一步
            if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                if(ploop->carryAddressCurrent<ploop->carryAddressTotal){
                    ploop->carryAddressCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
            }else {
                emit mWebViewManager->cleanWebViews(ploop->carryCurrentWebView);
                if(ploop->carryCurrent<ploop->carryTotal){
                    ploop->carryCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
           }
        }
    }
    startNextStep(lastStepID);

}
void RunDialog::endMouse(MFlowStepParamsMouse *params){
    QString lastStepID = params->stepID;

    if(params->carryParent!=nullptr){

        MFlowStepParamsLoop * ploop = static_cast<MFlowStepParamsLoop *>(params->carryParent);
        if(params->carryStepLocation==MFlowStepParams::LocationType::Last){
            // 移动鼠标不能作为单元素循环的最后一步
            if(ploop->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
                if(ploop->carryAddressCurrent<ploop->carryAddressTotal){
                    ploop->carryAddressCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
            }else {
                emit mWebViewManager->cleanWebViews(ploop->carryCurrentWebView);

                if(ploop->carryCurrent<ploop->carryTotal){
                    ploop->carryCurrent+=1;
                    lastStepID = ploop->stepID;
                }else {
                    this->startLoopTail(ploop->stepID,params->stepID);
                    return ;
                }
           }
        }
    }
    startNextStep(lastStepID);
}

void RunDialog::sendStartJobCommand(){
    loadingLabel->show();

    mFields = mHelper->fields;// 用任务中的字段代替数据表的字段，并重新赋header
    tableWidget->setColumnCount(mFields.length());
    tableWidget->setHorizontalHeaderLabels(mFields);

    // 任务已经开始，可以做一些业务逻辑
    //TODO

    exportBtn->hide();
    startBtn->hide();
    stopBtn->show();
    stopBtn->setEnabled(true);

    tableWidget->setRowCount(0);
    tableWidget->clearContents();

    mThreadPipline->sendStartCommand(mTask->runParams.loopPeriod);

    updatePageData(0,false);

    QString lastStepID = NULL;//NULL开始的为从头开始执行
    startNextStep(lastStepID);


}
void RunDialog::sendStopJobCommand(RunMessage::MessageType messageType,const QString &msg){
    mThreadPipline->sendStopCommand();
    stopBtn->setEnabled(false);
    stopBtn->setText("停止中");

    mSendStopMessageType = messageType;
    mSendStopMsg = msg;
}

void RunDialog::onThreadPiplineJobFinished(){
    // 任务已经确认被停止，可以做一些业务逻辑
    //TODO
    exportBtn->show();
    startBtn->show();
    stopBtn->hide();
    stopBtn->setText("停止采集");
    loadingLabel->hide();
    if(mTask->runParams.isLoop){
        if(mTask->runParams.loopIsContinue){
            int duration = 1000 * 60 *mTask->runParams.loopInterval;
            QTimer::singleShot(duration,this,[this](){
                ++mTask->runParams.loopPeriod;
                this->sendStartJobCommand();
            });
        }else{
            mMessage->showMessage(mSendStopMessageType,mSendStopMsg);
        }
    }else{
        mMessage->showMessage(mSendStopMessageType,mSendStopMsg);
    }

//    QTimer *timer = new QTimer(this);
//    connect(timer,&QTimer::timeout,this,[=](){
//        if(mThreadPipline->jobFinished){
//            timer->stop();
//            timer->disconnect();
//            timer->deleteLater();
//        }
//    });
//    timer->start(100);


}
void RunDialog::onThreadPiplineAlert(int type,QString info){
    if(1==type){
        bottomLogLabel->setText(info);
        info.clear();
    }else if (11==type) {
        sendStopJobCommand(RunMessage::MessageType::ERROR,info);
    }else{
        sendStopJobCommand(RunMessage::MessageType::SUCCESS,info);
    }
}

void RunDialog::onThreadPiplineUpdateData(int num,QStringList names,QStringList values){
    updatePageData(num,true);

    if(num%PAGESIZE==0){
        tableWidget->setRowCount(0);
        tableWidget->clearContents();
    }

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setRowHeight(row,30);

    for (int i = 0; i < names.length(); ++i) {
        int column = mHelper->fieldsH[names[i]];
        tableWidget->setItem(row,column,new QTableWidgetItem(values[i]));
    }
    tableWidget->scrollToBottom();

    names.clear();
    values.clear();

}
void RunDialog::onCleanWebViews(RunWebEngineView *webView){
    addrLine->setText(webView->url().url());

    int wl = webViewStacked->count();
    if(wl < 5){ // stacked中的webView小于5个则判断是否存在
        bool isExist = false;
        for (int i=0;i<wl;++i) {
            if(webViewStacked->widget(i)==webView){
                isExist = true;
                break;
            }
        }
        if(!isExist){
           webViewStacked->addWidget(webView);
        }

    }else{
        for (int i=0;i<wl;++i) {
            webViewStacked->removeWidget(webViewStacked->widget(i));
        }
        webViewStacked->addWidget(webView);
    }
    webViewStacked->setCurrentWidget(webView);
}

void RunDialog::onWebViewUrlChanged(const QString &url){
    addrLine->setText(url);
}

void RunDialog::initTopMenuUi(){
    QWidget *menuWidget = new QWidget(this);
    menuWidget->setFixedHeight(40);
    QHBoxLayout *menuHLayout = new QHBoxLayout(menuWidget);
    menuHLayout->setContentsMargins(10,0,10,0);
    menuHLayout->setSpacing(0);

    addrLine = new QLineEdit(menuWidget);
    addrLine->setStyleSheet(m_stylesheet_QLineEdit);
    addrLine->setClearButtonEnabled(true);
    addrLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    addrLine->setFixedHeight(28);

    loadingLabel = new ComLoadingLabel(menuWidget);


    QPushButton *refreshBtn = new QPushButton(menuWidget);
    refreshBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    refreshBtn->setFixedSize(70,25);
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setText("刷新");

    connect(refreshBtn,&QPushButton::clicked,this,[this](){
        if(mThreadPipline->jobFinished){
             mWebViewManager->getCurrentWebView()->load(QUrl(addrLine->text().trimmed()));

        }else{
            ComMessageBox::error(this,"任务执行中，无法刷新");
        }
    });


    menuHLayout->addWidget(addrLine);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(loadingLabel);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(refreshBtn);
    menuHLayout->addSpacing(10);
    boxLayout->addWidget(menuWidget);

}

void RunDialog::initWebViewUi(){
    webViewStacked = new QStackedWidget(this);
    webViewStacked->setContentsMargins(0,0,0,0);
    boxLayout->addWidget(new ComLineWidget(this));
    boxLayout->addWidget(webViewStacked);

    // webViewStacked
//    QWidget *webViewWidget = new QWidget(this);
//    QVBoxLayout *webViewLayout = new QVBoxLayout(webViewWidget);
//    webViewLayout->setContentsMargins(0,0,0,0);
//    webViewLayout->setSpacing(0);
//    webViewStacked = new QStackedWidget(webViewWidget);
//    webViewLayout->addWidget(webViewStacked);

//    boxLayout->addWidget(new ComLineWidget(this));
//    boxLayout->addWidget(webViewWidget);
}
void RunDialog::updatePageData(int num,bool isShowLastPage){
    if(num==0){// num == 0 恢复为默认参数
        mCurPage = 1;
    }
    int totalPage = num / PAGESIZE; // 总页数
    if((num % PAGESIZE) > 0){
        totalPage +=1;
    }
    mTotalPage = totalPage;
    if(isShowLastPage){// 是否显示最后的数据对应的页面
        mCurPage = mTotalPage;
    }

    pageLabel->setText(QString::number(mCurPage));
    pageLogLabel->setText(QString("共计%1条，%2页").arg(num).arg(mTotalPage));
}
void RunDialog::getData(int p){
    mCurPage = p;
    tableWidget->setRowCount(0);
    tableWidget->clearContents();
    QTimer::singleShot(100,this,[this](){
        updatePageData(Database::getInstance()->getTableRowCount(mTask->code),false);//更新page
        int fieldCount = mFields.length();
        QString sql = QString("select %1 from %2 limit %3,%4 ").arg(mFields.join(",")).arg(mTask->code).arg(PAGESIZE * (mCurPage-1)).arg(PAGESIZE);
        QVector<QVector<QString>> data = Database::getInstance()->select(fieldCount,sql);
        tableWidget->setRowCount(data.size());

        for (int row = 0; row < data.size(); ++row) {
            tableWidget->setRowHeight(row,30);
            for (int column = 0; column < fieldCount; ++column) {
                QTableWidgetItem *item = new QTableWidgetItem(data[row][column]);
                tableWidget->setItem(row,column,item);
            }
        }
        tableWidget->scrollToBottom();
    });
}

void RunDialog::initBottomUi(){
    // 状态
    QWidget *stateWidget = new QWidget(this);
    stateWidget->setFixedHeight(30);
    QHBoxLayout *stateHLayout = new QHBoxLayout(stateWidget);
    stateHLayout->setContentsMargins(0,0,0,0);

    tableStateBtn = new QToolButton(stateWidget);
    tableStateBtn->setCursor(Qt::PointingHandCursor);
    tableStateBtn->setFixedSize(20,20);
    tableStateBtn->setAutoRaise(true);
    tableStateBtn->setArrowType(Qt::ArrowType::RightArrow);

    QLabel *stateLabel = new QLabel(stateWidget);
    stateLabel->setStyleSheet(m_stylesheet_QLabel12);
    stateLabel->setText("箭头向下可查看数据");

    stateHLayout->addStretch(10);
    stateHLayout->addWidget(tableStateBtn);
    stateHLayout->addSpacing(6);
    stateHLayout->addWidget(stateLabel);
    stateHLayout->addStretch(10);

    tableWidget = new QTableWidget(this);
    tableWidget->setFixedHeight(300);
    tableWidget->setFrameShape(QFrame::NoFrame);//设置无边框
    tableWidget->setFocusPolicy(Qt::NoFocus); //去除虚边框
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中一行(整行选中)
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表格
    tableWidget->verticalHeader()->setVisible(true);
    tableWidget->horizontalHeader()->setVisible(true);
    tableWidget->hide();


    connect(tableStateBtn,&QToolButton::clicked,this,[this](){
        if(tableStateBtn->arrowType()==Qt::ArrowType::DownArrow){
            tableWidget->hide();
            tableStateBtn->setArrowType(Qt::ArrowType::RightArrow);
        }else if (tableStateBtn->arrowType()==Qt::ArrowType::RightArrow) {
            tableWidget->show();
            tableStateBtn->setArrowType(Qt::ArrowType::DownArrow);
        }
    });

    // 翻页行
    QWidget *turnWidget = new QWidget(this);
    turnWidget->setFixedHeight(30);
    QHBoxLayout *turnHLayout = new QHBoxLayout(turnWidget);
    turnHLayout->setContentsMargins(0,0,0,0);
    turnHLayout->setSpacing(6);

    QToolButton *last = new QToolButton(this);
    last->setText("<");
    QToolButton *next = new QToolButton(this);
    next->setText(">");

    pageLabel = new QLabel(this);
    pageLabel->setStyleSheet(m_stylesheet_QLabel12);
    pageLogLabel = new QLabel(this);
    pageLogLabel->setStyleSheet(m_stylesheet_QLabel12);
    updatePageData(0,false);

    turnHLayout->addStretch(10);
    turnHLayout->addWidget(last);
    turnHLayout->addWidget(pageLabel);
    turnHLayout->addWidget(next);
    turnHLayout->addWidget(pageLogLabel);
    turnHLayout->addStretch(10);

    connect(last,&QToolButton::clicked,this,[this](){
        if(mCurPage>1){
            getData(mCurPage-1);
        }
    });
    connect(next,&QToolButton::clicked,this,[this](){
        if(mCurPage<mTotalPage){
            getData(mCurPage+1);
        }
    });


    // 底部菜单行
    QWidget *bottomMenuWidget = new QWidget(this);
    bottomMenuWidget->setFixedHeight(40);
    QHBoxLayout *bottomMenuHLayout = new QHBoxLayout(bottomMenuWidget);
    bottomMenuHLayout->setContentsMargins(10,0,10,0);
    bottomMenuHLayout->setSpacing(0);

    bottomLogLabel = new QLabel(bottomMenuWidget);

    exportBtn = new QPushButton(bottomMenuWidget);
    exportBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    exportBtn->setFixedSize(70,28);
    exportBtn->setCursor(Qt::PointingHandCursor);
    exportBtn->setText(tr("导出数据"));

    startBtn = new QPushButton(bottomMenuWidget);
    startBtn->setStyleSheet(m_stylesheet_QPushButton);

    startBtn->setCursor(Qt::PointingHandCursor);
    if(!mTask->runParams.isLoop){
        startBtn->setFixedSize(70,28);
        startBtn->setText(tr("开始采集"));
    }else{
        startBtn->setFixedSize(120,28);
        startBtn->setText(tr("定时启动中"));
    }

    stopBtn = new QPushButton(bottomMenuWidget);
    stopBtn->setStyleSheet(m_stylesheet_QPushButton);
    stopBtn->setFixedSize(70,28);
    stopBtn->setCursor(Qt::PointingHandCursor);
    stopBtn->setText(tr("停止采集"));

    exportBtn->hide();
    startBtn->hide();
    stopBtn->hide();

    if(!mTask->runParams.isLoop){
        connect(startBtn,&QPushButton::clicked,this,[this](){
            sendStartJobCommand();
        });
    }
    connect(exportBtn,&QPushButton::clicked,this,[this](){
        TaskDataExportDialog *dlg = new TaskDataExportDialog(this,mTask->name,mTask->code);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();
    });

    connect(stopBtn ,&QPushButton::clicked,this,[this](){
        if(mTask->runParams.isLoop){
            mTask->runParams.loopIsContinue = false;
            sendStopJobCommand(RunMessage::MessageType::WARNING,"（循环采集被主动终止）中止采集");
        }else{
            sendStopJobCommand(RunMessage::MessageType::WARNING,"中止采集");
        }
    });

    bottomMenuHLayout->addWidget(bottomLogLabel);
    bottomMenuHLayout->addStretch(10);
    bottomMenuHLayout->addWidget(exportBtn);
    bottomMenuHLayout->addSpacing(10);
    bottomMenuHLayout->addWidget(startBtn);
    bottomMenuHLayout->addSpacing(10);
    bottomMenuHLayout->addWidget(stopBtn);


    boxLayout->addWidget(new ComLineWidget(this));
    boxLayout->addWidget(stateWidget);
    boxLayout->addWidget(new ComLineWidget(this));
    boxLayout->addWidget(tableWidget);
    boxLayout->addWidget(turnWidget);
    boxLayout->addWidget(new ComLineWidget(this));
    boxLayout->addWidget(bottomMenuWidget);
}
