#include "Settings.h"
#include "SettingsProxyDialog.h"
#include "Utils/ApiCheckVersion.h"
#include "Utils/models.h"
#include "style.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "Task/TaskWebEngineProfile.h"
#include <QSettings>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QCheckBox>
#include <QTimer>
#include <QMessageBox>
#include <QsLog.h>

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    QLOG_INFO() << "Settings::Settings()";
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,2,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(new ComLineWidget(this));


    QWidget * boxWidget = new QWidget(this);
    mainLayout->addWidget(boxWidget);
    boxLayout = new QVBoxLayout(boxWidget);
    boxLayout->setContentsMargins(20,0,20,0);
    boxLayout->setSpacing(0);

    boxLayout->addSpacing(30);
    initUi();
    boxLayout->addStretch(10);

    // 检测更新监听
    connect(ApiCheckVersion::getInstance(),&ApiCheckVersion::notifyCheckVersion,this,[this](bool state,QString &msg,MVersion &version){
        loadingLabel->hide();
        loadingMsgLabel->setText(msg);
    });

}
Settings::~Settings(){
  QLOG_INFO() << "Settings::~Settings()";
}

void Settings::initUi(){

    QWidget *outWidget = new QWidget(this);
    outWidget->setFixedSize(600,160);
    outWidget->setStyleSheet(".QWidget{background-color:rgb(205,205,205);border-radius: 5px;}");
    QVBoxLayout *outLayout = new QVBoxLayout(outWidget);
    outLayout->setContentsMargins(1,1,1,1);

    QWidget * settingsWidget = new QWidget(outWidget);
    settingsWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);border-radius: 5px;}");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWidget);
    settingsLayout->setContentsMargins(20,0,20,0);
    settingsLayout->setSpacing(0);



    // 版本检测更新 start
    QWidget *versionWidget = new QWidget(settingsWidget);
    versionWidget->setFixedHeight(45);
    QHBoxLayout *versionHLayout = new QHBoxLayout(versionWidget);
    versionHLayout->setContentsMargins(0,0,0,0);
    versionHLayout->setSpacing(0);
    QPushButton *checkBtn = new QPushButton(versionWidget);
    checkBtn->setFixedSize(80,28);
    checkBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    checkBtn->setCursor(Qt::PointingHandCursor);
    checkBtn->setText("检查更新");
    QLabel *versionLabel = new QLabel(versionWidget);
    versionLabel->setStyleSheet(m_stylesheet_QLabel12);
    versionLabel->setText(QString("当前版本号：%1").arg(QCoreApplication::applicationVersion()));
    loadingLabel = new ComLoadingLabel(versionWidget);
    loadingMsgLabel = new QLabel(versionWidget);
    loadingMsgLabel->setStyleSheet(m_stylesheet_QLabel12);


    connect(checkBtn,&QPushButton::clicked,this,[this](){
        loadingLabel->show();
        loadingMsgLabel->setText("");
        ApiCheckVersion::getInstance()->checkVersion();
    });

    versionHLayout->addWidget(checkBtn);
    versionHLayout->addSpacing(10);
    versionHLayout->addWidget(versionLabel);
    versionHLayout->addSpacing(10);
    versionHLayout->addWidget(loadingLabel);
    versionHLayout->addSpacing(10);
    versionHLayout->addWidget(loadingMsgLabel);
    versionHLayout->addStretch(10);
    // 版本检测更新 end

    // 清空缓存 start
    QWidget *clearCacheWidget = new QWidget(settingsWidget);
    clearCacheWidget->setFixedHeight(45);
    QHBoxLayout *clearCacheHLayout = new QHBoxLayout(clearCacheWidget);
    clearCacheHLayout->setContentsMargins(0,0,0,0);
    clearCacheHLayout->setSpacing(0);

    QPushButton * clearCacheBtn = new QPushButton(clearCacheWidget);
    clearCacheBtn->setFixedSize(80,28);
    clearCacheBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    clearCacheBtn->setCursor(Qt::PointingHandCursor);
    clearCacheBtn->setText("清空缓存");

    QLabel *clearCacheLabel = new QLabel(clearCacheWidget);
    clearCacheLabel->setStyleSheet(m_stylesheet_QLabel12);
    clearCacheLabel->setText("清空浏览器缓存，历史记录，登录信息等");

    connect(clearCacheBtn,&QPushButton::clicked,this,[this](){
//        QMessageBox::information(NULL, "清空缓存", "清空缓存成功",QMessageBox::NoButton);



        auto answer = QMessageBox::question(this,tr("清空缓存"),tr("清空浏览器缓存，历史记录，登录信息等?"));
        if (answer == QMessageBox::Yes){
            TaskWebEngineProfile::getInstance()->clearCache();

            ComMessageBox::success(NULL,"清空缓存成功");
        }
    });
    clearCacheHLayout->addWidget(clearCacheBtn);
    clearCacheHLayout->addSpacing(10);
    clearCacheHLayout->addWidget(clearCacheLabel);
    clearCacheHLayout->addStretch(10);
    // 清空缓存 end
    // 代理 start
    QWidget *proxyWidget = new QWidget(settingsWidget);
    proxyWidget->setFixedHeight(45);
    QHBoxLayout *proxyHLayout = new QHBoxLayout(proxyWidget);
    proxyHLayout->setContentsMargins(0,0,0,0);
    proxyHLayout->setSpacing(0);

    QPushButton * proxyBtn = new QPushButton(proxyWidget);
    proxyBtn->setFixedSize(80,28);
    proxyBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    proxyBtn->setCursor(Qt::PointingHandCursor);
    proxyBtn->setText("代理");

    QLabel *proxyLabel = new QLabel(proxyWidget);
    proxyLabel->setStyleSheet(m_stylesheet_QLabel12);
    proxyLabel->setText("目前支持手动设置");

    connect(proxyBtn,&QPushButton::clicked,this,[this](){
        SettingsProxyDialog  dlg(this);
        dlg.exec();
    });
    proxyHLayout->addWidget(proxyBtn);
    proxyHLayout->addSpacing(10);
    proxyHLayout->addWidget(proxyLabel);
    proxyHLayout->addStretch(10);
    // 代理 end

    settingsLayout->addWidget(versionWidget);
    settingsLayout->addWidget(new ComLineWidget(this));
    settingsLayout->addWidget(clearCacheWidget);
    settingsLayout->addWidget(new ComLineWidget(this));
    settingsLayout->addWidget(proxyWidget);


    outLayout->addWidget(settingsWidget);
    boxLayout->addWidget(outWidget,0,Qt::AlignHCenter);

}

