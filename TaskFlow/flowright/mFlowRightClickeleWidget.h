#ifndef M_FLOW_RIGHT_CLICKELE_WIDGET_H
#define M_FLOW_RIGHT_CLICKELE_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsClickEle;
class MRollWidget;
class MOptionBox;
class MFlowRightClickEleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFlowRightClickEleWidget(QWidget *parent,MFlowStepData* data);
    ~MFlowRightClickEleWidget();

private:
    QVBoxLayout *boxLayout;
    MFlowStepData* m_data;
    MFlowStepParamsClickEle *m_params;
    void initInfoUI();
    QLineEdit *nameLine;
    void initSenierOptionUI();
    MOptionBox *optionBox;
    QSpinBox *waitSpin;
    QLineEdit *eleXpathLine;//xpath
    QLineEdit *eleTextLine;

    QCheckBox *loopCheck;// 使用循环
    QCheckBox *isNewTabCheck;//开新标签

//    QCheckBox *isAjaxCheck;//ajax加载
//    QSpinBox *ajaxOvertimeSpin;//ajax超时
//    QCheckBox *isPageSpeedCheck;//页面加速（非ajax页面加速）
//    QCheckBox *isAnchorCheck;//定位锚点
//    QLineEdit *anchorIdLine;//锚点ID
//    QString anchorId;//锚点ID
//    QCheckBox *autoTryCheck;//自动重试

    QCheckBox *rollCheck;// 滚动页面
    MRollWidget * rollWidget;//滚动页面的子选项widget
    void initRetryUI();
public slots:
    void onSave(bool clicked);
};

#endif // M_FLOW_RIGHT_CLICKELE_WIDGET_H
