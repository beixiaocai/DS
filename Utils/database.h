#ifndef DATABASE_H
#define DATABASE_H

#include <QThread>
#include <QQueue>
#include <QHash>
QT_BEGIN_NAMESPACE;
class QSqlDatabase;
class QMutex;

QT_END_NAMESPACE;

struct MTask;
struct MTaskData;
struct MTaskGroup;
struct MTaskUserAgent;

class Database : public QThread
{
    Q_OBJECT
private:
    explicit Database();
    ~Database();

public:
    static Database *getInstance();

    //任务表
    bool addTask(MTask *task,QString &msg);
    bool delTask(const QString &code,QString &msg);
    bool getTasks(int groupId,QString &msg,QVector<MTask *> &tasks);
    bool getTask(const QString &code,QString &msg,MTask *task);

    //任务数据表
    bool createTaskData(const QString &code,QString &msg,const QStringList &fields);//创建任务数据表
    bool clearTaskData(const QString &code,QString &msg);//清空任务数据表的数据，但不删除数据表
    void asyncAddTaskData(const QString &add_task_data_sql);
    void asyncUpdateTaskData(MTaskData &taskData);
    bool getTaskData(const QString &code);//判断是否存在任务数据表
    bool delTaskData(const QString &code);//清空任务数据表的数据+删除数据表

    QString read_file(QString filename);

    QVector<QVector<QString>> select(int queryColumnCount,const QString &sql);
    int getTableRowCount(const QString &tbName);// 获取表行数
    QStringList getTableFields(const QString &tbName);// 获取业务表的字段集合

    int mem_ratio();
    void release();


    QVector<MTaskGroup> taskGroups;
    QVector<MTaskUserAgent>  userAgents;
    QString getRandomUserAgent();

    void setFinger(const QString &finger);
    QString getFinger() const;


protected:
    void run() override;

private:
    QSqlDatabase *m_sqldb;
    QQueue<QString> m_addTaskDataQueue;
    QQueue<MTaskData> m_updateTaskDataQueue;

    static QAtomicPointer<Database> m_instance;
    static QMutex m_instancemtx;
    QString m_finger;//当前机器指纹

signals:

};

#endif // DATABASE_H
