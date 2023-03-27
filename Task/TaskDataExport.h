#ifndef TASKDATAEXPOR_H
#define TASKDATAEXPOR_H

#include <QDialog>

class ComLoadingLabel;

class TaskDataExport : public QDialog
{
    Q_OBJECT
public:
    explicit TaskDataExport(const QString &taskName,const QString &taskCode,QWidget *parent);
    ~TaskDataExport();

private:
    ComLoadingLabel *loadingLabel;
    QString m_taskName;
    QString m_taskCode;
    void exportXlsx(int sheetSize);

signals:

};

#endif // TASKDATAEXPOR_H
