#include "RunWebviewManager.h"
#include "RunWebview.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "constant.h"
#include "RunDialog.h"
#include "RunHelper.h"
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
#include <QDebug>



RunWebViewManager::RunWebViewManager(RunDialog *runDialog) : QObject(runDialog){
    this->m_helper = runDialog->getHelper();

    profile =new QWebEngineProfile(this);
//    profile =QWebEngineProfile::defaultProfile();
    profile->setHttpCacheType(QWebEngineProfile::HttpCacheType::NoCache);
    profile->setHttpCacheMaximumSize(0);
    profile->setHttpUserAgent(m_helper->getTask()->defaultUserAgent);

    QString sourceCode= QString(
    "let ej = document.createElement('script');ej.type = 'text/javascript';"
    "ej.appendChild(document.createTextNode('%1'));"
    "document.body.appendChild(ej);").arg(Database::getInstance()->read_file(JS_EXECUTE_FILE).replace("\r"," ").replace("\n"," ").replace("\t"," "));
    QWebEngineScript script;
    profile->scripts()->remove(script);
    script.setSourceCode(sourceCode);
    profile->scripts()->insert(script);
    QWebEngineSettings *settings = profile->settings();
//    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, false); //关闭缓存

    settings->setAttribute(QWebEngineSettings::AllowRunningInsecureContent,m_helper->getTask()->isBrowserAllowRunningInsecureContent);
    settings->setAttribute(QWebEngineSettings::AutoLoadImages,m_helper->getTask()->isBrowserAutoLoadImages);
    settings->setAttribute(QWebEngineSettings::AutoLoadIconsForPage,m_helper->getTask()->isBrowserAutoLoadIconsForPage);
    settings->setAttribute(QWebEngineSettings::PluginsEnabled,m_helper->getTask()->isBrowserPluginsEnabled);


    connect(this,&RunWebViewManager::cleanWebViews,this,&RunWebViewManager::onCleanWebViews);
    connect(this,&RunWebViewManager::webViewNotifyUrlChanged,runDialog,&RunDialog::onWebViewUrlChanged);
    connect(this,&RunWebViewManager::cleanWebViews,runDialog,&RunDialog::onCleanWebViews);
    this->createWebView();

}
RunWebViewManager::~RunWebViewManager(){
    qDebug()<<"RunWebViewManager::~RunWebViewManager()";

    if(m_reusedWebView!=nullptr){
        delete m_reusedWebView;
        m_reusedWebView = nullptr;
    }
    for (int i = 0; i < mWebViews.length(); ++i) {
        delete mWebViews[i];
        mWebViews[i] = nullptr;
    }
    mWebViews.clear();
}

void RunWebViewManager::setCurrentWebView(RunWebView *webView){

    m_currentWebView = webView;
}
RunWebView * RunWebViewManager::getCurrentWebView(){

    return m_currentWebView;
}

RunWebView * RunWebViewManager::createWebView(){
    if(m_reusedWebView==nullptr){
        RunWebView *webView = new RunWebView(this,(QWidget*)this->parent());
        mWebViews.append(webView);

        emit cleanWebViews(webView);
        return webView;
    }else {
        RunWebView *webView = m_reusedWebView;
        m_reusedWebView = nullptr;
        mWebViews.append(webView);

        emit cleanWebViews(webView);
        return webView;
    }
}

void RunWebViewManager::onCleanWebViews(RunWebView *webView){
    setCurrentWebView(webView);
    // 清理webView
    QVector<RunWebView *> inUseWebViews;

    inUseWebViews.append(m_currentWebView);
    for (int i = 0; i < m_helper->loops.length(); ++i) {
        RunWebView *webView =  m_helper->loops[i]->carryCurrentWebView;
        if(webView!=nullptr){
            inUseWebViews.append(webView);
        }
    }
    for (int i = 0; i < mWebViews.length(); ++i) {
        RunWebView *webView = mWebViews[i];

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
