#include "TaskWebEngineView.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "constant.h"
#include "TaskWebEnginePage.h"
#include "TaskWebEngineBridge.h"
#include "TaskWebEngineProfile.h"
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScriptCollection>
#include <QWebEngineScript>
#include <QApplication>
#include <QStyle>
#include <QsLog.h>

TaskWebEngineView::TaskWebEngineView(QWidget *parent,MTask *task) : QWebEngineView(parent),
    loadProgress(100){
    QLOG_INFO() <<"TaskWebEngineView::TaskWebEngineView()";

    TaskWebEngineProfile::getInstance()->profile()->setHttpUserAgent(task->defaultUserAgent);
    bridge = new TaskWebEngineBridge(this);
    delete this->page();
    this->setPage(createPage());
    injectJavascript(true);

}
TaskWebEngineView::~TaskWebEngineView(){
    QLOG_INFO() <<"TaskWebEngineView::~TaskWebEngineView()";
    for (int i = 0; i < mPages.length(); ++i) {
        delete mPages[i];
    }
}
void TaskWebEngineView::injectJavascript(bool inject){
    if(inject){
        QString sourceCode = QString( "let qj = document.createElement('script');"
        "qj.appendChild(document.createTextNode('%1'));qj.type = 'text/javascript';"
        "let tj = document.createElement('script');"
        "tj.appendChild(document.createTextNode('%2'));tj.type = 'text/javascript';"
        "document.body.appendChild(qj);document.body.appendChild(tj);")
          .arg(Database::getInstance()->read_file(JS_QWEBCHANNEL_FILE).replace("\r"," ").replace("\n"," ").replace("\t"," "))
          .arg(Database::getInstance()->read_file(JS_CUSTOMTASK_FILE).replace("\r"," ").replace("\n"," ").replace("\t"," "));

        QWebEngineScript script;
        TaskWebEngineProfile::getInstance()->profile()->scripts()->clear();
        script.setSourceCode(sourceCode);
        TaskWebEngineProfile::getInstance()->profile()->scripts()->insert(script);
    }else {

        TaskWebEngineProfile::getInstance()->profile()->scripts()->clear();
    }
}

QWebEngineView *TaskWebEngineView::createWindow(QWebEnginePage::WebWindowType type){
    QLOG_INFO()<<"TaskWebEngineView::createWindow"<<type<<page()<<this->url().url();

    if(type == QWebEnginePage::WebBrowserTab){
//        TaskWebEngineView::setPage(createPage());
        this->setPage(createPage());
    }
    return this;
}

TaskWebEnginePage * TaskWebEngineView::createPage(){
    TaskWebEnginePage *page = new TaskWebEnginePage(this,TaskWebEngineProfile::getInstance()->profile());
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("bridge",bridge);
    page->setWebChannel(channel);

    mPages.append(page);

    return page;
}


QIcon TaskWebEngineView::favIcon() const
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
