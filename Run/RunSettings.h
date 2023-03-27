#ifndef RUNSETTINGS_H
#define RUNSETTINGS_H

#include <QObject>

QT_BEGIN_NAMESPACE;
class QWebEngineProfile;
QT_END_NAMESPACE;
class RunHelper;
class RunWebView;

class RunSettings : public QObject{
    Q_OBJECT
public:
    explicit RunSettings(RunHelper *helper,QObject *parent);
    ~RunSettings();

public:
    QWebEngineProfile *profile;
    bool pageIsFinished = false;
    RunWebView * createWebView();
    RunWebView * getCurrentWebView();
    void setCurrentWebView(RunWebView *);

    void startJob();// 确定开始job
    void stopJob();// 确定停止job


private:
    RunHelper *m_helper;
    QVector<RunWebView *> m_initdWebViews;// 所有使用的webView
    RunWebView *m_reusedWebView = nullptr;// 复用的webview，不放入m_initdWebViews
    RunWebView *m_currentWebView = nullptr;// 当前显示的webView


signals:
    void webViewNotifyUrlChanged(const QString &url);
    void changeWebView(RunWebView *webView);
public slots:
    void onChangeWebView(RunWebView *webView);
};

#endif // RUNSETTINGS_H
