#ifndef TASKDATAMENUDIALOG_H
#define TASKDATAMENUDIALOG_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QPushButton;
class QLineEdit;
class QSpinBox;
QT_END_NAMESPACE;

class TaskDataMenuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TaskDataMenuDialog(QWidget *parent,const QString &field,int fieldIndex);
    ~TaskDataMenuDialog();
public:
    QString field;
    int fieldIndex;
    QPushButton *downloadBtn;
    QLineEdit *fieldPrefixLine;
    QLineEdit *saveSuffixLine;//文件后缀
    QSpinBox *threadCountSpin;
    QSpinBox *threadIntervalSpin;//线程间隔时长
    QString saveDir;//文件夹
signals:

public slots:
};

#endif // TASKDATAMENUDIALOG_H
