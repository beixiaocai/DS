#ifndef SETTINGSPROXYDIALOG_H
#define SETTINGSPROXYDIALOG_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QCheckBox;
class QLineEdit;
QT_END_NAMESPACE;
class ComSpinWidget;

class SettingsProxyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsProxyDialog(QWidget *parent);

private:
    QVBoxLayout *boxLayout;
    void initProxyUi();// 初始化手动代理
    QCheckBox *openCb; // 是否开启代理服务器
    QLineEdit *ipLine;
    QLineEdit *portLine;
    QLineEdit *uLine;
    QLineEdit *pLine;
    void initBottomUi();

signals:

};

#endif // SETTINGSPROXYDIALOG_H
