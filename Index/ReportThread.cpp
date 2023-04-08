#include "ReportThread.h"
#include "Api.h"

ReportThread::ReportThread(QObject *parent): QThread(parent){
}
ReportThread::~ReportThread(){
    terminate();
    wait();
}

void ReportThread::run(){
    uint64_t reportCount = 0;
    while (true) {
        sleep(120);
        Api::reportHeart(reportCount);
        ++reportCount;
    }

}
