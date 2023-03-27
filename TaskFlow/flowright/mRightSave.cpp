#include "mRightSave.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include "style.h"

MRightSave::MRightSave(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout *boxHLayout = new QHBoxLayout(this);
    boxHLayout->setContentsMargins(0,0,0,0);
    QPushButton *saveBtn = new QPushButton(this);
    saveBtn->setText("保存属性");
    saveBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
    saveBtn->setFixedSize(80,28);
    saveBtn->setCursor(Qt::PointingHandCursor);
    connect(saveBtn,&QPushButton::clicked,this,[this,saveBtn](bool clicked){
        emit this->notifySave(clicked);
        saveBtn->setStyleSheet(".QPushButton {color:white;font-family:SimSun;font-size:13px;background-color:rgb(208,208,208);border-style: solid;border-width: 0px; border-radius: 3px;padding: 2px;}");

        saveBtn->setText("保存中");
        QTimer::singleShot(1000,this,[saveBtn](){
            saveBtn->setText("保存属性");
            saveBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
        });


    });
    boxHLayout->addWidget(saveBtn);
    boxHLayout->addStretch(10);

}
