#include "RunWebEngineView.h"
#include "RunWebEngineViewManager.h"
#include "RunWebEnginePage.h"
#include <QTimer>
#include <QsLog.h>

RunWebEngineView::RunWebEngineView(RunWebEngineViewManager *webViewManager,QWidget *parent) : QWebEngineView(parent),
    mWebViewManager(webViewManager){
    QLOG_INFO()<<"RunWebEngineView::RunWebEngineView()";

    delete page();
    RunWebEnginePage *newPage = new RunWebEnginePage(mWebViewManager->profile(),this);
    this->setPage(newPage);

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[this,timer](){
        QLOG_INFO()<<"RunWebEngineView::timeout 10s";
        timer->stop();
        this->reload();
    });
    connect(this, &QWebEngineView::loadStarted, this,[this,timer](){
        QLOG_INFO()<<"RunWebEngineView::loadStarted.......";
        mInjectCheckCount = 0;
        mWebViewManager->pageIsFinished = false;
        timer->start(10000);

    });
    connect(this, &QWebEngineView::loadFinished,this,[this,timer](bool){
        QLOG_INFO()<<"RunWebEngineView::loadFinished.......";
        timer->stop();
        injectJavascriptCheck();

    });
    connect(this, &QWebEngineView::urlChanged, this,[this](const QUrl &url){
        if(url.url()!=""){
            emit mWebViewManager->webViewNotifyUrlChanged(url.url());
        }
    });

}
RunWebEngineView::~RunWebEngineView(){
    QLOG_INFO()<<"RunWebEngineView::~RunWebEngineView()";
    delete page();

}
void RunWebEngineView::injectJavascriptCheck(){
    if(mInjectCheckCount>20){
        // 注入检测超过指定次数，可能该网页已不能访问，标记加载完整
        mWebViewManager->pageIsFinished = true;

    }else {
       page()->runJavaScript("injectJavascriptCheck()", [this](const QVariant &v) {
           if(v.toString()=="success"){
                mWebViewManager->pageIsFinished = true;
           }else {
                QLOG_INFO()<<"RunWebEngineView::injectJavascriptCheck mInjectCheckCount="<<mInjectCheckCount;
                mInjectCheckCount +=1;
                QTimer::singleShot(mInjectCheckCount*50,this,[this](){
                    injectJavascriptCheck();
                });
           }
       });
    }
}
QWebEngineView *RunWebEngineView::createWindow(QWebEnginePage::WebWindowType type){
    QLOG_INFO()<<"RunWebEngineView::createWindow"<<type<<page()<<this->url().url();

    if(type == QWebEnginePage::WebBrowserTab){
        return mWebViewManager->createWebView();
    }
    return this;
}
