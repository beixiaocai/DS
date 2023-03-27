#ifndef MFLOWMENUTOOLBUTTON_H
#define MFLOWMENUTOOLBUTTON_H

#include <QToolButton>
class MFlowStepData;
class MControl;
class PageCustomTaskFlowControl;


class MFlowMenuToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit MFlowMenuToolButton(QWidget *parent,MControl *control);
    ~MFlowMenuToolButton();
    MFlowStepData* data;
protected:
    void virtual mousePressEvent(QMouseEvent *event) override;

signals:
 void notificationToParentWidget(MFlowStepData* data); // 与PageCustomTaskFlowControl 的onNotificationFromChildWidget(int) 形成信号发送接收关系


public slots:
};

#endif // MFLOWMENUTOOLBUTTON_H
