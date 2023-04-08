#ifndef COMOPTIONSBOX_H
#define COMOPTIONSBOX_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QGridLayout;
QT_END_NAMESPACE;

class ComOptionsBox : public QWidget
{
    Q_OBJECT
public:
    explicit ComOptionsBox(QWidget *parent,const QString &name);

    QWidget *gWidget;
    QGridLayout *gLayout;
    int rowStart = 0;

private:


signals:

public slots:
};

#endif // COMOPTIONSBOX_H
