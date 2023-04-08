#ifndef COMDOWNLOADERTHREAD_H
#define COMDOWNLOADERTHREAD_H

#include <QThread>
#include <QQueue>

struct ComDownloadTask{
    ComDownloadTask() = delete;
    ComDownloadTask(const QString &url,const QString &saveDir,const QString saveSuffix,const QString &saveFilename);
public:
    QString getUrl(){return mUrl;};
    QString getSaveDir(){return mSaveDir;};
    QString getSaveSuffix(){return mSaveSuffix;};
    QString getSaveFilename(){return mSaveFilename;};
private:
    QString mUrl;
    QString mSaveDir;//下载文件路径
    QString mSaveSuffix;//下载文件后缀
    QString mSaveFilename;//下载文件名称（包括文件后缀）

};
class ComDownloaderThread : public QThread
{
    Q_OBJECT
public:
    explicit ComDownloaderThread(QObject *parent,int interval);
    ~ComDownloaderThread();
public:
    void pushDownloadTask(ComDownloadTask &downloadTask);
    int getDownloadTaskQSize();
private:
    QQueue<ComDownloadTask> mDownloadTaskQ;
    int mInterval;//线程休眠间隔 （单位秒）
protected:
    void run() override;
signals:
    void notifyGetDownloadTask(int addTotalCount,int addSuccessCount,int addErrorCount);

private slots:

};

#endif // COMDOWNLOADERTHREAD_H
