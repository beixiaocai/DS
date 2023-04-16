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
class TaskWebEngineView;
class PageCustomTaskFlow;
class TaskSmartAlertDialog;
class TaskBrowserMenu;
struct MTask;

class TaskDo : public QWidget
{
    Q_OBJECT

public:
    explicit TaskDo(QWidget *parent,MTask *task);
    ~TaskDo();

     void startLoad(int msec=50);

private:
    MTask *mTask;
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

    TaskBrowserMenu *browserMenu;// 浏览模式的菜单
    TaskWebEngineView *webView;
    TaskSmartAlertDialog *mSmartAlertDialog;
    void initSmartAlertView(int msec);


    void loadUrl(QString url);

signals:
    void notifyChangeTabName(const QString &name);
    void notifyChangeTabIcon(const QIcon &icon);
    void notifySettings();


};

#endif // TASKDO_H
