#include "TaskWebView.h"
#include "TaskWebViewBridge.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "constant.h"
#include "TaskWebPage.h"
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScriptCollection>
#include <QWebEngineScript>
#include <QApplication>
#include <QStyle>
#include <QsLog.h>

TaskWebView::TaskWebView(QWidget *parent,MTask *task) : QWebEngineView(parent),
    loadProgress(100){
    QLOG_INFO() <<"TaskWebView::TaskWebView()";

    //  setZoomFactor(zoomFactor() - 0.3);
//    m_profile =new QWebEngineProfile(this);
    m_profile =QWebEngineProfile::defaultProfile();
    m_profile->setHttpUserAgent(task->defaultUserAgent);
    bridge = new TaskWebViewBridge(this);
    delete this->page();
    this->setPage(createWebPage());
    injectJavascript(true);

}
TaskWebView::~TaskWebView(){
    QLOG_INFO() <<"TaskWebView::~TaskWebView()";
    for (int i = 0; i < m_pages.length(); ++i) {
        delete m_pages[i];
    }
}
void TaskWebView::injectJavascript(bool inject){
    if(inject){
        QString sourceCode = QString( "let qj = document.createElement('script');"
        "qj.appendChild(document.createTextNode('%1'));qj.type = 'text/javascript';"
        "let tj = document.createElement('script');"
        "tj.appendChild(document.createTextNode('%2'));tj.type = 'text/javascript';"
        "document.body.appendChild(qj);document.body.appendChild(tj);")
          .arg(Database::getInstance()->read_file(JS_QWEBCHANNEL_FILE).replace("\r"," ").replace("\n"," ").replace("\t"," "))
          .arg(Database::getInstance()->read_file(JS_CUSTOMTASK_FILE).replace("\r"," ").replace("\n"," ").replace("\t"," "));

        QWebEngineScript script;
        m_profile->scripts()->clear();
        script.setSourceCode(sourceCode);
        m_profile->scripts()->insert(script);
    }else {

        m_profile->scripts()->clear();
    }
}

QWebEngineView *TaskWebView::createWindow(QWebEnginePage::WebWindowType type){
    QLOG_INFO()<<"TaskWebView::createWindow"<<type<<page()<<this->url().url();

    if(type == QWebEnginePage::WebBrowserTab){
        TaskWebView::setPage(createWebPage());

    }
    return this;
}

TaskWebPage * TaskWebView::createWebPage(){
    TaskWebPage *page = new TaskWebPage(this,m_profile);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("bridge",bridge);
    page->setWebChannel(channel);

    m_pages.append(page);

    return page;
}


QIcon TaskWebView::favIcon() const
{
    QIcon favIcon = icon();
    if (!favIcon.isNull()){
       return favIcon;
    }

    if (loadProgress < 0) {
        static QIcon errorIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserStop));
        return errorIcon;
    } else if (loadProgress < 100) {
        static QIcon loadingIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
        return loadingIcon;
    } else {
        static QIcon defaultIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogContentsView));
        return defaultIcon;
    }
}
