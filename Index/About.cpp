#include "About.h"
#include "style.h"
#include "constant.h"
#include "Utils/ComLineWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>
#include <QDebug>

About::About(QWidget *parent) : QWidget(parent)
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
    initTopUi();
    boxLayout->addSpacing(20);
    initSoftwareUi();
    boxLayout->addStretch(10);
}

void About::initTopUi(){

    QWidget *outWidget = new QWidget(this);
    outWidget->setFixedSize(600,220);
    outWidget->setStyleSheet(".QWidget{background-color:rgb(205,205,205);border-radius: 5px;}");
    QVBoxLayout *outLayout = new QVBoxLayout(outWidget);
    outLayout->setContentsMargins(1,1,1,1);

    QWidget *topWidget = new QWidget(outWidget);
    topWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);border-radius: 5px;}");
    QVBoxLayout *topLayout = new QVBoxLayout(topWidget);
    topLayout->setContentsMargins(0,0,0,0);
    // about logo+名称
    QWidget *logoWidget = new QWidget(topWidget);
    logoWidget->setFixedHeight(60);
    QHBoxLayout *logoHLayout = new QHBoxLayout(logoWidget);
    logoHLayout->setContentsMargins(0,0,0,0);

    QLabel *logo = new QLabel(logoWidget);
    logo->setPixmap(QIcon(":/res/images/logo.png").pixmap(36,40));

    QLabel *nameLabel = new QLabel(logoWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel30);
    nameLabel->setText(QCoreApplication::applicationName());

    logoHLayout->addSpacing(20);
    logoHLayout->addWidget(logo);
    logoHLayout->addSpacing(10);
    logoHLayout->addWidget(nameLabel);
    logoHLayout->addStretch(10);


    // about-声明
    QWidget *smWidget = new QWidget(topWidget);
    QHBoxLayout *smHLayout = new QHBoxLayout(smWidget);
    smHLayout->setContentsMargins(0,0,0,0);
    smWidget->setFixedHeight(100);
    QLabel *smLabel = new QLabel(smWidget);
    smLabel->setStyleSheet(m_stylesheet_QLabel);
    smLabel->setText("声明");

    QLabel *smContentLabel = new QLabel(smWidget);
    smContentLabel->setFixedWidth(500);
//    smContentLabel->setGeometry(QRect(328, 240, 329, 27*4)); //四倍行距
    smContentLabel->setWordWrap(true);
    smContentLabel->setText(tr("<p style='font-family:Microsoft YaHei;font-size:14px;margin:10px;line-height:25px;'>本产品只是采集数据的工具,我们不对数据版权负责。我们尊重版权,您必须在采集之前联系数据所有者取得授权,"
                            "任何因数据授权产生的问题由使用者负生部责任,另外对于数据的使用请严格遭守《国家网络安全法》,"
                            "使用本软件即代表悠同意此声明</p>"));
    smHLayout->addSpacing(20);
    smHLayout->addWidget(smLabel,0,Qt::AlignCenter);
    smHLayout->addSpacing(10);
    smHLayout->addWidget(smContentLabel);
    smHLayout->addStretch(10);


    topLayout->addWidget(logoWidget);
    topLayout->addWidget(new ComLineWidget(this));

    topLayout->addWidget(smWidget);

    topLayout->addStretch(10);

    outLayout->addWidget(topWidget);
    boxLayout->addWidget(outWidget,0,Qt::AlignHCenter);

}

void About::initSoftwareUi(){
    QWidget *outWidget = new QWidget(this);
    outWidget->setFixedSize(600,180);
    outWidget->setStyleSheet(".QWidget{background-color:rgb(205,205,205);border-radius: 5px;}");
    QVBoxLayout *outLayout = new QVBoxLayout(outWidget);
    outLayout->setContentsMargins(1,1,1,1);

    QWidget * softwareWidget = new QWidget(outWidget);
    softwareWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);border-radius: 5px;}");
    QVBoxLayout *softwareLayout = new QVBoxLayout(softwareWidget);
    softwareLayout->setContentsMargins(20,10,20,10);

    QLabel *nameLabel = new QLabel(softwareWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel14);
    nameLabel->setText("DS");

    QLabel *copyrightLabel = new QLabel(softwareWidget);
    copyrightLabel->setStyleSheet(m_stylesheet_QLabel14);
    copyrightLabel->setText(QString("当前版本 V%1").arg(QCoreApplication::applicationVersion()));



    QLabel *thridLabel = new QLabel(softwareWidget);
    thridLabel->setStyleSheet(m_stylesheet_QLabel14);
    thridLabel->setText(tr("特别感谢 <a style='text-decoration:none;color:rgb(43,113,237);' href='https://www.chromium.org'>Chromium</a>, <a style='text-decoration:none;color:rgb(43,113,237);' href='https://www.qt.io'>Qt</a>"));
    thridLabel->setOpenExternalLinks(true);
    connect(thridLabel,&QLabel::linkActivated,this,[](const QString &link){
       QDesktopServices::openUrl(QUrl(link));
    });

    QLabel *authorLabel = new QLabel(softwareWidget);
    authorLabel->setStyleSheet(m_stylesheet_QLabel14);
    authorLabel->setText(tr("作者 <a style='text-decoration:none;color:rgb(43,113,237);' href='https://space.bilibili.com/487906612'>北小菜</a>"));
    authorLabel->setOpenExternalLinks(true);
    connect(authorLabel,&QLabel::linkActivated,this,[](const QString &link){
       QDesktopServices::openUrl(QUrl(link));
    });



    // 开源地址 start
    QWidget *osWidget = new QWidget(softwareWidget);// 开源地址
    osWidget->setFixedHeight(20);
    QHBoxLayout *osHLayout = new QHBoxLayout(osWidget);
    osHLayout->setContentsMargins(0,0,0,0);

    QLabel *osLabel = new QLabel(osWidget);
    osLabel->setStyleSheet(m_stylesheet_QLabel14);
    osLabel->setText("开源地址");
    QToolButton *osAddressBtn = new QToolButton(osWidget);
    osAddressBtn->setCursor(Qt::PointingHandCursor);
    osAddressBtn->setStyleSheet("QToolButton{border:0px;font-family:Microsoft YaHei;font-size:13px;color:rgb(43,113,237)}");
    osAddressBtn->setText(URL_OPENSOURCE);
    connect(osAddressBtn,&QToolButton::clicked,this,[](){
        QDesktopServices::openUrl(QUrl(URL_OPENSOURCE));
    });

    osHLayout->addWidget(osLabel);
    osHLayout->addSpacing(6);
    osHLayout->addWidget(osAddressBtn);
    osHLayout->addStretch(10);
    // 开源地址 end

    softwareLayout->addStretch(1);
    softwareLayout->addWidget(nameLabel);
        softwareLayout->addSpacing(5);
    softwareLayout->addWidget(copyrightLabel);
    softwareLayout->addWidget(thridLabel);
    softwareLayout->addWidget(authorLabel);
    softwareLayout->addSpacing(10);
    softwareLayout->addWidget(new ComLineWidget(this));
    softwareLayout->addWidget(osWidget);
    softwareLayout->addStretch(1);
    outLayout->addWidget(softwareWidget);
    boxLayout->addWidget(outWidget,0,Qt::AlignHCenter);

}
