#include "Report.h"
#include "Utils/ApiReport.h"

Report::Report(): QThread(nullptr){

}
Report::~Report(){

    terminate();
    wait();
}
void Report::run(){

    uint32_t reportCount = 0;

    while (true) {
        sleep(120);
        ApiReport::getInstance()->reportHeart(reportCount);
        ++reportCount;

    }
}
