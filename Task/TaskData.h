#ifndef TASKDATA_H
#define TASKDATA_H

#include <QWidget>
#include <QHash>
#include <QVector>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QToolButton;
class QLabel;
class QTableWidget;
class QListWidget;
class QListWidgetItem;
QT_END_NAMESPACE;
class ComLoadingLabel;
class TaskDataDownloadManager;

class TaskData : public QWidget
{
    Q_OBJECT
public:
    explicit TaskData(QWidget *parent,const QString &taskName,const QString &taskCode);
    ~TaskData();
private:
    QVBoxLayout *boxLayout;
    void initMenuUi();
    ComLoadingLabel *loadingLabel;

    void initTableUi();
    QTableWidget* tableWidget;
    void initBottomUi();
    QListWidget *logListWidget;
    QVector<QListWidgetItem *> logListWidgetItems_clear;
    QLabel *pageLabel;
    QLabel *pageLogLabel;

    void updatePageData(int num,bool isShowLastPage);
private:
    QString mTaskName;
    QString mTaskCode;
    QStringList mFields;//任务采集数据的字段
    QHash<QString,TaskDataDownloadManager *> mDownloadManagerH;

    void getData(int p = 1);
    int  mCurPage = 1;   // 当前页的页数
    int  mTotalPage = 0; // 当前总页数

signals:

};

#endif // TASKDATA_H
