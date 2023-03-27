#ifndef M_FLOW_RIGHT_LOOP_WIDGET_H
#define M_FLOW_RIGHT_LOOP_WIDGET_H
#include <QWidget>
#include <QMap>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
class QListWidget;
class QTextEdit;
class QLabel;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsLoop;
class MOptionBox;

class MFlowRightLoopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MFlowRightLoopWidget(QWidget *parent,MFlowStepData* data);
    ~MFlowRightLoopWidget();
private:
    QMap<QString,QString> m_tmap;
    const QString transferLoopTypeName(const QString name,bool isForward=true);

private:
    MFlowStepData* m_data;
    MFlowStepParamsLoop *m_params;
    QVBoxLayout *boxLayout;

    void initInfoUI();
    QLineEdit *nameLine;
    void initSenierOptionUI();
    QLabel *listWidgetName;
    QListWidget *listWidget;

    void setTextList(const QStringList textList);
    MOptionBox *optionBox;
    QSpinBox *waitSpin;
    QLabel *loopTypeValueLabel;
    QTextEdit *loopTypeValueEdit;

    QCheckBox *endLoopWithTimesCheck;
    QSpinBox *endLoopTimesSpin;



    void initRetryUI();
signals:

public slots:
    void onSave(bool clicked);
};


#endif // M_FLOW_RIGHT_LOOP_WIDGET_H
