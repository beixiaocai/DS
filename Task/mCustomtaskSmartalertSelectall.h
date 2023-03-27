#ifndef M_CUSTOMTASK_SMARTALERT_SELECTALL_H
#define M_CUSTOMTASK_SMARTALERT_SELECTALL_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QPushButton;
class QListWidget;
QT_END_NAMESPACE;

class MCustomTaskSmartAlertSelectAll : public QWidget
{
    Q_OBJECT
public:
    explicit MCustomTaskSmartAlertSelectAll(QWidget *parent,const QStringList nameItems);
    QPushButton *cancelButton;
    void setFieldNameList(const QStringList nameList);

private:
    QListWidget *listWidget;

signals:
    void clickedItem(const QString &name);

public slots:
};

#endif // M_CUSTOMTASK_SMARTALERT_SELECTALL_H
