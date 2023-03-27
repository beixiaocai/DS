#ifndef M_FLOW_RIGHT_SWITCH_WIDGET_H
#define M_FLOW_RIGHT_SWITCH_WIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsSwitch;
class MOptionBox;
class MFlowRightSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFlowRightSwitchWidget(QWidget *parent,MFlowStepData* data);
    ~MFlowRightSwitchWidget();
private:
    QVBoxLayout *boxLayout;
    MFlowStepData* m_data;
    MFlowStepParamsSwitch *m_params;
    void initInfoUI();
    QLineEdit *nameLine;
    void initSenierOptionUI();
    MOptionBox *optionBox;
    QSpinBox *waitSpin;
    QCheckBox *loopCheck;// 使用循环
    QLineEdit *eleXpathLine;//xpath
    QLineEdit *eleTextLine;
    void initRetryUI();// 重试
public slots:
    void onSave(bool clicked);
};

#endif // M_FLOW_RIGHT_SWITCH_WIDGET_H
