#ifndef PAGE_CUSTOMTASK_FLOW_H
#define PAGE_CUSTOMTASK_FLOW_H
#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
QT_END_NAMESPACE;
class MFlowStepData;
class PageCustomTaskFlowControl;
struct MTask;

class PageCustomTaskFlow : public QWidget
{
    Q_OBJECT

public:
    explicit PageCustomTaskFlow(MTask *task,QWidget *parent);
    ~PageCustomTaskFlow();
    PageCustomTaskFlowControl *fc;

private:
    MTask *m_task;

    void initMenuUi();
    QWidget *menuWidget;
    void initLeftUi();
    QWidget *leftWidget;
    void initRightUi();
    QWidget *rightWidget;
    QVBoxLayout *rightScrollWidgetLayout;
    float m_leftratio = 0.45;

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
    void onNotificationFromFlowControl(MFlowStepData* data);// 接收来自flow_control的通知

};

#endif // PAGE_CUSTOMTASK_FLOW_H
