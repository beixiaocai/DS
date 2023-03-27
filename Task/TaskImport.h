#ifndef TASKIMPOR_H
#define TASKIMPOR_H

#include <QDialog>
struct MTask;

class TaskImport : public QDialog
{
    Q_OBJECT
public:
    explicit TaskImport(QWidget *parent);
    ~TaskImport();
private:
    int m_currentGroupId = 0; //0：获取所有分组的任务
    QVector<MTask*> m_importSuccessTasks;
signals:

public slots:


};

#endif // TASKIMPOR_H
