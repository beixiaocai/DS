#include "TaskDataDownloadManager.h"
#include "ComDownloaderThread.h"
#include "database.h"
#include <QsLog.h>

TaskDataDownloadManager::TaskDataDownloadManager(QObject *parent,
                                                 const QString &taskName,const QString &taskCode,const QStringList &fields,
                                                 int threadCount,int threadInterval,
                                                 const QString &field,int fieldIndex,const QString &fieldPrefix,const QString &saveDir,const QString &saveSuffix) :
    QObject(parent),mTaskName(taskName),mTaskCode(taskCode),mFields(fields),
    mThreadCount(threadCount),mThreadInterval(threadInterval),
    mField(field),mFieldIndex(fieldIndex),mFieldPrefix(fieldPrefix),mSaveDir(saveDir),mSaveSuffix(saveSuffix){

    QLOG_INFO()<<"TaskDataDownloadManager::TaskDataDownloadManager()";

    for (int i = 0; i < threadCount; ++i) {
        ComDownloaderThread *downloaderThread = new ComDownloaderThread(this,threadInterval);
        connect(downloaderThread, &ComDownloaderThread::notifyGetDownloadTask, this, [this,downloaderThread](int addTotalCount,int addSuccessCount,int addErrorCount) {
            this->mTotalCount += addTotalCount;
            this->mSuccessCount += addSuccessCount;
            this->mErrorCount += addErrorCount;

            if(mDownloadTaskQ.size() > 0){
                ComDownloadTask downloadTask = mDownloadTaskQ.dequeue();//出队列
                downloaderThread->pushDownloadTask(downloadTask);
            }else{
                //从数据库读取任务
                this->readSeedFromDatabase();
            }
            if(mSeedIsFinished && 0 == this->getAllDownloadTaskQSize()){
                //种子数据已经结束 && 所有任务线程的种子队列也为空
                QString log = QString("下载已完成 '%1'，处理任务数 %2，成功 %3，失败 %4").arg(mField).arg(mTotalCount).arg(mSuccessCount).arg(mErrorCount);
                emit this->notifyLog(mField,log);
                this->stopDownloaderThreads();
            }else{
                QString log = QString("下载中 '%1'，处理任务数 %2，成功 %3，失败 %4").arg(mField).arg(mTotalCount).arg(mSuccessCount).arg(mErrorCount);
                emit this->notifyLog(mField,log);
            }
        });

        downloaderThread->start();
        mDownloaderThreads.push_back(downloaderThread);
    }
}
TaskDataDownloadManager::~TaskDataDownloadManager(){
    QLOG_INFO()<<"TaskDataDownloadManager::~TaskDataDownloadManager()";
    this->stopDownloaderThreads();
}
void TaskDataDownloadManager::stopDownloaderThreads(){

    for (int i = 0; i < mDownloaderThreads.size(); ++i) {
        delete mDownloaderThreads[i];
        mDownloaderThreads[i] = nullptr;
    }
    mDownloaderThreads.clear();

}
bool TaskDataDownloadManager::getAllDownloadTaskQSize(){
    int size = mDownloadTaskQ.size();
    for (int i = 0; i < mDownloaderThreads.size(); ++i) {
        size += mDownloaderThreads[i]->getDownloadTaskQSize();
    }
    return size;
}
void TaskDataDownloadManager::readSeedFromDatabase(){
    if(mSeedIsFinished){
        return;
    }
    int fieldCount = mFields.length();
    QString sql = QString("select %1 from %2 limit %3,%4 ").arg(mFields.join(",")).
            arg(mTaskCode).
            arg(mPageSize * (mCurPage-1)).
            arg(mPageSize);

    QVector<QVector<QString>> data = Database::getInstance()->select(fieldCount,sql);

    if(0==data.length()){
        //任务已经完全被消费
        mSeedIsFinished = true;
    }else{
        for (int row = 0; row < data.length(); ++row) {
            int fieldId = -1;
            QString url;
            for (int column = 0; column < fieldCount; ++column) {
                if(column==0){
                    fieldId = data[row][column].toInt();//数据表的ID
                }
                if(mFieldIndex == column){
                     url =mFieldPrefix + data[row][column]; //数据表该字段的值
                }
            }
            //构造下载任务
            QString saveFilename = QString("%1/%2_%3%4").arg(mSaveDir).arg(fieldId).arg(mField).arg(mSaveSuffix);
            ComDownloadTask downloadTask(url,mSaveDir,mSaveSuffix,saveFilename);
            mDownloadTaskQ.enqueue(downloadTask);
        }

        ++mCurPage;
    }


}
