#include "ReportThread.h"
#include "Api.h"
#include <QsLog.h>

ReportThread::ReportThread(QObject *parent): QThread(parent){
}
ReportThread::~ReportThread(){
    terminate();
    wait();
}

void ReportThread::run(){
    uint64_t reportCount = 0;
    while (true) {
        //QLOG_INFO()<<reportCount<< "-ReportThread::run()ReportThread::run()ReportThread::run()ReportThread::run()ReportThread::run()";

        sleep(120);
        Api::reportHeart(reportCount);
        ++reportCount;
    }

}
