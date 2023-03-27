#include "mCustomtaskSmartalertSelectall.h"
#include "style.h"
#include "mCustomtaskSmartalertItem.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>


MCustomTaskSmartAlertSelectAll::MCustomTaskSmartAlertSelectAll(QWidget *parent,const QStringList nameItems) : QWidget(parent)
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



    listWidget = new QListWidget(this);
    listWidget->setFixedHeight(90);

//   listWidget->setStyleSheet("QListWidget{border:1px solid gray; color:black; }"
//    "QListWidget::Item{padding-top:20px; padding-bottom:4px; }"
//    "QListWidget::Item:hover{background:skyblue; }"
//    "QListWidget::item:selected{background:lightgray; color:red; }"
//    "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
//    );

    boxLayout->addWidget(listWidget,0,Qt::AlignHCenter);

    boxLayout->addStretch(10);
}
void MCustomTaskSmartAlertSelectAll::setFieldNameList(const QStringList nameList){

    // step1 清理原有字段
//    QVector<QListWidgetItem *>items;
//    for (int i = 0; i < listWidget->count(); ++i) {
//        QListWidgetItem *item = listWidget->item(i);
//        items.append(item);
//    }
//    for (int i = 0; i < items.count(); ++i) {
//        delete items[i];
//    }
    listWidget->clear();
    // step2 设置新字段
    QSize s(220,26);
    QListWidgetItem *item;
    for (int i = 0; i < nameList.length(); ++i) {
        item = new QListWidgetItem(nameList[i]);
        item->setSizeHint(s);
        listWidget->addItem(item);
    }

}

