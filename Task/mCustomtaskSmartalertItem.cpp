#include "mCustomtaskSmartalertItem.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include "style.h"

MCustomTaskSmartAlertItem::MCustomTaskSmartAlertItem(QWidget *parent,QString name,QString tip) :
    QWidget(parent){

    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(QString(".QWidget{background-color:%1;}").arg(m_rgb_basic));
    this->setFixedHeight(25);
    this->setFixedWidth(parent->width());

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(20,0,20,0);

    // 菜单行
    QWidget *menuWidget = new QWidget(this);
    QHBoxLayout *menuLayout = new QHBoxLayout(menuWidget);
    menuLayout->setContentsMargins(0,0,0,0);

    QStyle* style = QApplication::style();
    m_tipButton = new QToolButton(menuWidget);
    m_tipButton->setStyleSheet("QToolButton{border:none;}");
    m_tipButton->setIcon(style->standardIcon(QStyle::SP_CommandLink));
    m_tipButton->setToolTip(tip);

    m_namePushButton = new QPushButton(menuWidget);
    m_namePushButton->setStyleSheet("QPushButton {color:rgb(2,91,222);border:none;};");
    if("选中全部"==name){
           m_namePushButton->setFont(QFont("宋体",11,QFont::Bold));
    }else {
           m_namePushButton->setFont(QFont("宋体",10,QFont::Normal));
    }
    menuLayout->addWidget(m_namePushButton,0,Qt::AlignLeft);
    menuLayout->addWidget(m_tipButton,0,Qt::AlignLeft);
    m_namePushButton->setCursor(Qt::PointingHandCursor);
    m_namePushButton->setText(name);
    m_namePushButton->setFocusPolicy(Qt::NoFocus);



    // 分割线行

    QWidget *line = new QWidget(this);
    line->setStyleSheet(".QWidget{background-color:rgb(233,233,233);}");
    line->setFixedHeight(1);


    boxLayout->addWidget(menuWidget);
    boxLayout->addWidget(line);

}
