#ifndef TASKWEBENGINEVIEW_H
#define TASKWEBENGINEVIEW_H

#include <QWebEngineView>

class TaskWebEnginePage;
class TaskWebEngineBridge;
struct MTask;

class TaskWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit TaskWebEngineView(QWidget *parent,MTask *task);
    ~TaskWebEngineView();
public:
    TaskWebEngineBridge *bridge;
    int loadProgress;// 加载进度
    void injectJavascript(bool inject);
    QIcon favIcon() const;// 根据 loadProgress参数得到的一个icon

private:
    QVector<TaskWebEnginePage *> mPages;
    TaskWebEnginePage* createPage();
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
signals:


public slots:

};

#endif // TASKWEBENGINEVIEW_H
