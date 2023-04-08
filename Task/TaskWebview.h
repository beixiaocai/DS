#ifndef TASKWEBVIEW_H
#define TASKWEBVIEW_H

#include <QWebEngineView>

class TaskWebPage;
class TaskWebViewBridge;
struct MTask;

class TaskWebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit TaskWebView(QWidget *parent,MTask *task);
    ~TaskWebView();
public:
    TaskWebViewBridge *bridge;
    int loadProgress;// 加载进度
    void injectJavascript(bool inject);
    QIcon favIcon() const;// 根据 loadProgress参数得到的一个icon

private:
    QWebEngineProfile *m_profile;
    QVector<TaskWebPage *> m_pages;

    TaskWebPage* createWebPage();
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
signals:


public slots:

};

#endif // TASKWEBVIEW_H
