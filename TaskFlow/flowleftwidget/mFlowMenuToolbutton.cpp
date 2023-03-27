#include "mFlowMenuToolbutton.h"
#include "TaskFlow/mFcModels.h"
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
MFlowMenuToolButton::MFlowMenuToolButton(QWidget *parent,MControl * control) : QToolButton(parent)
{
    this->data = new MFlowStepData(control,nullptr);
    this->data->widget = this;
    setStyleSheet(".MFlowMenuToolButton{border:0px;}");
    setCursor(Qt::PointingHandCursor);// 手指
    setToolTip(control->menuTip);

    QSize s(36,30);
    setFixedSize(s);
    setIconSize(s);
    setIcon(QIcon(control->menuIcon));
}

void MFlowMenuToolButton::mousePressEvent(QMouseEvent *event){

    this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_START;
    emit this->notificationToParentWidget(this->data);
    QPoint offset = event->pos();
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << offset;
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("application/MFlowMenuToolButton", itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(this->icon().pixmap(30,30));
//      drag->setHotSpot(offset);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
       this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT;
       emit this->notificationToParentWidget(this->data);
    } else {
       this->data->notificationType=MCONNSTANT_NOTIFICATION_TYPE_OTHER_END;
       emit this->notificationToParentWidget(this->data);
    }

}
MFlowMenuToolButton::~MFlowMenuToolButton(){

    data->widgetIsDelete = true;
    delete data;

}
