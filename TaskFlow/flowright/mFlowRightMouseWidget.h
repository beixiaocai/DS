#ifndef M_FLOW_RIGHT_MOUSE_WIDGET_H
#define M_FLOW_RIGHT_MOUSE_WIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsMouse;
class MOptionBox;

class MFlowRightMouseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFlowRightMouseWidget(QWidget *parent,MFlowStepData* data);
    ~MFlowRightMouseWidget();
private:
    QVBoxLayout *boxLayout;
    MFlowStepData* m_data;
    MFlowStepParamsMouse *m_params;
    void initInfoUI();
    QLineEdit *nameLine;
    QLineEdit *eleTextLine;
    void initSenierOptionUI();
    MOptionBox *optionBox;
    QSpinBox *waitSpin;
    QLineEdit *eleXpathLine;//xpath
    QCheckBox *loopCheck;// 使用循环

    void initRetryUI();// 重试
public slots:
    void onSave(bool clicked);
};


#endif // M_FLOW_RIGHT_MOUSE_WIDGET_H
