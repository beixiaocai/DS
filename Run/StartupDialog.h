#ifndef STARTUPDIALOGH_H
#define STARTUPDIALOGH_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QStackedWidget;
class QRadioButton;
class QCheckBox;
class QSpinBox;
QT_END_NAMESPACE;
struct MTask;

class StartupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartupDialog(MTask *task);
    ~StartupDialog();

private:
    MTask *mTask;
private:
    QHBoxLayout    *boxHLayout;
    QPushButton    *leftDirectStartBtn;
    QPushButton    *leftTimedStartBtn;


    QStackedWidget *rightStackedWidget;

    QWidget        *rightDirectStartWidget;//直接启动
    QCheckBox      *rightDirectTracelessBrowserCb;//直接启动是否开启无痕浏览器


    QWidget        *rightTimedStartWidget;//定时启动
    QCheckBox      *rightTimedTracelessBrowserCb;//定时启动是否开启无痕浏览器
    QSpinBox       *rightTimedLoopIntervalSpin; //定时启动循环间隔时长（单位分钟）
    QSpinBox       *rightTimedLoopSingleSpin;   //定时启动单次运行时长（单位分钟）
    void initLeftUi();
    void initRightDirectStartUi();
    void initRightTimedStartUi();

signals:

public slots:
};

#endif // STARTUPDIALOGH_H
