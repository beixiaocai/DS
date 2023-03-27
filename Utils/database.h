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


    bool createTable(const QString &tbName,const QStringList &fields); // 创建业务表
    bool clearTable(const QString &tbName);

    void async_addTaskData(const QString &add_task_data_sql);
    void async_updateTaskData(MTaskData &taskData);
    bool addTask(MTask *task,QString &msg);
    bool delTask(const QString &code,QString &msg);
    bool delTaskData(const QString &code,bool isDel = false);

    bool getTasks(int groupId,QString &msg,QVector<MTask *> &tasks);
    bool getTaskWithCode(const QString &code,QString &msg,MTask *task);
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


    // 在mainwindow上打开的独立的dialog，退出登录的时候，需要确保这些对象全部销毁，否则会出现内存泄漏
    QHash<QObject *,bool> runDialogs;

protected:
    void run() override;

private:
    QSqlDatabase *m_sqldb;
    QQueue<QString> m_addTaskDataQueue;
    QQueue<MTaskData> m_updateTaskDataQueue;

    static QAtomicPointer<Database> m_instance;
    static QMutex m_instancemtx;
    QString m_finger;

signals:

};

#endif // DATABASE_H
