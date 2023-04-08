#ifndef TASKSETTINGS_H
#define TASKSETTINGS_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QRadioButton;
class QCheckBox;
QT_END_NAMESPACE;
struct MTask;

class TaskSettings : public QWidget
{
    Q_OBJECT
public:
    explicit TaskSettings(QWidget *parent,MTask *task);
    ~TaskSettings();
private:
    MTask *m_task;

    QVBoxLayout *boxLayout;

    void addTitle(const QString &title);
    QCheckBox * addSettingCheckBox(const QString &name,const QString &tip=NULL);
    void initSettingUi();
    void initMenuUi();


signals:
     void notifyBack();
public slots:

};

#endif // TASKSETTINGS_H
