#ifndef TASKDATADOWNLOADMANAGER_H
#define TASKDATADOWNLOADMANAGER_H

#include <QObject>
#include <QVector>
#include <QQueue>

struct ComDownloadTask;
class ComDownloaderThread;
class TaskDataDownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskDataDownloadManager(QObject *parent,const QString &taskName,const QString &taskCode,const QStringList &fields,int threadCount,int threadInterval,const QString &field,int fieldIndex,const QString &saveDir,const QString &saveSuffix);
    ~TaskDataDownloadManager();

private:
    QString mTaskName;
    QString mTaskCode;
    QStringList mFields;
    int mThreadCount;
    int mThreadInterval;
    QString mField;
    int mFieldIndex;
    QString mSaveDir;
    QString mSaveSuffix;
private:
    QQueue<ComDownloadTask> mDownloadTaskQ;
    QVector<ComDownloaderThread *> mDownloaderThreads;
    void stopDownloaderThreads();

    uint64_t mTotalCount = 0;  //下载总数量
    uint64_t mSuccessCount = 0;//下载成功总数量
    uint64_t mErrorCount = 0;  //下载失败总数量


    bool getAllDownloadTaskQSize();//获取所有待执行的任务数量
    void readSeedFromDatabase();//从数据库获取下载任务
    bool mSeedIsFinished = false;//种子数据是否已经结束
    int  mCurPage = 1;    // 当前页的页数
    int  mPageSize = 100; // 每页读取数量

signals:
    void notifyLog(const QString &field,const QString &log);

};

#endif // TASKDATADOWNLOADMANAGER_H
