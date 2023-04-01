#ifndef STARTUPBOX_H
#define STARTUPBOX_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QGridLayout;
QT_END_NAMESPACE;

class StartupBox : public QWidget
{
    Q_OBJECT
public:
    explicit StartupBox(QWidget *parent = nullptr);


    QWidget *gWidget;
    QGridLayout *gLayout;
    int rowStart = 0;

private:


signals:

public slots:
};

#endif // STARTUPBOX_H
