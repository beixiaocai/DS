#ifndef TASKDATAEXPOR_H
#define TASKDATAEXPOR_H

#include <QDialog>

class ComLoadingLabel;

class TaskDataExport : public QDialog
{
    Q_OBJECT
public:
    explicit TaskDataExport(QWidget *parent,const QString &taskName,const QString &taskCode);
    ~TaskDataExport();

private:
    QString mTaskName;
    QString mTaskCode;
    ComLoadingLabel *loadingLabel;
    void exportXlsx(int sheetSize);

signals:

};

#endif // TASKDATAEXPOR_H
