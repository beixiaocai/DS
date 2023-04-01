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
    QWidget        *rightDirectStartWidget;
    QWidget        *rightTimedStartWidget;
    QCheckBox      *rightLoopCb; // 是否开启循环采集
    QSpinBox       *rightLoopIntervalSpin;//循环间隔时长（单位分钟）
    QSpinBox       *rightLoopSingleSpin;//单次运行时长（单位分钟）
    void initLeftUi();
    void initRightDirectStartUi();
    void initRightTimedStartUi();

signals:

public slots:
};

#endif // STARTUPDIALOGH_H
