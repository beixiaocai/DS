#ifndef RUNMESSAGE_H
#define RUNMESSAGE_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLabel;
QT_END_NAMESPACE;


class RunMessage : public QDialog
{
    Q_OBJECT
public:
    explicit RunMessage(QWidget *parent);
    enum MessageType{
        SUCCESS,
        ERROR,
        WARNING
    };
    void showMessage(MessageType type,const QString &summary);
private:
    QVBoxLayout *boxLayout;
    QLabel *iconLabel;
    QLabel *titleDesc;
    QLabel *summaryDesc;

    void initBottomUi();
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void closeMessage();
public slots:
};

#endif // RUNMESSAGE_H
