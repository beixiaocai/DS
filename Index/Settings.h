#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
QT_END_NAMESPACE;

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent);
private:
    QVBoxLayout *boxLayout;

    void initSettingsUi();


signals:


};

#endif // SETTINGS_H
