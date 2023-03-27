#include "ApiReport.h"
#include "constant.h"
#include "models.h"
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
#include <QMutex>
#include <QMutexLocker>
//#include <QsLog.h>

QAtomicPointer<ApiReport> ApiReport::mInstance = 0;
QMutex ApiReport::mInstanceMtx;

ApiReport::ApiReport(QObject *parent) : QObject(parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager,&QNetworkAccessManager::finished, this,&ApiReport::replyFinished);
}
ApiReport::~ApiReport(){

}
ApiReport * ApiReport::getInstance(){
    //! testAndSetOrders操作保证在原子操作前和后的的内存访问 不会被重新排序。
    if(mInstance.testAndSetOrdered(0,0)){
        QMutexLocker locker(&mInstanceMtx);
        mInstance.testAndSetOrdered(0,new ApiReport());
    }
    return mInstance;

}


void ApiReport::replyFinished(QNetworkReply *reply){

    reply->abort();
    reply->deleteLater();

    MVersion version;
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
    qDebug() <<"ApiReport::replyFinished state="<<state<<",msg="<<msg;
}

void ApiReport::reportCrash(QString &crashMsg){
    return ;
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
    qDebug() <<url;
    qDebug()<<url;

    QUrl qurl(url);
    QNetworkRequest request(qurl);
    mNetworkManager->get(request);

}

void ApiReport::reportHeart(uint32_t reportCount){
    QString url = QString("%1/reportHeart?version=%2&finger=%3&count=%4").
            arg(HOST).
            arg(QCoreApplication::applicationVersion()).
            arg(Database::getInstance()->getFinger()).arg(reportCount);
    QUrl qurl(url);
    QNetworkRequest request(qurl);
    mNetworkManager->get(request);
}

