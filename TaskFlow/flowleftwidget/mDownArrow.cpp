#include "mDownArrow.h"
#include <QPainter>


MDownArrow::MDownArrow(QWidget *parent) : QWidget(parent){
      setFixedSize(50,20);
}
void MDownArrow::setIconNormal(){
    iconType = 1;
    this->update();
}
void MDownArrow::setIconPress(){

    iconType=2;
    this->update();
}


void MDownArrow::paintEvent(QPaintEvent *event){

//    QPainter painter(this);
//    if(iconType==1){
//        QIcon icon(":/images/icon/flow/arrow-down.png");
//        QPixmap pix = icon.pixmap(16,22);
//        painter.drawPixmap(QRect(17,0,16,22),pix);
//    }else {
//        QIcon icon(":/images/icon/flow/arrow-circle.png");
//        QPixmap pix = icon.pixmap(28,22);
//        painter.drawPixmap(QRect(11,0,28,22),pix);
//    }


//    QPainter painter(this);

//    QPen pen;
//    pen.setWidth(2);
//    pen.setJoinStyle(Qt::MiterJoin);//让箭头变尖

//    painter.setPen(pen);
//    painter.drawLine(QPoint(width()/2,0),QPoint(width()/2,height()));

    if(iconType==1){
        QPoint sourcePoint = QPoint(width()/2,0);
        QPoint destPoint = QPoint(width()/2,height());
        int arrowSize = 5;
        QPainter painter(this);
//       static const double Pi = 3.14159265358979323846264338327950288419717;
       static const double Pi = 3.14;

       static double TwoPi = 2.0 * Pi;

       QLineF line(sourcePoint,destPoint);
       if (qFuzzyCompare(line.length(), qreal(0.)))
           return;
       QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
       pen.setWidth(1);
       pen.setColor(QColor(Qt::GlobalColor::black));
       painter.setPen(pen);

       painter.drawLine(line);

       double angle = ::acos(line.dx() / line.length());
       if (line.dy() >= 0)
           angle = TwoPi - angle;
       QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                                 cos(angle - Pi / 3) * arrowSize);
       QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                                 cos(angle - Pi + Pi / 3) * arrowSize);
       painter.drawLine(QLineF(destArrowP1,destPoint));
       painter.drawLine(QLineF(destArrowP2,destPoint));
       QWidget::paintEvent(event);
    }else {
        QPainter painter(this);

        QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen.setWidth(1);
        pen.setColor(QColor(Qt::GlobalColor::blue));
        painter.setPen(pen);
        int radius = 7; // 带有加号的圆圈半径

        QPoint hsourcePoint = QPoint(width()/2,height()/2-radius);
        QPoint hdestPoint = QPoint(width()/2,height()/2+radius);

        QPoint vsourcePoint = QPoint(width()/2-radius,height()/2);
        QPoint vdestPoint = QPoint(width()/2+radius,height()/2);

        painter.drawLine(hsourcePoint,hdestPoint);
        painter.drawLine(vsourcePoint,vdestPoint);

        //画圆
        painter.drawEllipse(QPoint(width()/2,height()/2),radius,radius);

    }

}



