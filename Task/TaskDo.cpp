#include "TaskDo.h"
#include "TaskSmartAlertDialog.h"
#include "TaskWebEngineView.h"
#include "TaskBrowserMenu.h"
#include "TaskFlow/pageCustomtaskFlow.h"
#include "TaskFlow/pageCustomtaskFlowControl.h"
#include "Run/StartupDialog.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "Utils/ComSplitVWidget.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "Utils/ComLoadingLabel.h"
#include "style.h"
#include <QVBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QsLog.h>

TaskDo::TaskDo(QWidget *parent,MTask *task) : QWidget(parent),mTask(task){


    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);
    boxLayout->setSpacing(0);

    initTopMenuUi();
    boxLayout->addWidget(new ComLineWidget(this));
    initFlowUi();
    initBottomUi();
    nameLine->setText(mTask->name);

}
TaskDo::~TaskDo()
{
    disconnect(webView,0,0,0);
    delete webView;
    webView = nullptr;
}
void TaskDo::startLoad(int msec){
    QTimer::singleShot(msec,this,[this](){
        if(mSmartAlertDialog!=nullptr){
            loadUrl(mTask->addressList[0]);

            if(mTask->type==MTask::TASKTYPE::ADD){
                // 新增任务（默认新增第一步流程）
                flow->fc->setFlowAddressList(mTask->addressList,true);
            }else{
                // 编辑任务（反序列化任务流程图并复原）
                flow->fc->deserialize();
            }
        }else{
            startLoad(20);
        }
    });
}

void TaskDo::initTopMenuUi(){

    // 智能选项的智能菜单
    QMenu *tipMenu = new QMenu(this);
    tipMenu->setStyleSheet(".QMenu{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:13px;border:1px solid rgb(64,65,66);border-radius: 3px; padding: 1px;}");

    QAction *pullA = tipMenu->addAction(tr("生成下拉刷新流程"));
    connect(pullA, &QAction::triggered, this,[this](){
       QTimer::singleShot(1000,this,[this](){
             flow->fc->addFlowLoopAndRollClickStep();
       });

    });

    tipMenu->addAction(pullA);

    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(45);
    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(10,0,10,0);
    topLayout->setSpacing(0);

    boxLayout->addWidget(topWidget);

    nameLine = new QLineEdit(topWidget);
    nameLine->setStyleSheet(m_stylesheet_QLineEdit);
    nameLine->setFixedHeight(30);


    loadingLabel = new ComLoadingLabel(topWidget);


    QPushButton *saveBtn = new QPushButton(topWidget);
    saveBtn->setText("保存");
    saveBtn->setStyleSheet(m_stylesheet_QPushButton);
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setFixedSize(80,28);
    connect(saveBtn,&QPushButton::clicked,this,[this](){

        loadingLabel->show();

        mTask->name = nameLine->text().trimmed();
        emit this->notifyChangeTabName(mTask->name);

        flow->fc->serialize(); // 任务流程序列化

        QString msg;
        bool state = Database::getInstance()->addTask(mTask,msg);
        loadingLabel->hide();
        if(state){
            // 保存成功
            ComMessageBox::success(this,msg);
        }else{
           ComMessageBox::error(this,msg);
        }

    });

    QPushButton *executeBtn = new QPushButton(topWidget);
    executeBtn->setText("执行");
    executeBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    executeBtn->setCursor(Qt::PointingHandCursor);
    executeBtn->setFixedSize(80,28);
    connect(executeBtn,&QPushButton::clicked,this,[this](){
        // 执行的时候，先保存一下，保存成功后，再进行执行
        loadingLabel->show();
        flow->fc->serialize(); // 任务流程序列化


        QString msg;
        bool state = Database::getInstance()->addTask(mTask,msg);
        loadingLabel->hide();
        if(state){
            // 保存成功->进入执行页面
            StartupDialog *startup = new StartupDialog(new MTask(*mTask));
            startup->setAttribute(Qt::WA_DeleteOnClose);
            startup->show();
        }else{
           ComMessageBox::error(this,msg);
        }

    });
    QPushButton *settingsBtn = new QPushButton(topWidget);
    settingsBtn->setText("设置");
    settingsBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    settingsBtn->setCursor(Qt::PointingHandCursor);
    settingsBtn->setFixedSize(80,28);
    connect(settingsBtn,&QPushButton::clicked,this,[this](){
       emit this->notifySettings();
    });

    QToolButton *tipBtn = new QToolButton(topWidget);
    tipBtn->setToolTip("智能选项");
    tipBtn->setStyleSheet(m_stylesheet_QToolButton_empty);
    tipBtn->setIcon(QIcon(":/res/images/icon/flow/tip.png"));
    tipBtn->setIconSize(QSize(28,30));
    tipBtn->setCursor(Qt::PointingHandCursor);
    connect(tipBtn,&QToolButton::clicked,this,[tipMenu](){
        tipMenu->exec(QCursor::pos());
    });


    QCheckBox *flowCheckBox = new QCheckBox(topWidget);
    flowCheckBox->setStyleSheet(m_stylesheet_QCheckBox);
    flowCheckBox->setText(tr("流程"));
    flowCheckBox->setCursor(Qt::PointingHandCursor);

    connect(flowCheckBox,&QCheckBox::toggled,this,[this](bool checked){
        if(checked){
             flowWidget->show();
        }else{
             flowWidget->hide();
        }
    });

    topLayout->addWidget(nameLine);
    topLayout->addSpacing(10);
    topLayout->addWidget(loadingLabel);
    topLayout->addSpacing(10);
    topLayout->addWidget(saveBtn);
    topLayout->addSpacing(10);
    topLayout->addWidget(executeBtn);
    topLayout->addSpacing(10);
    topLayout->addWidget(settingsBtn);
    topLayout->addSpacing(10);
    topLayout->addWidget(tipBtn);
    topLayout->addSpacing(10);
    topLayout->addWidget(flowCheckBox);
    topLayout->addSpacing(5);

}
void TaskDo::initFlowUi(){
    flowWidget = new QWidget(this);

    flowWidget->setFixedHeight(300);

    boxLayout->addWidget(flowWidget);
    flowWidget->hide();
    QTimer::singleShot(100,this,[this](){
        QVBoxLayout *flowLayout = new QVBoxLayout(flowWidget);
        flowLayout->setContentsMargins(0,0,0,0);
        flowLayout->setSpacing(0);

        flow = new PageCustomTaskFlow(mTask,flowWidget);
        ComSplitVWidget *splitVWidget = new ComSplitVWidget(flowWidget);
        splitVWidget->setFixedHeight(4);

        connect(splitVWidget,&ComSplitVWidget::moveDistance,this,[=](int distance){
            int h = flowWidget->height()+distance;
            if(h>60){
                flowWidget->setFixedHeight(h);
            }
        });
        flowLayout->addWidget(flow);
        flowLayout->addWidget(splitVWidget);
    });

}
void TaskDo::initBottomUi(){
    QWidget *bottomWidget = new QWidget(this);
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(0,0,0,0);
    bottomLayout->setSpacing(0);
    boxLayout->addWidget(bottomWidget);

    // 第一行
    QWidget *bottomMenuWidget = new QWidget(bottomWidget);
    bottomMenuWidget->setFixedHeight(36);
    QHBoxLayout *bottomMenuHLayout = new QHBoxLayout(bottomMenuWidget);
    bottomMenuHLayout->setContentsMargins(10,0,10,0);
    bottomMenuHLayout->setSpacing(0);

    addrEdit = new QLineEdit(bottomMenuWidget);
    addrEdit->setFixedHeight(28);
    addrFavAction = new QAction(addrEdit);
    addrEdit->addAction(addrFavAction, QLineEdit::LeadingPosition);
    addrEdit->setStyleSheet(m_stylesheet_QLineEdit);
    addrEdit->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    connect(addrEdit, &QLineEdit::returnPressed, this,[this](){
        this->loadUrl(addrEdit->text().trimmed());
    });

    QToolButton *refreshBtn = new QToolButton(bottomMenuWidget);
    refreshBtn->setToolTip("刷新");
    refreshBtn->setStyleSheet(m_stylesheet_QToolButton_empty);
    refreshBtn->setIcon(QIcon(":/res/images/icon/flow/refresh.png"));
    refreshBtn->setIconSize(QSize(25,25));
    refreshBtn->setCursor(Qt::PointingHandCursor);

    connect(refreshBtn,&QToolButton::clicked,this,[this](){
        this->loadUrl(addrEdit->text().trimmed());
    });

    QCheckBox *modeCheck = new QCheckBox(bottomMenuWidget);
    modeCheck->setCursor(Qt::PointingHandCursor);
    modeCheck->setStyleSheet(m_stylesheet_QCheckBox);
    modeCheck->setText("浏览器模式");

    smartAlertCheck = new QCheckBox(bottomMenuWidget);
    smartAlertCheck->setCursor(Qt::PointingHandCursor);
    smartAlertCheck->setStyleSheet(m_stylesheet_QCheckBox);
    smartAlertCheck->setText("智能提示");

    bottomMenuHLayout->addWidget(addrEdit);
    bottomMenuHLayout->addSpacing(10);
    bottomMenuHLayout->addWidget(refreshBtn);
    bottomMenuHLayout->addSpacing(5);
    bottomMenuHLayout->addWidget(modeCheck);
    bottomMenuHLayout->addSpacing(10);
    bottomMenuHLayout->addWidget(smartAlertCheck);



    //第二行
    browserMenu = new TaskBrowserMenu(bottomWidget);
    browserMenu->setFixedHeight(40);
    browserMenu->hide();

    progressBar = new QProgressBar(bottomWidget);
    progressBar->setFixedHeight(2);
    progressBar->setTextVisible(false);
    progressBar->setStyleSheet(QStringLiteral("QProgressBar {border: 0px} QProgressBar::chunk {background-color: #da4453}"));

    //第三行
    webView = new TaskWebEngineView(bottomWidget,mTask);

    bottomLayout->addWidget(bottomMenuWidget);
    bottomLayout->addWidget(new ComLineWidget(this));
    bottomLayout->addWidget(progressBar);
    bottomLayout->addWidget(browserMenu);

    bottomLayout->addWidget(webView);

    connect(webView, &TaskWebEngineView::urlChanged, this, [this](const QUrl &url){
         addrEdit->setText(url.url());
         addrEdit->setCursorPosition(0);
    });
    connect(webView, &QWebEngineView::loadStarted, this,[this](){
        webView->loadProgress = 0;
        progressBar->setValue(0);

        addrFavAction->setIcon(webView->favIcon());
    });
    connect(webView, &QWebEngineView::loadProgress, [this](int progress) {
        webView->loadProgress = progress;
        progressBar->setValue(progress);
    });
    connect(webView, &QWebEngineView::iconChanged, [this](const QIcon & icon ) {
        addrFavAction->setIcon(icon);
        emit this->notifyChangeTabIcon(icon);
    });
    connect(webView, &QWebEngineView::loadFinished,this,[this](bool finished){
        webView->loadProgress = finished ? 100 : -1;

        progressBar->setValue(0);

        if(mTask->isHaveName==false){
            // 首次赋予名字
            QString title = webView->title();
            if(title.length()>0){
                mTask->name = title;
                nameLine->setText(mTask->name);
                mTask->isHaveName=true;
                emit this->notifyChangeTabName(mTask->name);
            }
        }
    });

    connect(modeCheck,&QCheckBox::toggled,this,[this](bool checked){
        if(checked){
            // 浏览器模式，删除注入的js
           browserMenu->show();
           webView->injectJavascript(false);
           webView->reload();
           if(smartAlertCheck->checkState()==Qt::CheckState::Checked){
                smartAlertCheck->setChecked(false);
            }
        }else {
            // 规则模式，重新注入js
           browserMenu->hide();
           webView->injectJavascript(true);
           webView->reload();
        }
    });

    connect(smartAlertCheck,&QCheckBox::toggled,this,[this](bool checked){
        //智能显示按钮
        if(isFirstShowSmartAlert){
            QPoint p = QCursor().pos();
            mSmartAlertDialog->move(p.x()+25,p.y()+10);
            isFirstShowSmartAlert = false;
        }
        if(checked){
            mSmartAlertDialog->show();
        }else{
            mSmartAlertDialog->hide();
        }
    });

    initSmartAlertView(200);
}
void TaskDo::initSmartAlertView(int msec){
    QTimer::singleShot(msec,this,[this](){
        if(flow!=nullptr){
            mSmartAlertDialog = new TaskSmartAlertDialog(this,webView,flow->fc);
            connect(mSmartAlertDialog,&TaskSmartAlertDialog::notifySmartAlertChecked,[this](bool checked){
                if(checked&& smartAlertCheck->checkState()==Qt::CheckState::Checked){
                    // 显示命令 && 当前智能提示按钮显示选中 -> 则忽略
                }else {
                    smartAlertCheck->setChecked(checked);
                }
            });
        }else{
           initSmartAlertView(20);
        }
      });
}

void TaskDo::loadUrl(QString url){

    if (url.startsWith("http://") || url.startsWith("https://")){
    }else{
        url ="http://"+url;
    }
    QLOG_INFO() << "TaskDo::loadUrl() url="<<url;
    webView->load(QUrl(url));
    if(smartAlertCheck->checkState()==Qt::CheckState::Checked){
        // 如果当前智能提示框被选中，则重置显示
        if(mSmartAlertDialog!=nullptr){
            mSmartAlertDialog->block();
        }
    }
}


