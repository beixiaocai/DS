#include "mainwindow.h"
#include "ComMessageBox.h"
#include "Api.h"
#include <QApplication>
#include <QSharedMemory>
#include <QFile>
#include <QDir>
#include <windows.h>
#include <QsLog.h>
#include <QDate>
#include <QDebug>
using namespace  QsLogging;

bool mkDirs(QString dirPath)
{
    QDir dir(dirPath);
    if(dir.exists())
    {
      return true;
    }
    else
    {
       bool result = dir.mkpath(dirPath);//创建多级目录
       return result;
    }
}

void initLogger(const QString &logDir){

    if(mkDirs(logDir)){
        Logger& logger = Logger::instance();
        logger.setLoggingLevel(QsLogging::TraceLevel);
        //    QString timeStr = QDate::currentDate().toString("yyyy-MM-dd");
        //    QDateTime time = QDateTime::fromString(timeStr,"yyyy-MM-dd");

        QString logFile = QString("%1/run.log").arg(logDir);
        DestinationPtr des(
                    DestinationFactory::MakeFileDestination(logFile,
                               EnableLogRotation,
                               MaxSizeBytes(1*1024*1024),
                               MaxOldLogCount(1)));
        logger.addDestination(des);
        QLOG_INFO()<<"initLogger() success logDir="<<logDir;
    }else{
        QLOG_INFO()<<"initLogger() error logDir="<<logDir;
    }

}
void cleanLogDir(const QString &logDir){

    QDir dir(logDir);
    if(!dir.exists()){
        return ;
    }
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i){
        QFileInfo fileInfo = list.at(i);
        QString filename =fileInfo.fileName();
        if(filename.size() > 10){
            QDateTime time = QDateTime::fromString(filename.mid(0,10),"yyyy-MM-dd");
            if(time.toSecsSinceEpoch() > 0){
                //此前遗留的日志文件，删除
                fileInfo.dir().remove(fileInfo.absoluteFilePath());
            }
        }

    }
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){

    EXCEPTION_RECORD* record = pException->ExceptionRecord;

    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QString crashMsg = QString("抱歉，软件发生了崩溃，请重启。错误代码：%1，错误地址：%2").
            arg(errCode).arg(errAdr);

    Api::reportCrash(crashMsg);
    ComMessageBox::error(NULL,crashMsg);

    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("any12345");
    QCoreApplication::setOrganizationDomain("www.any12345.com");
    QCoreApplication::setApplicationName("DS");
    QCoreApplication::setApplicationVersion("1.6");

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数


#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif


    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(QStringLiteral(":/res/images/logo.png")));
    QFile qss(":/res/qss/style.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    QSharedMemory shared("DS");
    if(shared.attach()){
        ComMessageBox::error(NULL,"软件已经在运行中");
        return 0;
    }
    shared.create(1);

    // 获取程序当前地址，需要在QApplication初始化之后获取，否则获取的地址错误
//    QString logDir = QDir::homePath()+"/AppData/Local/DS/logs";
    QString logDir = QApplication::applicationDirPath() + "/logs";
//    cleanLogDir(logDir);
    initLogger(logDir);

    QLOG_INFO()<< "DS V"+QCoreApplication::applicationVersion()+"  Start";


    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
