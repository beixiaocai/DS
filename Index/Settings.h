#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QLabel;
QT_END_NAMESPACE;
class ComLoadingLabel;

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent);
    ~Settings();
private:
    QVBoxLayout *boxLayout;
    ComLoadingLabel *loadingLabel;
    QLabel          *loadingMsgLabel;

    void initUi();


signals:


};

#endif // SETTINGS_H
