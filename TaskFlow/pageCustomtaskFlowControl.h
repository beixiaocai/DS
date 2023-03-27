#ifndef PAGE_CUSTOMTASK_FLOW_CONTROL_H
#define PAGE_CUSTOMTASK_FLOW_CONTROL_H
#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE;
class QVBoxLayout;
QT_END_NAMESPACE;
class MControl;
class MFcHelper;
class MFlowStepData;
struct MParamsField;
struct MTask;


class PageCustomTaskFlowControl : public QWidget
{
    Q_OBJECT
public:
    explicit PageCustomTaskFlowControl(MTask *task,QWidget *parent);
    ~PageCustomTaskFlowControl();

    MTask *task;

    QVBoxLayout *boxLayout;

    // 支持窗体进入判断公共方法，所有支持拖拽进入的窗体的判断方法
    void dragEnterEvery(QDragEnterEvent *event);
    // 支持窗体拖拽进入的公共方法，所有支持窗体接收的执行方法
    void dropEvery(QWidget *parentWindow,QVBoxLayout *boxLayout, QDropEvent *event);


    void setFlowAddressList(const QStringList &addressList,bool isFirst);

    void addFlowOpenWebStep(const QString &url,bool isLoop=false);

    void addFlowClickStep(const QString &ruleUrl,const QString &nodeXpath,const QString &nodeText,const QString &name,bool isLoop,bool isNewTab,bool isRoll,bool isTurnPage=false);

    void addFlowExtractStep(MParamsField *field,bool isLoop=false);

    void addFlowInputStep(const QString &nodeXpath,const QString &nodeText,const QString text,bool isLoop=false);

    void addFlowMouseStep(const QString &nodeXpath,const QString &nodeText,bool isLoop=false);

    void addFlowLoopAndExtractStep(MParamsField *field,
                                   const QString &loopTypeName,
                                   const QString &loopTypeValue,
                                   const QStringList &textList);
    void addFlowLoopAndClickStep(const QString &nodeRuleUrl,
                                 const QString &nodeXpath,
                                 const QString &nodeText,
                                 const QString &loopTypeName,
                                 const QString &loopTypeValue,
                                 const QStringList &textList,
                                 const QString &clickName,
                                 bool isNewTab,bool isTurnPage);
    void addFlowLoopAndRollClickStep();// 快捷添加下拉刷新的流程
    void serialize();
    void deserialize();

    QHash<QString,MControl*> controls;  // 所有控件项

private:
    struct NextStepLocation{

        QWidget     *widget; // 下一步窗体的父窗体
        QVBoxLayout *layout; // 下一步窗体的父窗体layout
        int target=0;        // 下一步窗体应该插入父窗体layout的目标target
        bool noUpdateSize = true;

        QString ruleUrlParent; // 当前步骤父窗体的ruleUrl
        QString ruleUrlParentInnerLastStep; // 当前步骤父窗体内，上一步骤的ruleUrl
        QString ruleUrlNewStep; // 当前步骤后即将加入的步骤的预测ruleUrl（除click步骤外，多使用预测ruleUrl）
    };

    NextStepLocation getNextStepLocation();

    QString getStepURL(QWidget *widget) const;

    MFcHelper *m_fcHelper;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void notificationToSmartAlertWidget(MFlowStepData* data);//通知智能提示框，有控件窗体发生变化
    void notificationToFlow(MFlowStepData* data);// 通知flow，有控件窗体发生变化
public slots:
    void onNotificationFromChildWidget(MFlowStepData* data);// 接收来自拖拽窗体的通知

};

#endif // PAGE_CUSTOMTASK_FLOW_CONTROL_H
