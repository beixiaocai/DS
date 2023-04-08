#include "RunMessage.h"
#include "RunDialog.h"
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "style.h"
#include "Utils/models.h"
#include <QCloseEvent>

RunMessage::RunMessage(QWidget *parent) : QDialog(parent)
{
    MTask *task = ((RunDialog *)parent)->getTask();

    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint);
    setFixedSize(600,240);

    setWindowTitle("提示");
    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);
    QWidget *topWidget = new QWidget(this);
    topWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);}");
    QHBoxLayout *topHLayout = new QHBoxLayout(topWidget);
    topHLayout->setContentsMargins(0,0,0,0);
    topHLayout->setSpacing(0);



    //左侧widget
    QWidget *leftWidget = new QWidget(topWidget);
    leftWidget->setFixedWidth(110);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0,20,0,0);
    iconLabel = new QLabel(leftWidget);

    iconLabel->setPixmap(QIcon(":/res/images/icon/success.png").pixmap(40,40));
    leftLayout->addWidget(iconLabel,0,Qt::AlignHCenter);
    leftLayout->addStretch(10);

    //右侧widget
    QWidget *rightWidget = new QWidget(topWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0,20,0,0);
    rightLayout->setSpacing(20);


    titleDesc = new QLabel(rightWidget);
    titleDesc->setText("任务状态");
    titleDesc->setStyleSheet(m_stylesheet_QLabel14);

    QLabel *nameDesc = new QLabel(rightWidget);
    nameDesc->setText(tr(QString("<span style='color:rgb(153,153,153)'>任务</span>   <span style='font-size:14px;font-family:Microsoft YaHei;'>%1</span>").arg(task->name).toUtf8()));

    summaryDesc = new QLabel(rightWidget);
    summaryDesc->setStyleSheet(m_stylesheet_QLabel);

    rightLayout->addWidget(titleDesc);
    rightLayout->addWidget(nameDesc);
    rightLayout->addWidget(summaryDesc);
    rightLayout->addStretch(10);

    topHLayout->addWidget(leftWidget);
    topHLayout->addWidget(rightWidget);

    boxLayout->addSpacing(2);
    boxLayout->addWidget(topWidget);


    initBottomUi();

    this->hide();

}
void RunMessage::showMessage(MessageType type, const QString &summary){

    QString filename;
    QString title;
    if(MessageType::SUCCESS==type){
        filename = ":/res/images/icon/success.png";
        title = "采集已完成";
    }else if (MessageType::ERROR==type) {
        filename = ":/res/images/icon/error.png";
        title = "采集已停止";
    }else{
        filename = ":/res/images/icon/warning.png";
        title = "采集已停止";
    }
    iconLabel->setPixmap(QIcon(filename).pixmap(50,50));
    titleDesc->setText(title);
    summaryDesc->setText(summary);
    this->show();
}

void RunMessage::initBottomUi(){
    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(10,0,10,0);

    QPushButton *okBtn = new QPushButton(bottomWidget);
    okBtn->setStyleSheet(m_stylesheet_QPushButton);
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setText("确定");
    okBtn->setFixedSize(90,28);
    connect(okBtn,&QPushButton::clicked,this,[this](){
        this->close();
    });


    bottomLayout->addStretch(10);
    bottomLayout->addWidget(okBtn,0,Qt::AlignRight);

    boxLayout->addWidget(bottomWidget);

}
void RunMessage::closeEvent(QCloseEvent *event){
    event->accept();
    emit this->closeMessage();

}
