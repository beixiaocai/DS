#ifndef TASK_H
#define TASK_H

#include "QWidget"
QT_BEGIN_NAMESPACE;
class QStackedWidget;
class QComboBox;
QT_END_NAMESPACE;

class TaskDo;
class TaskSettings;
struct MTask;
struct MTaskGroup;

class Task : public QWidget
{
    Q_OBJECT
public:
    explicit Task(MTask *task,QWidget *parent);
    ~Task();

private:
    MTask *m_task;
    QStackedWidget *stackedWidget;

    void initAddView();
    QWidget *addView;
    QComboBox *groupComboBox;
    QVector<MTaskGroup> m_taskGroups;

    int m_currentGroupId = 0;
    void initAddViewDo();
    TaskDo *mTaskDo;
    void initSettings();
    TaskSettings *mSettings;
signals:
    void notifyChangeTabName(const QString &name);
    void notifyChangeTabIcon(const QIcon &icon);

};

#endif // TASK_H
