#include "RunWebview.h"
#include "RunWebviewManager.h"
#include "RunWebpage.h"
#include <QTimer>
#include <QDebug>

RunWebView::RunWebView(RunWebViewManager *webviewManager,QWidget *parent) : QWebEngineView(parent),
    mWebviewManager(webviewManager){

    delete page();
    this->setPage(new RunWebPage(mWebviewManager->profile));


    QTimer *timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,[this,timer](){
        qDebug()<<"RunWebView::timeout 10s";
        timer->stop();
        this->reload();
    });
    connect(this, &QWebEngineView::loadStarted, this,[this,timer](){
        qDebug()<<"RunWebView::loadStarted.......";
        m_injectCheckTimes = 0;
        mWebviewManager->pageIsFinished = false;

        timer->start(10000);

    });
    connect(this, &QWebEngineView::loadFinished,this,[this,timer](bool){
        qDebug()<<"RunWebView::loadFinished.......";
        timer->stop();
        injectJavascriptCheck();

    });
    connect(this, &QWebEngineView::urlChanged, this,[this](const QUrl &url){
        if(url.url()!=""){
            emit mWebviewManager->webViewNotifyUrlChanged(url.url());
        }
    });

}
RunWebView::~RunWebView(){
    qDebug()<<"RunWebView::~RunWebView()";
    delete page();

}
void RunWebView::injectJavascriptCheck(){
    if(m_injectCheckTimes>20){
        // 注入检测超过指定次数，可能该网页已不能访问，标记加载完整
        mWebviewManager->pageIsFinished = true;

    }else {
       page()->runJavaScript("injectJavascriptCheck()", [this](const QVariant &v) {
           if(v.toString()=="success"){
                mWebviewManager->pageIsFinished = true;
           }else {
                qDebug()<<"RunWebView::injectJavascriptCheck m_injectCheckTimes="<<m_injectCheckTimes;
                m_injectCheckTimes +=1;
                QTimer::singleShot(m_injectCheckTimes*50,this,[this](){
                    injectJavascriptCheck();
                });
           }
       });
    }
}
QWebEngineView *RunWebView::createWindow(QWebEnginePage::WebWindowType type){
    qDebug()<<"RunWebView::createWindow"<<type<<page()<<this->url().url();

    if(type == QWebEnginePage::WebBrowserTab){
        return mWebviewManager->createWebView();
    }
    return this;
}
