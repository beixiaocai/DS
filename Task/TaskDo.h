#ifndef TASKDO_H
#define TASKDO_H
#include "QWidget"

QT_BEGIN_NAMESPACE;
class QStackedWidget;
class QVBoxLayout;
class QCheckBox;
class QLineEdit;
class QProgressBar;
QT_END_NAMESPACE;

class ComLoadingLabel;
class MCustomTaskWebView;
class PageCustomTaskFlow;
class TaskSmartalert;
class MCustomTaskBrowserMenu;
struct MTask;

class TaskDo : public QWidget
{
    Q_OBJECT

public:
    explicit TaskDo(MTask *task,QWidget *parent = nullptr);
    ~TaskDo();

     void startLoad(int msec=50);

private:
    MTask *m_task;
    QVBoxLayout *boxLayout;

    void initTopMenuUi();
    QLineEdit *nameLine;
    ComLoadingLabel *loadingLabel;

    void initFlowUi();
    QWidget *flowWidget;
    PageCustomTaskFlow *flow=nullptr;

    void initBottomUi();
    bool isFirstShowSmartAlert = true;//是否首次显示智能提示框
    QLineEdit *addrEdit;
    QAction   *addrFavAction;// 网址输入框图标
    QCheckBox *smartAlertCheck;
    QProgressBar *progressBar;

    MCustomTaskBrowserMenu *browserMenu;// 浏览模式的菜单
    MCustomTaskWebView *webView;
    TaskSmartalert *smartAlertView = nullptr;
    void initSmartAlertView(int msec);


    void loadUrl(QString url);

signals:
    void notifyChangeTabName(const QString &name);
    void notifyChangeTabIcon(const QIcon &icon);
    void notifySettings();


};

#endif // TASKDO_H
