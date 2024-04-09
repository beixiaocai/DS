#ifndef TASKDATAEXPORDIALOG_H
#define TASKDATAEXPORDIALOG_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QCheckBox;
class QSpinBox;
class QLabel;
QT_END_NAMESPACE;
class ComLoadingLabel;

class TaskDataExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TaskDataExportDialog(QWidget *parent,const QString &taskName,const QString &taskCode);
    ~TaskDataExportDialog();

private:
    QString mTaskName;
    QString mTaskCode;
    QVBoxLayout *boxLayout;
    QCheckBox *xlsxCb;
    QCheckBox *csvCb;

    QLabel    *remarkLabel;
    QSpinBox  *remarkSpin;
    QString mSaveDir;//保存文件路径

    QLabel          *loadingMsgLabel;
    ComLoadingLabel *loadingLabel;

private:
    void exportXlsx();
    void exportCsv();

signals:

};

#endif // TASKDATAEXPORDIALOG_H
