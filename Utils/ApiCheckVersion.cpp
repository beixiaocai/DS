#include "ApiCheckVersion.h"
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
#include <QsLog.h>

QAtomicPointer<ApiCheckVersion> ApiCheckVersion::mInstance = 0;
QMutex ApiCheckVersion::mInstanceMtx;

ApiCheckVersion::ApiCheckVersion(QObject *parent) : QObject(parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
//    QNetworkCookieJar *cookieJar =  new QNetworkCookieJar(manager);
//    cookieJar->setCookiesFromUrl(networkManager->cookieJar()->cookiesForUrl(qurl),qurl);
//    manager->setCookieJar(cookieJar);
    connect(mNetworkManager,&QNetworkAccessManager::finished, this,&ApiCheckVersion::onRequestFinished);

}
ApiCheckVersion::~ApiCheckVersion(){
    if(mNetworkManager){
        mNetworkManager->disconnect();
        delete mNetworkManager;
        mNetworkManager = nullptr;
    }
}
ApiCheckVersion * ApiCheckVersion::getInstance(){
    //! testAndSetOrders操作保证在原子操作前和后的的内存访问 不会被重新排序。
    if(mInstance.testAndSetOrdered(0,0)){
        QMutexLocker locker(&mInstanceMtx);
        mInstance.testAndSetOrdered(0,new ApiCheckVersion(nullptr));
    }
    return mInstance;

}

void ApiCheckVersion::onRequestFinished(QNetworkReply *reply){
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
                QJsonObject data = json.value("data").toObject();
                version.version = data.value("version").toString().toFloat();
                version.pubdate = data.value("pubdate").toString();
                version.updateContent = data.value("updateContent").toString();
                version.url = data.value("url").toString();

            }
        }
    }
    QLOG_INFO() <<"ApiCheckVersion::onRequestFinished state="<<state<<",msg="<<msg;
    emit this->notifyCheckVersion(state,msg,version);

}

void ApiCheckVersion::checkVersion(){

//    bool state = false;
//    QString msg = "当前是最新版本";
//    MVersion version;
//    version.version = QString("1.0").toFloat();
//    version.pubdate = "2023.3.16";
//    version.updateContent = "更新了，谢谢";
//    version.url = "https://gitee.com/Vanishi";
//    emit this->notifyGetVersion(state,msg,version);

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

    QString url = HOST+"/checkVersion?version="+QCoreApplication::applicationVersion();

    QHash<QString,QString>::const_iterator it;
    for (it=params.constBegin();it!=params.constEnd();++it) {
        url +="&"+it.key()+"="+it.value();
    }
//    QLOG_INFO() <<"ApiCheckVersion::checkVersionh url="<<url;

    QUrl qurl(url);
    QNetworkRequest request(qurl);
    mNetworkManager->get(request);
}
