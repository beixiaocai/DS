#ifndef M_FLOW_RIGHT_OPENWEB_WIDGET_H
#define M_FLOW_RIGHT_OPENWEB_WIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsOpenWeb;
class MRollWidget;
class MOptionBox;

class MFlowRightOpenWebWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MFlowRightOpenWebWidget(QWidget *parent ,MFlowStepData* data);
    ~MFlowRightOpenWebWidget();
private:
    QVBoxLayout *boxLayout;
    MFlowStepData* m_data;
    MFlowStepParamsOpenWeb *m_params;

    void initInfoUI();
    QLineEdit *nameLine;
    QSpinBox *overtimeSpin;
    QLineEdit *siteLine;

    void initSenierOptionUI();
    MOptionBox *optionBox;
    QSpinBox *waitSpin;// 执行前等待时间
//    QCheckBox *autoTryCheck;
    QCheckBox *customCookieCheck;
    QLineEdit *cookieLine;
    QCheckBox *loopCheck;// 使用循环
    QCheckBox *preventCheck;//是否阻止弹出窗口
    QCheckBox *clearCacheCheck;
    QCheckBox *rollCheck;
    MRollWidget * rollWidget;

    void initRetryUI();// 重试

public slots:
    void onSave(bool clicked);
};

#endif // M_FLOW_RIGHT_OPENWEB_WIDGET_H
