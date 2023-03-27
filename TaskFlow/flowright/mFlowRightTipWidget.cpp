#include "mFlowRightTipWidget.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MFlowRightTipWidget::MFlowRightTipWidget(const QString name,const QString tip,QWidget *parent) : QWidget(parent)
{

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(10,20,10,5);
    boxLayout->setSpacing(20);

    // 第一行

    QLabel *nameLabel = new QLabel(this);
    nameLabel->setStyleSheet(m_stylesheet_QLabel);
    nameLabel->setText(name);
    boxLayout->addWidget(nameLabel,0,Qt::AlignHCenter);

    // 第二行
    QWidget *tipWidget = new QWidget(this);
    tipWidget->setObjectName("tipWidget");
    tipWidget->setStyleSheet("QWidget#tipWidget{border-style: solid;border-width: 1px; border-radius: 5px;border-color:rgb(43,113,237);padding: 1px;}");
    tipWidget->setFixedHeight(80);
    QHBoxLayout *tipLayout = new QHBoxLayout(tipWidget);
    tipLayout->setContentsMargins(30,0,30,0);

    QLabel *tipLabel = new QLabel(tipWidget);
    tipLabel->setText(tip);

    tipLayout->addWidget(tipLabel,0,Qt::AlignHCenter);

    boxLayout->addWidget(tipWidget,0,Qt::AlignHCenter);

    boxLayout->addStretch(10);


}
