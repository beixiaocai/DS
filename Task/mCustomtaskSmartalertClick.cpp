#include "mCustomtaskSmartalertClick.h"
#include "style.h"
#include "mCustomtaskSmartalertItem.h"
#include <QVBoxLayout>
#include <QPushButton>


MCustomTaskSmartAlertClick::MCustomTaskSmartAlertClick(QWidget *parent,const QStringList nameItems) : QWidget(parent)
{

    setFixedWidth(300);
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,10,0,0);


    // 首次点击页面上的元素的交互页面结构
    MCustomTaskSmartAlertItem *item;
    QString currentName;
    for (int i = 0; i < nameItems.length(); ++i) {
        currentName = nameItems[i];

        item= new MCustomTaskSmartAlertItem(this,currentName,currentName);

        connect(item->m_namePushButton,&QPushButton::clicked,[this,currentName](){
            emit this->clickedItem(currentName);
        });


        boxLayout->addWidget(item,0,Qt::AlignHCenter);
    }
    cancelButton = new QPushButton(this);
    cancelButton->setStyleSheet(m_stylesheet_QPushButton_hollow);
    cancelButton->setText(tr("取消选择"));
    cancelButton->setFixedSize(90,24);
    cancelButton->setCursor(Qt::PointingHandCursor);

    boxLayout->addWidget(cancelButton,0,Qt::AlignHCenter);
    boxLayout->addStretch(10);



}
