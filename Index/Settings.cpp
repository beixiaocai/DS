#include "Settings.h"
#include "SettingsProxy.h"
#include "Utils/ApiCheckVersion.h"
#include "Utils/models.h"
#include "style.h"
#include "Version.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLineWidget.h"
#include <QSettings>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QCheckBox>
#include <QTimer>


Settings::Settings(QWidget *parent) : QWidget(parent)
{
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
    initSettingsUi();
    boxLayout->addStretch(10);

}


void Settings::initSettingsUi(){

    QWidget *outWidget = new QWidget(this);
    outWidget->setFixedSize(600,110);
    outWidget->setStyleSheet(".QWidget{background-color:rgb(205,205,205);border-radius: 5px;}");
    QVBoxLayout *outLayout = new QVBoxLayout(outWidget);
    outLayout->setContentsMargins(1,1,1,1);

    QWidget * settingsWidget = new QWidget(outWidget);
    settingsWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);border-radius: 5px;}");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWidget);
    settingsLayout->setContentsMargins(20,0,20,0);
    settingsLayout->setSpacing(0);

    // 版本升级 start
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
    ComLoadingLabel *loadingLabel = new ComLoadingLabel(versionWidget);
    QLabel *noupdateLabel = new QLabel(versionWidget);
    noupdateLabel->setStyleSheet(m_stylesheet_QLabel12);


    connect(ApiCheckVersion::getInstance(),&ApiCheckVersion::notifyCheckVersion,this,[this,loadingLabel,noupdateLabel](bool state,QString &msg,MVersion &version){
        loadingLabel->hide();
        if(state){
            float curVersion = QCoreApplication::applicationVersion().toFloat();
            if(version.version > curVersion){
                Version dlg(version,this);
                dlg.exec();
            }else{

                noupdateLabel->setText(msg);
            }
        }else{
            noupdateLabel->setText(msg);
        }

    });


    connect(checkBtn,&QPushButton::clicked,this,[this,loadingLabel,noupdateLabel](){
        loadingLabel->show();
        noupdateLabel->setText("");
        ApiCheckVersion::getInstance()->checkVersion();
    });

    versionHLayout->addWidget(checkBtn);
    versionHLayout->addSpacing(10);
    versionHLayout->addWidget(versionLabel);
    versionHLayout->addSpacing(10);
    versionHLayout->addWidget(loadingLabel);
    versionHLayout->addSpacing(10);
    versionHLayout->addWidget(noupdateLabel);
    versionHLayout->addStretch(10);
    // 版本升级 end


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
        SettingsProxy  dlg(this);
        dlg.exec();
    });
    proxyHLayout->addWidget(proxyBtn);
    proxyHLayout->addSpacing(10);
    proxyHLayout->addWidget(proxyLabel);
    proxyHLayout->addStretch(10);
    // 代理 end




    settingsLayout->addWidget(versionWidget);
    settingsLayout->addWidget(new ComLineWidget(this));
    settingsLayout->addWidget(proxyWidget);


    outLayout->addWidget(settingsWidget);
    boxLayout->addWidget(outWidget,0,Qt::AlignHCenter);

}

