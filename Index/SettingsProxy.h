#ifndef SETTINGSPROXY_H
#define SETTINGSPROXY_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QCheckBox;
class QLineEdit;
QT_END_NAMESPACE;
class ComSpinWidget;

class SettingsProxy : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsProxy(QWidget *parent);

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

#endif // SETTINGSPROXY_H
