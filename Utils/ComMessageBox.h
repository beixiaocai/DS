#ifndef COMMESSAGEBOX_H
#define COMMESSAGEBOX_H

#include <QDialog>

class ComMessageBox : public QDialog
{
    Q_OBJECT
private:
    explicit ComMessageBox(bool success,const QString &text,QWidget *parent);

public:
    static void success(QWidget *parent,const QString &text);
    static void error(QWidget *parent,const QString &text);


signals:

};

#endif // COMMESSAGEBOX_H
