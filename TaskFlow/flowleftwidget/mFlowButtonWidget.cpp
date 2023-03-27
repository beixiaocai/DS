#include "mFlowButtonWidget.h"
#include "mFlowStepMenuDialog.h"
#include "TaskFlow/mFcModels.h"
#include "mDownArrow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QDebug>


MFlowButtonWidget::MFlowButtonWidget(QWidget *parent,MControl *control,MDownArrow *arrow,MFlowStepParams *params,bool bgnormal) : QWidget(parent)
{
    setFixedSize(110,30);
    setAttribute(Qt::WA_StyledBackground,true);
    if(bgnormal){
       setStyleSheet(m_stylesheet_QWidget_flowleftwidget_blue_normal);
    }else {
       setStyleSheet(m_stylesheet_QWidget_flowleftwidget_blue_press);
    }
    setCursor(Qt::PointingHandCursor);

    this->arrow = arrow;
    this->data = new MFlowStepData(control,params);
    this->data->widget=this;

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);

    title = new QLabel(this);
    title->setStyleSheet(m_stylesheet_QLabel_title);
    title->setText(control->menuTip);
    boxLayout->addWidget(title,0,Qt::AlignCenter);


}
MFlowButtonWidget::~MFlowButtonWidget()
{
    data->widgetIsDelete = true;
    delete data;

}

void MFlowButtonWidget::mousePressEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton){
        //左键
        this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_START;
        emit this->notificationToParentWidget(this->data);

        setStyleSheet(m_stylesheet_QWidget_flowleftwidget_blue_press);

        QPoint offset = event->pos();
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << offset;
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/MFlowButtonWidget", itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
//        drag->setPixmap(QPixmap::grabWidget(this));
        drag->setHotSpot(offset);

         if (drag->exec(Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
              this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;
              emit this->notificationToParentWidget(this->data);
        } else {
             this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_OTHER_END_SHOWCONTENT;
             emit this->notificationToParentWidget(this->data);
         }


    }else {
        MFlowStepMenuDialog *menu = new MFlowStepMenuDialog(this);
        connect(menu->deleteBtn,&QPushButton::clicked,[this,menu](){

            this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_DELETE_CURRENT_WIDGET;
            emit this->notificationToParentWidget(this->data);

            menu->deleteLater();
        });
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->move(QCursor::pos());
        menu->show();


    }

}
