#include "TaskWebEngineProfile.h"
#include <QMutex>
#include <QMutexLocker>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QsLog.h>
#include "Utils/Utils.h"

QAtomicPointer<TaskWebEngineProfile> TaskWebEngineProfile::mInstance = 0;
QMutex TaskWebEngineProfile::mInstanceMtx;

TaskWebEngineProfile::TaskWebEngineProfile(QObject *parent) : QObject(parent)
{
    //  setZoomFactor(zoomFactor() - 0.3);
//    mProfile =new QWebEngineProfile(this);
//    mProfile =  QWebEngineProfile::defaultProfile();
    mCacheName = QString::fromLatin1("ChromiumV%1").arg(qWebEngineChromiumVersion());
    mProfile = new QWebEngineProfile(mCacheName,this);
//    QString cachePath = "/file";
//    profile->setCachePath(cachePath);
//    profile->setPersistentStoragePath(cachePath);
    mProfile->setHttpCacheType(QWebEngineProfile::HttpCacheType::DiskHttpCache);
    mProfile->setPersistentCookiesPolicy(QWebEngineProfile::PersistentCookiesPolicy::ForcePersistentCookies);

    mCachePath = mProfile->persistentStoragePath();
//    mProfile->setHttpCacheMaximumSize(102400);
    QLOG_INFO() <<"TaskWebEngineProfile cachePath()="<<mCachePath;

    QWebEngineSettings *settings = mProfile->settings();
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    settings->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
    settings->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanPaste, true);

    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, false);

}
TaskWebEngineProfile::~TaskWebEngineProfile(){

}
TaskWebEngineProfile * TaskWebEngineProfile::getInstance(){
    //! testAndSetOrders操作保证在原子操作前和后的的内存访问 不会被重新排序。
    if(mInstance.testAndSetOrdered(0,0)){
        QMutexLocker locker(&mInstanceMtx);
        mInstance.testAndSetOrdered(0,new TaskWebEngineProfile(nullptr));
    }
    return mInstance;

}

QWebEngineProfile* TaskWebEngineProfile::profile(){
    return mProfile;
}
QWebEngineProfile* TaskWebEngineProfile::copyProfile(){

    QWebEngineProfile *newCopyProfile = new QWebEngineProfile(mCacheName,this);
    newCopyProfile->setHttpCacheType(QWebEngineProfile::HttpCacheType::MemoryHttpCache);
    newCopyProfile->setPersistentCookiesPolicy(QWebEngineProfile::PersistentCookiesPolicy::AllowPersistentCookies);

    return newCopyProfile;
}
QString TaskWebEngineProfile::cachePath(){
    return mCachePath;
}
void TaskWebEngineProfile::clearCache(){


    mProfile->clearAllVisitedLinks();
    mProfile->clearHttpCache();

    Utils::clearDir(mCachePath);

}
