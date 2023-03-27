#include "ComSplitHWidget.h"

ComSplitHWidget::ComSplitHWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".ComSplitHWidget{background-color:rgb(221,222,225);}");

    setCursor(Qt::SplitHCursor);
}
void ComSplitHWidget::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton){
        this->raise();//将此按钮移动到顶层显示
        this->pressPoint =event->pos();
    }

}

void ComSplitHWidget::mouseMoveEvent(QMouseEvent *event){

    if(event->buttons() == Qt::LeftButton){
        QPoint change= event->pos() - this->pressPoint;
        int distance = change.x();
        emit moveDistance(distance);

    }
}
