#ifndef RUNWEBENGINEVIEW_H
#define RUNWEBENGINEVIEW_H

#include <QWebEngineView>
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
#include <QtCore5Compat>
#endif

class RunWebEngineViewManager;

class RunWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit RunWebEngineView(RunWebEngineViewManager *webViewManager,QWidget *parent);
    ~RunWebEngineView();
private:
    RunWebEngineViewManager *mWebViewManager;
    int  mInjectCheckCount = 0;
    void injectJavascriptCheck();

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
signals:


public slots:

};
#endif // RUNWEBENGINEVIEW_H
