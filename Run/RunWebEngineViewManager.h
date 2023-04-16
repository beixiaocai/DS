#ifndef RUNWEBENGINEVIEWMANAGER_H
#define RUNWEBENGINEVIEWMANAGER_H

#include <QObject>

QT_BEGIN_NAMESPACE;
class QWebEngineProfile;
QT_END_NAMESPACE;
class RunDialog;
class RunHelper;
class RunWebEngineView;

class RunWebEngineViewManager : public QObject{
    Q_OBJECT

public:
    friend class RunWebEngineView;

    explicit RunWebEngineViewManager(RunDialog *runDialog);
//    RunWebEngineViewManager() = delete;
    ~RunWebEngineViewManager();

public:
    bool pageIsFinished = false;

    RunWebEngineView * getCurrentWebView();
    void setCurrentWebView(RunWebEngineView *);
    QWebEngineProfile* profile();
private:
    RunHelper *mHelper;
    QWebEngineProfile *mProfile;
    QVector<RunWebEngineView *> mWebViews;// 所有使用的webView
    RunWebEngineView *m_reusedWebView = nullptr;// 复用的webview，不放入m_initdWebViews
    RunWebEngineView *m_currentWebView = nullptr;// 当前显示的webView

    RunWebEngineView * createWebView();
signals:
    void webViewNotifyUrlChanged(const QString &url);
    void cleanWebViews(RunWebEngineView *webView);
public slots:
    void onCleanWebViews(RunWebEngineView *webView);
};

#endif // RUNWEBENGINEVIEWMANAGER_H
