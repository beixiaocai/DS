#include "VersionDialog.h"
#include "style.h"
#include "Utils/models.h"
#include "Utils/ComLineWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QsLog.h>
VersionDialog::VersionDialog(QWidget *parent,MVersion &version) : QDialog(parent)
{
    QLOG_INFO() << "VersionDialog::VersionDialog()";
    setFixedSize(520,320);

    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".VersionDialog{background-color:rgb(255,255,255);}");
    setWindowTitle("发现新版本");

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);
    boxLayout->setSpacing(0);

    // 更新内容
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20,20,20,0);
    contentLayout->setSpacing(0);

    QLabel *titleLabel = new QLabel(contentWidget);
    titleLabel->setStyleSheet(m_stylesheet_QLabel18);
    titleLabel->setText("更新内容");

    QLabel *versionLabel = new QLabel(contentWidget);
    versionLabel->setStyleSheet(m_stylesheet_QLabel);
    versionLabel->setText(QString("发现新版本 V%1，更新时间 %2").arg(version.version).arg(version.pubdate));


    QLabel *contentLabel = new QLabel(contentWidget);
    contentLabel->setStyleSheet(m_stylesheet_QLabel);
    contentLabel->setWordWrap(true);
    contentLabel->setText(tr(QByteArray(version.updateContent.toUtf8())));

    contentLayout->addWidget(titleLabel);
    contentLayout->addSpacing(10);
    contentLayout->addWidget(new ComLineWidget(contentWidget));
    contentLayout->addSpacing(10);
    contentLayout->addWidget(versionLabel);
    contentLayout->addSpacing(10);
    contentLayout->addWidget(contentLabel);

    // 底部菜单
    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(50);
    QHBoxLayout *bottomHLayout = new QHBoxLayout(bottomWidget);
    bottomHLayout->setContentsMargins(0,0,0,0);
    bottomHLayout->setSpacing(0);



    QPushButton *okBtn = new QPushButton(bottomWidget);
    okBtn->setText("立即升级");
    okBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setFixedSize(90,32);
    connect(okBtn,&QPushButton::clicked,this,[version](){
       QDesktopServices::openUrl(QUrl(version.url));

    });
    QPushButton *cancelBtn = new QPushButton(bottomWidget);
    cancelBtn->setText("暂不升级");
    cancelBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    cancelBtn->setCursor(Qt::PointingHandCursor);
    cancelBtn->setFixedSize(90,32);
    connect(cancelBtn,&QPushButton::clicked,this,[this](){
        this->close();
    });

    bottomHLayout->addStretch(10);
    bottomHLayout->addWidget(okBtn);
    bottomHLayout->addSpacing(20);
    bottomHLayout->addWidget(cancelBtn);
    bottomHLayout->addSpacing(20);

    boxLayout->addWidget(contentWidget);
    boxLayout->addStretch(10);
    boxLayout->addWidget(new ComLineWidget(this));
    boxLayout->addWidget(bottomWidget);

}
