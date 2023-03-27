#ifndef M_CUSTOMTASK_SMARTALERT_ITEM_H
#define M_CUSTOMTASK_SMARTALERT_ITEM_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QPushButton;
class QToolButton;
QT_END_NAMESPACE;

class MCustomTaskSmartAlertItem : public QWidget
{
    Q_OBJECT

public:
    explicit MCustomTaskSmartAlertItem(QWidget *parent,QString name,QString tip );

    QPushButton*m_namePushButton;
    QToolButton *m_tipButton;

};

#endif // M_CUSTOMTASK_SMARTALERT_ITEM_H
