#include "RunWebEngineViewManager.h"
#include "RunWebEngineView.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "constant.h"
#include "RunDialog.h"
#include "RunHelper.h"
#include "Task/TaskWebEngineProfile.h"
#include "TaskFlow/mFcModels.h"
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QWebEngineScript>
#include <QWebEngineSettings>
#include <QWebEngineCookieStore>
#include <QWebEngineHttpRequest>
#include <QNetworkAccessManager>
#include <QWebEngineCookieStore>
#include <QLineEdit>
#include <QsLog.h>

RunWebEngineViewManager::RunWebEngineViewManager(RunDialog *runDialog) : QObject(runDialog){
    QLOG_INFO()<<"RunWebEngineViewManager::RunWebEngineViewManager()";
    this->mHelper = runDialog->getHelper();

    if(mHelper->getTask()->runParams.isTraceless){//开启无痕浏览器模式
        mProfile = new QWebEngineProfile(this);
        mProfile->setHttpCacheType(QWebEngineProfile::HttpCacheType::NoCache);
        mProfile->setHttpCacheMaximumSize(0);
        mProfile->setPersistentCookiesPolicy(QWebEngineProfile::PersistentCookiesPolicy::NoPersistentCookies);
    }else{
        mProfile = TaskWebEngineProfile::getInstance()->copyProfile();
    }


    mProfile->setHttpUserAgent(mHelper->getTask()->defaultUserAgent);

    QString sourceCode= QString(
    "let ej = document.createElement('script');ej.type = 'text/javascript';"
    "ej.appendChild(document.createTextNode('%1'));"
    "document.body.appendChild(ej);").arg(Database::getInstance()->read_file(JS_EXECUTE_FILE).replace("\r"," ").replace("\n"," ").replace("\t"," "));
    QWebEngineScript script;
    mProfile->scripts()->remove(script);
    script.setSourceCode(sourceCode);
    mProfile->scripts()->insert(script);
    QWebEngineSettings *settings = mProfile->settings();

    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, false); //关闭缓存
    settings->setAttribute(QWebEngineSettings::AllowRunningInsecureContent,mHelper->getTask()->isBrowserAllowRunningInsecureContent);
    settings->setAttribute(QWebEngineSettings::AutoLoadImages,mHelper->getTask()->isBrowserAutoLoadImages);
    settings->setAttribute(QWebEngineSettings::AutoLoadIconsForPage,mHelper->getTask()->isBrowserAutoLoadIconsForPage);
    settings->setAttribute(QWebEngineSettings::PluginsEnabled,mHelper->getTask()->isBrowserPluginsEnabled);


    connect(this,&RunWebEngineViewManager::cleanWebViews,this,&RunWebEngineViewManager::onCleanWebViews);
    connect(this,&RunWebEngineViewManager::webViewNotifyUrlChanged,runDialog,&RunDialog::onWebViewUrlChanged);
    connect(this,&RunWebEngineViewManager::cleanWebViews,runDialog,&RunDialog::onCleanWebViews);
    this->createWebView();

}
RunWebEngineViewManager::~RunWebEngineViewManager(){
    QLOG_INFO()<<"RunWebEngineViewManager::~RunWebEngineViewManager()";

    if(m_reusedWebView!=nullptr){
        delete m_reusedWebView;
        m_reusedWebView = nullptr;
    }
    for (int i = 0; i < mWebViews.length(); ++i) {
        delete mWebViews[i];
        mWebViews[i] = nullptr;
    }
    mWebViews.clear();

    if(mProfile){
        delete mProfile;
        mProfile = nullptr;
    }
}
QWebEngineProfile* RunWebEngineViewManager::profile(){
    return mProfile;
}
void RunWebEngineViewManager::setCurrentWebView(RunWebEngineView *webView){

    m_currentWebView = webView;
}
RunWebEngineView * RunWebEngineViewManager::getCurrentWebView(){

    return m_currentWebView;
}

RunWebEngineView * RunWebEngineViewManager::createWebView(){
    if(m_reusedWebView==nullptr){
        RunWebEngineView *webView = new RunWebEngineView(this,(QWidget*)this->parent());
        mWebViews.append(webView);

        emit cleanWebViews(webView);
        return webView;
    }else {
        RunWebEngineView *webView = m_reusedWebView;
        m_reusedWebView = nullptr;
        mWebViews.append(webView);

        emit cleanWebViews(webView);
        return webView;
    }
}

void RunWebEngineViewManager::onCleanWebViews(RunWebEngineView *webView){
    setCurrentWebView(webView);
    // 清理webView
    QVector<RunWebEngineView *> inUseWebViews;

    inUseWebViews.append(m_currentWebView);
    for (int i = 0; i < mHelper->loops.length(); ++i) {
        RunWebEngineView *webView =  mHelper->loops[i]->carryCurrentWebView;
        if(webView!=nullptr){
            inUseWebViews.append(webView);
        }
    }
    for (int i = 0; i < mWebViews.length(); ++i) {
        RunWebEngineView *webView = mWebViews[i];

        bool isDelete = true;
        for (int j = 0; j < inUseWebViews.length(); ++j) {
            if(inUseWebViews.at(j)==webView){
                isDelete = false;
                break;
            }
        }
        if(isDelete){
            webView->page()->profile()->clearHttpCache();
            webView->page()->profile()->clearAllVisitedLinks();

            mWebViews.remove(i);
            if(m_reusedWebView==nullptr){
                m_reusedWebView = webView;
            }else {
                webView->disconnect();
                delete webView;
                webView = nullptr;
            }
        }
    }
}
