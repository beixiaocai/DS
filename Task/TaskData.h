#ifndef TASKDATA_H
#define TASKDATA_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QTableWidget;
class QToolButton;
class QLabel;
QT_END_NAMESPACE;
class ComLoadingLabel;


class TaskData : public QWidget
{
    Q_OBJECT
public:
    explicit TaskData(const QString &taskName,const QString &taskCode,QWidget *parent);
    ~TaskData();
private:
    QVBoxLayout *boxLayout;
    void initMenuUi();
    ComLoadingLabel *loadingLabel;

    void initTableWidget();
    QTableWidget* tableWidget;
    QLabel *pageLabel;
    QLabel *pageLogLabel;
    void updateTurnPage(int num,bool isFinal);
private:
    QStringList m_tbfields;

    void getData(int p = 1);
    int  m_currentPage = 1;   // 当前页的页数
    int  m_totalPage = 0; // 当前总页数

private:
    QString m_taskName;
    QString m_taskCode;
signals:

};

#endif // TASKDATA_H
