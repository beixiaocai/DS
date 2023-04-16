#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class Tab;
struct MVersion;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Tab *tabWidget;
    void initMenu();
    void initSettings();

signals:
private slots:
    void onCheckVersion(bool state,QString &msg,MVersion &version);
};

#endif // MAINWINDOW_H
