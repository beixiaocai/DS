#ifndef M_CUSTOMTASK_SMARTALERT_XPATH_H
#define M_CUSTOMTASK_SMARTALERT_XPATH_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QHBoxLayout;
class QLabel;
QT_END_NAMESPACE;

class MCustomTaskSmartAlertXPath : public QWidget
{
    Q_OBJECT
public:
    explicit MCustomTaskSmartAlertXPath(QWidget *parent);

    void setXpath(const QString &xpath);
private:
    QHBoxLayout *boxLayout;
    QLabel *xpathLabel;
signals:

public slots:
};

#endif // M_CUSTOMTASK_SMARTALERT_XPATH_H
