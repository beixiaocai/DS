#ifndef MTABWIDGET_H
#define MTABWIDGET_H
#include <QTabWidget>
struct MTask;
class Tab : public  QTabWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent);

    void createIndex();    // 只能最多创建一个
    void createSettings(); // 只能最多创建一个
    void createAbout(); // 只能最多创建一个
    void createTask(const QString address=nullptr);// 允许创建多个
    void createTaskManage();// 只能最多创建一个

private:
    void setTask(MTask *task);
    QString formatTabName(const QString &name);

signals:

public slots:
    void onCloseTab(int index);

};

#endif // MTABWIDGET_H
