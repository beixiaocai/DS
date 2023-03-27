#ifndef M_CUSTOMTASK_WEBVIEW_H
#define M_CUSTOMTASK_WEBVIEW_H

#include <QWebEngineView>

class WebPage;
class MCustomTaskBridge;
struct MTask;

class MCustomTaskWebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit MCustomTaskWebView(MTask *task,QWidget *parent);
    ~MCustomTaskWebView();

    MCustomTaskBridge *bridge;

    void injectJavascript(bool inject);
    QIcon favIcon() const;// 根据 loadProgress参数得到的一个icon

    int loadProgress;// 加载进度
private:
    QWebEngineProfile *m_profile;
    QVector<WebPage *> m_pages;

    WebPage* createWebPage();
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
signals:


public slots:

};

#endif // M_CUSTOMTASK_WEBVIEW_H
