#ifndef M_CUSTOMTASK_SMARTALERT_SUCCESS_H
#define M_CUSTOMTASK_SMARTALERT_SUCCESS_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QListWidget;
class QLabel;
QT_END_NAMESPACE;
class MCustomTaskSmartAlertSuccess : public QWidget
{
    Q_OBJECT
public:
    explicit MCustomTaskSmartAlertSuccess(QWidget *parent);

    void showMsg(const QString titleText,bool isShowList=false);
    void showList(const QStringList nameList);

private:
    QLabel *titleLabel;
    QListWidget *listWidget;

signals:

public slots:
};

#endif // M_CUSTOMTASK_SMARTALERT_SUCCESS_H
