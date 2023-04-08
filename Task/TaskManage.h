#ifndef TASKMANAGE_H
#define TASKMANAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QTableWidget;
class QComboBox;
class QPushButton;
class QMenu;
QT_END_NAMESPACE;
class ComLoadingLabel;
class ComLoadingWidget;
struct MTask;
struct MTaskGroup;
class TaskManage : public QWidget
{
    Q_OBJECT

public:
    explicit TaskManage(QWidget *parent);
    ~TaskManage();
private:
    QVBoxLayout *boxLayout;
    void initTopMenuUi();
    QComboBox *groupComboBox;
    QPushButton *refreshBtn;
    ComLoadingLabel *loadingLabel;
    ComLoadingWidget *loading;
    void initRowMenu();
    QMenu *rowMenu;
    void initTableUi();
    QTableWidget* tableWidget;

private:
    enum GetTaskUseType{
        ExecuteTask,// 执行任务
        EditTask,    // 编辑任务
        ExportTask,  // 导出任务
    };
    GetTaskUseType m_useType;
    QString m_exportTaskDir; // 导出任务到本地的文件夹路径

    QVector<MTaskGroup> m_taskGroups;
    int mCurGroupId = 0; //0：获取所有分组的任务

    QVector<MTask *> mTasks;
    MTask *mCurTask = nullptr;
    void getTasks();
    void showTasks();
    void getTask(const QString &code);
    void onDelTask(bool state,QString &msg);
signals:
    void notifyPageTaskData(const QString &taskName,const QString &taskCode);
    void notifyEditCustomTask(MTask *task);

public slots:


};

#endif // TASKMANAGE_H
