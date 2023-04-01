#ifndef RUNWEBVIEWMANAGER_H
#define RUNWEBVIEWMANAGER_H

#include <QObject>

QT_BEGIN_NAMESPACE;
class QWebEngineProfile;
QT_END_NAMESPACE;
class RunDialog;
class RunHelper;
class RunWebView;

class RunWebViewManager : public QObject{
    Q_OBJECT

public:
    friend class RunWebView;

    explicit RunWebViewManager(RunDialog *runDialog);
//    RRunWebViewManager() = delete;
    ~RunWebViewManager();

public:
    QWebEngineProfile *profile;
    bool pageIsFinished = false;

    RunWebView * getCurrentWebView();
    void setCurrentWebView(RunWebView *);



private:
    RunHelper *m_helper;
    QVector<RunWebView *> mWebViews;// 所有使用的webView
    RunWebView *m_reusedWebView = nullptr;// 复用的webview，不放入m_initdWebViews
    RunWebView *m_currentWebView = nullptr;// 当前显示的webView

    RunWebView * createWebView();
signals:
    void webViewNotifyUrlChanged(const QString &url);
    void cleanWebViews(RunWebView *webView);
public slots:
    void onCleanWebViews(RunWebView *webView);
};

#endif // RUNWEBVIEWMANAGER_H
