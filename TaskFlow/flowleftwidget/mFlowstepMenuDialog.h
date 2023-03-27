#ifndef MFLOWSTEPMENUDIALOG_H
#define MFLOWSTEPMENUDIALOG_H


#include <QDialog>

QT_BEGIN_NAMESPACE;
class QPushButton;
QT_END_NAMESPACE;

class MFlowStepMenuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MFlowStepMenuDialog(QWidget *parent);
    QPushButton *deleteBtn;
signals:

public slots:
};

#endif // MFLOWSTEPMENUDIALOG_H
