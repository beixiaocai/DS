#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
QT_END_NAMESPACE;

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget *parent);
private:
    QVBoxLayout *boxLayout;

    void initTopUi();
    void initSoftwareUi();

signals:

};

#endif // ABOUT_H
