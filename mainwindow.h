#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class Tab;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Tab *tabWidget;
    void initMenu();
    void initData();

signals:

};

#endif // MAINWINDOW_H
