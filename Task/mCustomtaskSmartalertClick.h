#ifndef M_CUSTOMTASK_SMARTALERT_ITEM_WIDGET_H
#define M_CUSTOMTASK_SMARTALERT_ITEM_WIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QPushButton;
QT_END_NAMESPACE;

class MCustomTaskSmartAlertClick : public QWidget
{
    Q_OBJECT
public:
    explicit MCustomTaskSmartAlertClick(QWidget *parent,const QStringList nameItems);
    QPushButton *cancelButton;
signals:
    void clickedItem(const QString &name);
public slots:
};

#endif // M_CUSTOMTASK_SMARTALERT_ITEM_WIDGET_H
