#include "ComDownloaderThread.h"
#include "Api.h"
#include <QPixmap>
#include <QFile>
#include <QsLog.h>

ComDownloadTask::ComDownloadTask(const QString &url,const QString &saveDir,const QString saveSuffix,const QString &saveFilename)
    :mUrl(url),mSaveDir(saveDir),mSaveSuffix(saveSuffix),mSaveFilename(saveFilename)
{

}
ComDownloaderThread::ComDownloaderThread(QObject *parent,int interval): QThread(parent),mInterval(interval){
    QLOG_INFO()<<"ComDownloaderThread::ComDownloaderThread()";
}
ComDownloaderThread::~ComDownloaderThread(){
    QLOG_INFO()<<"ComDownloaderThread::~ComDownloaderThread()";
    terminate();
    wait();

}
void ComDownloaderThread::pushDownloadTask(ComDownloadTask &downloadTask){
    ComDownloaderThread::mDownloadTaskQ.enqueue(downloadTask);
}
int ComDownloaderThread::getDownloadTaskQSize(){
    return mDownloadTaskQ.size();
}
void ComDownloaderThread::run(){
    int addTotalCount=0;
    int addSuccessCount = 0;
    int addErrorCount = 0;

    while (true) {
        int size = mDownloadTaskQ.size();
        if(size > 0){
            ++addTotalCount;
            //存在下载任务
            ComDownloadTask downloadTask = mDownloadTaskQ.dequeue();//出队列
            bool state = false;
            QString msg;
            QByteArray byteArray = Api::download(downloadTask.getUrl(),state,msg);
            if(state){
                //下载成功
                if(".jpg" == downloadTask.getSaveSuffix()){
                    QPixmap pix;
                    if(pix.loadFromData(byteArray,"jpg")){
                        pix.save(downloadTask.getSaveFilename());
                         ++addSuccessCount;
                    }else{
                        QLOG_INFO()<<"ComDownloaderThread::run() save image error url="<<downloadTask.getUrl();
                         ++addErrorCount;
                    }
                }else{
                    QFile file(downloadTask.getSaveFilename());
                    if(file.open(QIODevice::WriteOnly)){
                        file.write(byteArray);
                        file.close();
                        ++addSuccessCount;
                    }else{
                        QLOG_INFO()<<"ComDownloaderThread::run() save file error url="<<downloadTask.getUrl();
                        ++addErrorCount;
                    }

                }
            }else{
                QLOG_INFO()<<"ComDownloaderThread::run() error url="<<downloadTask.getUrl()<<"，msg="<<msg;
                ++addErrorCount;
            }
        }else{
            sleep(1);
        }

        sleep(mInterval);
        if(0==size || 1==size){
            emit this->notifyGetDownloadTask(addTotalCount,addSuccessCount,addErrorCount);
            addTotalCount = 0;
            addSuccessCount = 0;
            addErrorCount = 0;
        }
    }
}
