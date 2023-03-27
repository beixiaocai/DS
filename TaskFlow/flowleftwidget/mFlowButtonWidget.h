#ifndef M_FLOW_BUTTON_WIDGET_H
#define M_FLOW_BUTTON_WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE;
class QLabel;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParams;
class MDownArrow;
class MControl;

class MFlowButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MFlowButtonWidget(QWidget *parent,MControl *control,MDownArrow *arrow,MFlowStepParams *params=nullptr,bool bgnormal=false);
    ~MFlowButtonWidget();
    MDownArrow * arrow;//附属的箭头，位于widget之上，方向朝下
    QLabel *title;
    MFlowStepData* data;

    QWidget *currentParentWidget;//当前窗体在历经拖拽等一系列动作后，最新的父窗体

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
 void notificationToParentWidget(MFlowStepData* data); // 与PageCustomTaskFlowControl 的onNotificationFromChildWidget 形成信号发送接收关系


public slots:
};

#endif // M_FLOW_BUTTON_WIDGET_H
