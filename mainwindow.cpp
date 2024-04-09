#include "mainwindow.h"
#include "Tab.h"
#include "database.h"
#include "TaskImport.h"
#include "style.h"
#include "constant.h"
#include "Utils/models.h"
#include "Utils/ApiCheckVersion.h"
#include "Index/VersionDialog.h"
#include "Task/TaskWebEngineProfile.h"
#include <QDesktopServices>
#include <QUrl>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QAction>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QSettings>
#include <QSysInfo>
#include <QNetworkAccessManager>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>
#include <QScreen>
#include <QRect>
#include <QTimer>
#include <QsLog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(QString(".MainWindow{background-color:%1;}").arg(m_rgb_basic));
    setWindowTitle(QCoreApplication::applicationName());

    QList<QScreen *> screens = QGuiApplication::screens();//获取多屏幕
//    QScreen * screen = QGuiApplication::primaryScreen();//获取主屏幕
    QScreen * screen = screens.at(0);//获取多屏幕第一块屏幕（暂未做多屏幕的兼容）
    QRect screenRect = screen->geometry();
    int screenW = screenRect.width();
    int screenH = screenRect.height();
    int initW = screenW * 1080 / 1920;
    int initH = initW * 720 / 1080;

    QLOG_INFO() << "MainWindow::MainWindow() screens.size="<< screens.size()<<",screenW="<<screenW<<",screenH="<<screenH<<",initW="<<initW<<",initH="<<initH;
    this->resize(initW,initH);
    //开启调试
//    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "11124");
    // QtWebEngineProcess.exe与主进程合二为一
//    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    //版本显示audio标签control控件高度异常 设置这样即可正常显示
//    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-features=UseModernMediaControls");
    initMenu();

    tabWidget = new Tab(this);
    setCentralWidget(tabWidget);

    initSettings();
    tabWidget->createIndex();

    connect(ApiCheckVersion::getInstance(),&ApiCheckVersion::notifyCheckVersion,this,&MainWindow::onCheckVersion);
    QTimer::singleShot(2000,this,[](){
        TaskWebEngineProfile::getInstance();
        ApiCheckVersion::getInstance()->checkVersion();
    });

}
MainWindow::~MainWindow(){
    QLOG_INFO() << "MainWindow::~MainWindow()";
    Database::getInstance()->release();// 释放单例

}
void MainWindow::onCheckVersion(bool state,QString &msg,MVersion &version){
    //qDebug()<<"MainWindow::onCheckVersion state="<<state<<",msg="<<msg<<",version.version="<<version.version;

//    disconnect(ApiCheckVersion::getInstance(),&ApiCheckVersion::notifyCheckVersion,this,&MainWindow::onCheckVersion);
    if(state){
        float curVersion = QCoreApplication::applicationVersion().toFloat();
        if(version.version > curVersion){
//                VersionDialog *versionLog = new VersionDialog(this,version);
//                versionLog->setAttribute(Qt::WA_DeleteOnClose);
//                versionLog->show();
            VersionDialog dlg(this,version);
            dlg.exec();
        }
    }

}
void MainWindow::initMenu(){
    // 文件菜单
    QMenu *fileMenu = new QMenu(tr("文件"));

    QAction *taskAct = fileMenu->addAction("新建任务");
    taskAct->setShortcuts(QKeySequence::New);
    connect(taskAct, &QAction::triggered, this, [this]() {
         tabWidget->createTask();
    });

    QAction *taskManageAct = fileMenu->addAction("任务管理");
    connect(taskManageAct, &QAction::triggered, this, [this]() {
         tabWidget->createTaskManage();
    });


    QAction *importTaskAct = fileMenu->addAction("导入任务");
    connect(importTaskAct, &QAction::triggered, this, [this]() {
        TaskImport dlg(this);
        dlg.exec();
    });


    fileMenu->addAction(taskAct);
    fileMenu->addSeparator();
    fileMenu->addAction(taskManageAct);
    fileMenu->addSeparator();
    fileMenu->addAction(importTaskAct);



    // 帮助菜单
    QMenu *helpMenu = new QMenu(tr("&帮助"));
    QAction *settingsAct = helpMenu->addAction("设置");
        settingsAct->setShortcuts(QKeySequence::WhatsThis);
    connect(settingsAct, &QAction::triggered, this, [this]() {
        tabWidget->createSettings();
    });

    QAction *aboutAct = helpMenu->addAction("关于");
    aboutAct->setShortcuts(QKeySequence::Preferences);
    connect(aboutAct, &QAction::triggered, this, [this]() {
        tabWidget->createAbout();
    });

    QAction *feedbackAct = helpMenu->addAction("反馈");
    connect(feedbackAct, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl(URL_FEEDBACK));
    });

    QAction *documentAct = helpMenu->addAction("文档");
    connect(documentAct, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl(URL_DOCUMENT));
    });


    QAction *logoutAct = helpMenu->addAction("退出");
    logoutAct->setShortcuts(QKeySequence::Quit);
    connect(logoutAct, &QAction::triggered, this, [this]() {
        switch (QMessageBox::information(this,"提示","确认要退出吗？","确定","取消",0)){
            case 0:{
                QLOG_INFO() << "logout";
                QApplication* app;
                app->quit();
                break;
            }
        }
    });
    helpMenu->addAction(aboutAct);
    helpMenu->addSeparator();
    helpMenu->addAction(settingsAct);
    helpMenu->addSeparator();
    helpMenu->addAction(feedbackAct);
    helpMenu->addSeparator();
    helpMenu->addAction(documentAct);
    helpMenu->addSeparator();
    helpMenu->addAction(logoutAct);


    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);

}

void MainWindow::initSettings(){

    QSettings settings;


    if(settings.value(SETTINGS_KEY_OPEN_PROXY).toBool()){
        QNetworkProxyFactory::setUseSystemConfiguration(false);
        QNetworkProxy networkProxy;
        networkProxy.setType(QNetworkProxy::HttpProxy);
        networkProxy.setHostName(settings.value(SETTINGS_KEY_PROXY_IP).toString());
        networkProxy.setPort(settings.value(SETTINGS_KEY_PROXY_PORT).toString().toUInt());
        QString username = settings.value(SETTINGS_KEY_PROXY_USERNAME).toString();
        QString password = settings.value(SETTINGS_KEY_PROXY_PASSWORD).toString();

        if (username.length() > 0 && password.length()>0){
            networkProxy.setUser(username);
            networkProxy.setPassword(password);
        }
        QNetworkProxy::setApplicationProxy(networkProxy);
    }else{
        QNetworkProxyFactory::setUseSystemConfiguration(true);
    }

    QString finger;
    if(settings.value(SETTINGS_KEY_FINGER).toString().isNull()){
          finger = (QSysInfo::machineHostName()+"-"+QSysInfo::machineUniqueId()).toUtf8().toBase64();
          settings.setValue(SETTINGS_KEY_FINGER,finger);
    }else{
         finger = settings.value(SETTINGS_KEY_FINGER).toString();
    }
    Database::getInstance()->setFinger(finger);

}
