#ifndef M_FLOW_RIGHT_INPUT_WIDGET_H
#define M_FLOW_RIGHT_INPUT_WIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsInput;
class MOptionBox;
class MFlowRightInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFlowRightInputWidget(QWidget *parent,MFlowStepData* data);
    ~MFlowRightInputWidget();
private:
    QVBoxLayout *boxLayout;
    MFlowStepData* m_data;
    MFlowStepParamsInput *m_params;
    void initInfoUI();
    QLineEdit *nameLine;
    void initSenierOptionUI();
    MOptionBox *optionBox;
    QSpinBox *waitSpin;
    QCheckBox *loopCheck;// 使用循环
    QLineEdit *eleXpathLine;//xpath
    QLineEdit *eleTextLine;
    QLineEdit *inputTextLine;//输入文本
    void initRetryUI();// 重试
public slots:
    void onSave(bool clicked);
};

#endif // M_FLOW_RIGHT_INPUT_WIDGET_H
