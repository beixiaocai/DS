#include "ComOptionsBox.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSpinBox>

ComOptionsBox::ComOptionsBox(QWidget *parent,const QString &name) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".ComOptionsBox{ background-color:rgb(241,244,251);}");

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(3,0,3,2);

    // 菜单
    QWidget *menuWidget = new QWidget(this);
    QHBoxLayout *menuLayout = new QHBoxLayout(menuWidget);
    menuLayout->setContentsMargins(5,0,5,0);
    menuWidget->setFixedHeight(30);
    menuWidget->setStyleSheet(".QWidget{background-color: rgba(0,0,0,0);}");

    QToolButton *icon = new QToolButton(menuWidget);
    icon->setAutoRaise(true);
    icon->setArrowType(Qt::ArrowType::DownArrow);
    QLabel *titleLabel = new QLabel(menuWidget);
    titleLabel->setStyleSheet(".QLabel{background-color: rgba(0,0,0,0);color:rgb(0,0,0);}");
    titleLabel->setText(tr(name.toStdString().data()));
    menuLayout->addWidget(icon);
    menuLayout->addWidget(titleLabel);



    //内容
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setStyleSheet(".QWidget{background-color: rgb(255,255,255);}");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0,0,0,0);

    boxLayout->addWidget(menuWidget);
    boxLayout->addWidget(contentWidget);

    connect(icon,&QToolButton::clicked,this,[icon,contentWidget](){
        if(icon->arrowType()==Qt::ArrowType::DownArrow){
            contentWidget->hide();
            icon->setArrowType(Qt::ArrowType::RightArrow);
        }else if (icon->arrowType()==Qt::ArrowType::RightArrow) {
            contentWidget->show();
            icon->setArrowType(Qt::ArrowType::DownArrow);
        }
    });

    gWidget = new QWidget(contentWidget);
    gLayout = new QGridLayout(gWidget);
    gLayout->setSpacing(18);
    gLayout->setColumnMinimumWidth(0,60);
    gLayout->setColumnStretch(2,10);


    contentLayout->addWidget(gWidget);

}
