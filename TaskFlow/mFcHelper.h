#ifndef MFCHELPER_H
#define MFCHELPER_H
#include <QObject>
#include <QHash>
QT_BEGIN_NAMESPACE;
class QWidget;
class QVBoxLayout;
QT_END_NAMESPACE;

class MDownArrow;
class MFlowStepData;
class MFlowStepParams;
class PageCustomTaskFlowControl;


class MFcHelper : public QObject{
public:
    explicit MFcHelper(PageCustomTaskFlowControl *parent);
    ~MFcHelper();

    QVector<MDownArrow *> arrows;// 所有箭头窗体的集合
    void setarrowsPress(); // 所有箭头窗体按下状态
    void setarrowsNormal();// 所有箭头窗体正常状态

    QVector<QWidget *> allstepwidgets;  // 所有步骤窗体的集合
    void updateBackground(); // 更新所有步骤窗体的背景颜色
    void removeStep(QWidget *stepwidget); // 流程控制上删除步骤widget
    void updateFixedSize();             // 更新所有窗体的布局

    void setCurrentStepData(MFlowStepData *data);
    MFlowStepData * getCurrentStepData();

    void serialize();
    void deserialize();

private:
    void addButtonWidget(MFlowStepParams *params,bool bgnormal);
    void addIfWidget(MFlowStepParams *params,bool bgnormal);
    void addLoopWidget(MFlowStepParams *params,bool bgnormal);

    struct Location{
        QWidget     *widget; // 下一步窗体的父窗体
        QVBoxLayout *layout; // 下一步窗体的父窗体layout
        int target=0;        // 下一步窗体应该插入父窗体layout的目标target
    };

    Location getLocation(MFlowStepParams *params);
private:

    MFlowStepData  *m_currentData = nullptr; //当前按下的控件的参数data
    PageCustomTaskFlowControl *m_fc;

    // 删除窗体相关属性
    QVector<QWidget *> m_removeWidgets; // 临时存放需要删除的widgets
    void updateDelWidgets(QWidget *widget); // 递归获取所有需要删除的widgets

    // 更新尺寸相关属性
    QMultiHash<int,QWidget *> m_tempLoopTypeWidgets;  //临时存放循环类型的widget
    int                       m_tempLoopTypeWidgetMaxDeep = 0;// 临时存放的循环类型widget最大深度
    void getLoopTypeWidgets(int deep=0,QWidget *widget=nullptr);// 递归获取所有循环类型的widget



};

#endif // MFCHELPER_H
