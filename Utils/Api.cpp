#include "Api.h"
#include "constant.h"
#include "database.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTimer>
#include <QsLog.h>


Api::Api(QObject *parent) : QObject(parent)
{
    QLOG_INFO() <<"Api::Api()";

}
Api::~Api(){
    QLOG_INFO() <<"Api::~Api()";

}
QByteArray Api::download(const QString &url,bool &state,QString &msg){
//    QLOG_INFO() <<"Api::download url="<<url;
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest qurl(url);
//    qurl.setRawHeader("Content-Type", "application/octet-stream");
    QNetworkReply *reply = manager.get(qurl);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);//请求结束并下载完成后，退出子事件循环
//    QTimer::singleShot(3000, &loop, &QEventLoop::quit);// 3s超时
    loop.exec();
    QByteArray byteArray;
    if(reply->error()!= QNetworkReply::NoError){
        state = false;
        msg = reply->errorString();
    }else {
        state = true;
        byteArray = reply->readAll();
    }
//    reply->abort();
    reply->deleteLater();
    return byteArray;
}
void Api::reportHeart(uint64_t reportCount){
    QString url = QString("%1/reportHeart?version=%2&finger=%3&count=%4").
            arg(HOST).
            arg(QCoreApplication::applicationVersion()).
            arg(Database::getInstance()->getFinger()).arg(reportCount);
//    QLOG_INFO() <<"Api::reportHeart url="<<url;

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);//请求结束并下载完成后，退出子事件循环
//    QTimer::singleShot(3000, &loop, &QEventLoop::quit);// 3s超时

    loop.exec();
//    reply->abort();
    reply->deleteLater();

    bool state = false;
    QString msg = "未知错误";
    if(reply->error()!= QNetworkReply::NoError){
        msg = reply->errorString();
    }else {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll(),&jsonError);
        if(jsonError.error!=QJsonParseError::NoError){
            msg = jsonError.errorString();
        }else{
            QJsonObject json = document.object();
            msg = json.value("msg").toString();
            if(json.value("code").toInt() == 1000){
                state = true;
            }
        }
    }
//    QLOG_INFO() <<"Api::reportHeart state="<<state<<",msg="<<msg;

}
void Api::reportCrash(QString &crashMsg){

    /**
     * &buildCpuArchitecture=x86_64
     * &currentCpuArchitecture=x86_64
     * &machineUniqueId=f1ec52cd-6c2c-439b-9406-469d9476a089&bootUniqueId=
     * &prettyProductName=Windows 10 Version 2009
     * &productType=windows&buildAbi=x86_64-little_endian-llp64
     * &machineHostName=shisan
     * &kernelVersion=10.0.19044
     * &kernelType=winnt
     * &productVersion=10"
     **/

    QHash<QString,QString> params;

    params.insert("finger",Database::getInstance()->getFinger());
    params.insert("bootUniqueId",QSysInfo::bootUniqueId());
    params.insert("buildAbi",QSysInfo::buildAbi());
    params.insert("buildCpuArchitecture",QSysInfo::buildCpuArchitecture());
    params.insert("currentCpuArchitecture",QSysInfo::currentCpuArchitecture());
    params.insert("kernelType",QSysInfo::kernelType());
    params.insert("kernelVersion",QSysInfo::kernelVersion());
    params.insert("machineHostName",QSysInfo::machineHostName());
    params.insert("machineUniqueId",QSysInfo::machineUniqueId());
    params.insert("prettyProductName",QSysInfo::prettyProductName());
    params.insert("productType",QSysInfo::productType());
    params.insert("productVersion",QSysInfo::productVersion());
    params.insert("crashMsg",crashMsg);

    QString url = HOST+"/reportCrash?version="+QCoreApplication::applicationVersion();
    QHash<QString,QString>::const_iterator it;
    for (it=params.constBegin();it!=params.constEnd();++it) {
        url +="&"+it.key()+"="+it.value();
    }
    QLOG_INFO() <<"Api::reportCrash url="<<url;

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);//请求结束并下载完成后，退出子事件循环
//    QTimer::singleShot(3000, &loop, &QEventLoop::quit);// 3s超时
    loop.exec();

//    reply->abort();
    reply->deleteLater();

    bool state = false;
    QString msg = "未知错误";
    if(reply->error()!= QNetworkReply::NoError){
        msg = reply->errorString();
    }else {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll(),&jsonError);
        if(jsonError.error!=QJsonParseError::NoError){
            msg = jsonError.errorString();
        }else{
            QJsonObject json = document.object();
            msg = json.value("msg").toString();
            if(json.value("code").toInt() == 1000){
                state = true;
            }
        }
    }

    QLOG_INFO() <<"Api::reportCrash state="<<state<<",msg="<<msg;

}


