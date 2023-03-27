#ifndef M_FLOW_RIGHT_EXTRACT_WIDGET_H
#define M_FLOW_RIGHT_EXTRACT_WIDGET_H

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLineEdit;
class QSpinBox;
class QCheckBox;
class QTableWidget;
class QToolButton;
QT_END_NAMESPACE;
class MFlowStepData;
class MFlowStepParamsExtract;
class MOptionBox;
struct MParamsField;
struct MTask;

class MFieldMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MFieldMenu(QWidget *parent);
    ~MFieldMenu();

    QToolButton *editBtn;
    QToolButton *delBtn;
signals:
};

class MFieldEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MFieldEditDialog(MParamsField * field,QWidget *parent);
signals:
};
class MFlowRightExtractWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFlowRightExtractWidget(QWidget *parent,MFlowStepData* data,MTask *task);
    ~MFlowRightExtractWidget();

private:
    QVBoxLayout *boxLayout;
    MFlowStepData* m_data;
    MFlowStepParamsExtract *m_params;
    MTask *m_task;
    void initInfoUI();
    QLineEdit *nameLine;
    void initFieldsUI();
    QTableWidget *tableWidget;
    void initSenierOptionUI();
    MOptionBox *optionBox;
    QSpinBox *waitSpin;
    QCheckBox *loopCheck;// 使用循环

    void addField(MParamsField * field);
public slots:
    void onSave(bool clicked);

};


#endif // M_FLOW_RIGHT_EXTRACT_WIDGET_H
