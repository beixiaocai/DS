#include "mCustomtaskSmartalertSuccess.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>


MCustomTaskSmartAlertSuccess::MCustomTaskSmartAlertSuccess(QWidget *parent) : QWidget(parent)
{

    setFixedWidth(300);
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,10,0,0);

    //已成功设置采集字段的页面结构
    titleLabel = new QLabel(this);
    boxLayout->addWidget(titleLabel,0,Qt::AlignHCenter);
    boxLayout->addSpacing(5);

    listWidget = new QListWidget(this);
    listWidget->setMaximumHeight(260);

    boxLayout->addWidget(listWidget,0,Qt::AlignHCenter);

}
void MCustomTaskSmartAlertSuccess::showMsg(const QString titleText, bool isShowList){
    titleLabel->setText(titleText);
    if(isShowList){
        listWidget->show();
    }else {
        listWidget->clear();
        listWidget->hide();
    }
}
void MCustomTaskSmartAlertSuccess::showList(const QStringList nameList){

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
    if(nameList.length()>0){
        QSize s(220,26);
        QListWidgetItem *item;
        for (int i = 0; i < nameList.length(); ++i) {
            item = new QListWidgetItem(nameList[i]);
            item->setSizeHint(s);
            listWidget->addItem(item);
        }
    }

}
