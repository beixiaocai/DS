#include "RunSettings.h"
#include "RunWebview.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "constant.h"
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



RunSettings::RunSettings(RunHelper *helper,QObject *parent) : m_helper(helper),QObject(parent){


    connect(this,&RunSettings::changeWebView,this,&RunSettings::onChangeWebView);

    profile =new QWebEngineProfile(this);
//    profile =QWebEngineProfile::defaultProfile();
    profile->setHttpCacheType(QWebEngineProfile::HttpCacheType::NoCache);
    profile->setHttpCacheMaximumSize(0);
    profile->setHttpUserAgent(m_helper->task->defaultUserAgent);

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

    settings->setAttribute(QWebEngineSettings::AllowRunningInsecureContent,m_helper->task->isBrowserAllowRunningInsecureContent);
    settings->setAttribute(QWebEngineSettings::AutoLoadImages,m_helper->task->isBrowserAutoLoadImages);
    settings->setAttribute(QWebEngineSettings::AutoLoadIconsForPage,m_helper->task->isBrowserAutoLoadIconsForPage);
    settings->setAttribute(QWebEngineSettings::PluginsEnabled,m_helper->task->isBrowserPluginsEnabled);

}
RunSettings::~RunSettings(){
    if(m_reusedWebView!=nullptr){
        delete m_reusedWebView;
    }
    for (int i = 0; i < m_initdWebViews.length(); ++i) {
        delete m_initdWebViews[i];
    }
    m_initdWebViews.clear();
}

void RunSettings::setCurrentWebView(RunWebView *webView){

    m_currentWebView = webView;
}
RunWebView * RunSettings::getCurrentWebView(){

    return m_currentWebView;
}

RunWebView * RunSettings::createWebView(){
    if(m_reusedWebView==nullptr){
        RunWebView *webView = new RunWebView(this,(QWidget*)this->parent());
        m_initdWebViews.append(webView);

        emit changeWebView(webView);
        return webView;
    }else {
        RunWebView *webView = m_reusedWebView;
        m_reusedWebView = nullptr;
        m_initdWebViews.append(webView);

        emit changeWebView(webView);
        return webView;
    }
}

void RunSettings::startJob(){

}
void RunSettings::stopJob(){

}



void RunSettings::onChangeWebView(RunWebView *webView){
    setCurrentWebView(webView);
    // 清理webView
    QVector<RunWebView *> useingWebViews;

    useingWebViews.append(m_currentWebView);

    for (int i = 0; i < m_helper->loops.length(); ++i) {
        RunWebView *webView =  m_helper->loops[i]->carryCurrentWebView;
        if(webView!=nullptr){
            useingWebViews.append(webView);
        }
    }

    for (int i = 0; i < m_initdWebViews.length(); ++i) {
        RunWebView *webView = m_initdWebViews[i];

        bool isDelete = true;
        for (int j = 0; j < useingWebViews.length(); ++j) {
            if(useingWebViews.at(j)==webView){
                isDelete = false;
                break;
            }
        }
        if(isDelete){
            webView->page()->profile()->clearHttpCache();
            webView->page()->profile()->clearAllVisitedLinks();

            m_initdWebViews.remove(i);
            if(m_reusedWebView==nullptr){
                m_reusedWebView = webView;
            }else {
                webView->disconnect();
                delete webView;
            }
        }
    }
}
