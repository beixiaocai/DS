#include "mainwindow.h"
#include "ComMessageBox.h"
#include "Api.h"
#include "Utils.h"
#include <QApplication>
#include <QSharedMemory>
#include <windows.h>
#include <QDate>
#include <qtwebenginecoreglobal.h>
#include <QsLog.h>
#include <QDebug>
using namespace  QsLogging;

void initLogger(const QString &logDir){

    if(Utils::mkDirs(logDir)){
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
    QCoreApplication::setOrganizationName("beixiaocai");
    QCoreApplication::setOrganizationDomain("ds.beixiaocai.com");
    QCoreApplication::setApplicationName("DS");
    QCoreApplication::setApplicationVersion("1.9");

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
//    Utils::clearLogDir(logDir);
    initLogger(logDir);

    QString vInfo = QString("DS V%1,qwebengine V%2 chromium V%3 chromiumSP V%4").
            arg(QCoreApplication::applicationVersion()).
            arg(qWebEngineVersion()).
            arg(qWebEngineChromiumVersion()).
            arg(qWebEngineChromiumSecurityPatchVersion());

    qDebug()<< vInfo;
    QLOG_INFO()<< vInfo;

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
