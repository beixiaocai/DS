#include "mainwindow.h"
#include "ComMessageBox.h"
#include "ApiReport.h"
#include <QApplication>
#include <QSharedMemory>
#include <QFile>
#include <QDir>
#include <windows.h>
//#include <QsLog.h>
#include <QDate>
#include <QDebug>
//using namespace  QsLogging;

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

//void initLogger(QString &logDir){

//    if(mkDirs(logDir)){
//        Logger& logger = Logger::instance();
//        logger.setLoggingLevel(QsLogging::TraceLevel);
//        QString logPath = QString("%1/%2.txt").arg(logDir,QDate::currentDate().toString("yyyy-MM-dd"));
//        DestinationPtr des(DestinationFactory::MakeFileDestination(
//                                           logPath, EnableLogRotation,MaxSizeBytes(10*1024*1024),MaxOldLogCount(5)));
//        logger.addDestination(des);
//        qDebug()<<"main.cpp: initLogger success logDir="<<logDir;
//    }else{
//        qDebug()<<"main.cpp: initLogger error logDir="<<logDir;
//    }

//}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){

    EXCEPTION_RECORD* record = pException->ExceptionRecord;

    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QString crashMsg = QString("抱歉，软件发生了崩溃，请重启。错误代码：%1，错误地址：%2").
            arg(errCode).arg(errAdr);

    ApiReport::getInstance()->reportCrash(crashMsg);
    ComMessageBox::error(NULL,crashMsg);

    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("any12345");
    QCoreApplication::setOrganizationDomain("www.any12345.com");
    QCoreApplication::setApplicationName("DS");
    QCoreApplication::setApplicationVersion("1.3");

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
//    initLogger(logDir);

    qDebug() << "mainwindow.cpp: DS start";
    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
