#include "SettingsProxyDialog.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "style.h"
#include "constant.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
#include <QtCore5Compat>
#endif


SettingsProxyDialog::SettingsProxyDialog(QWidget *parent) : QDialog(parent)
{
    setFixedSize(520,320);

    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".SettingsProxyDialog{background-color:rgb(255,255,255);}");
    setWindowTitle("设置代理");

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);
    boxLayout->setSpacing(0);

    initProxyUi();
    boxLayout->addStretch(10);
    boxLayout->addWidget(new ComLineWidget(this));
    initBottomUi();


}
void SettingsProxyDialog::initProxyUi(){

    QWidget *proxyWidget = new QWidget(this);
    QVBoxLayout *proxyLayout = new QVBoxLayout(proxyWidget);
    proxyLayout->setContentsMargins(20,20,20,0);
    proxyLayout->setSpacing(0);

    QLabel *titleLabel = new QLabel(proxyWidget);
    titleLabel->setStyleSheet(m_stylesheet_QLabel14);
    titleLabel->setText("手动设置代理");

    openCb = new QCheckBox(proxyWidget);
    openCb->setStyleSheet(m_stylesheet_QCheckBox);
    openCb->setText("开启代理服务");


    // 代理地址start
    QWidget *proxyValueWidget = new QWidget(proxyWidget);
    QHBoxLayout *proxyValueHLayout = new QHBoxLayout(proxyValueWidget);
    proxyValueHLayout->setContentsMargins(0,0,0,0);
    proxyValueHLayout->setSpacing(0);

    ipLine = new QLineEdit(proxyValueWidget);
    ipLine->setFixedSize(140,30);
    ipLine->setPlaceholderText("IP");
    ipLine->setStyleSheet(m_stylesheet_QLineEdit);
    ipLine->setClearButtonEnabled(true);
    ipLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    portLine = new QLineEdit(proxyValueWidget);
    portLine->setFixedSize(140,30);
    portLine->setPlaceholderText("端口");
    portLine->setStyleSheet(m_stylesheet_QLineEdit);
    portLine->setClearButtonEnabled(true);
    portLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);


    proxyValueHLayout->addWidget(ipLine);proxyValueHLayout->addSpacing(5);
    proxyValueHLayout->addWidget(portLine);proxyValueHLayout->addStretch(10);
    // 代理地址end

    // 代理验证 start
    QWidget *proxyAuthWidget = new QWidget(proxyWidget);
    QHBoxLayout *proxyAuthHLayout = new QHBoxLayout(proxyAuthWidget);
    proxyAuthHLayout->setContentsMargins(0,0,0,0);
    proxyAuthHLayout->setSpacing(0);

    uLine = new QLineEdit(proxyAuthWidget);
    uLine->setFixedSize(140,30);
    uLine->setPlaceholderText("账户");
    uLine->setStyleSheet(m_stylesheet_QLineEdit);
    uLine->setClearButtonEnabled(true);
    uLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    pLine = new QLineEdit(proxyAuthWidget);
    pLine->setFixedSize(140,30);
    pLine->setPlaceholderText("密码");
    pLine->setStyleSheet(m_stylesheet_QLineEdit);
    pLine->setClearButtonEnabled(true);
    pLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    QLabel * authLabel = new QLabel(proxyAuthWidget);
    authLabel->setText("均为非必填");

    proxyAuthHLayout->addWidget(uLine);proxyAuthHLayout->addSpacing(5);
    proxyAuthHLayout->addWidget(pLine);proxyAuthHLayout->addSpacing(5);
    proxyAuthHLayout->addWidget(authLabel);
    proxyAuthHLayout->addStretch(10);
    // 代理验证 end


    proxyLayout->addWidget(titleLabel);
    proxyLayout->addSpacing(10);
    proxyLayout->addWidget(new ComLineWidget(this));
    proxyLayout->addSpacing(10);
    proxyLayout->addWidget(openCb);
    proxyLayout->addSpacing(20);
    proxyLayout->addWidget(proxyValueWidget);
    proxyLayout->addSpacing(10);
    proxyLayout->addWidget(proxyAuthWidget);
    proxyLayout->addStretch(10);


    QSettings settings;

    // 设置默认值
    openCb->setChecked(settings.value(SETTINGS_KEY_OPEN_PROXY).toBool());
    ipLine->setText(settings.value(SETTINGS_KEY_PROXY_IP).toString());
    portLine->setText(settings.value(SETTINGS_KEY_PROXY_PORT).toString());
    uLine->setText(settings.value(SETTINGS_KEY_PROXY_USERNAME).toString());
    pLine->setText(settings.value(SETTINGS_KEY_PROXY_PASSWORD).toString());


    boxLayout->addWidget(proxyWidget);
}
void SettingsProxyDialog::initBottomUi(){


    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(50);
    QHBoxLayout *bottomHLayout = new QHBoxLayout(bottomWidget);
    bottomHLayout->setContentsMargins(0,0,0,0);
    bottomHLayout->setSpacing(0);

    QPushButton *okBtn = new QPushButton(bottomWidget);
    okBtn->setText("保存");
    okBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setFixedSize(90,32);
    connect(okBtn,&QPushButton::clicked,this,[this](){
        QSettings settings;
        QString ip = ipLine->text().trimmed();
        quint16 port = portLine->text().trimmed().toUInt();
        QString username = uLine->text().trimmed();
        QString password = pLine->text().trimmed();

        if(openCb->checkState()==Qt::CheckState::Checked){
            QRegExp reip("^((25[0-5]|2[0-4]\\d|[1]{1}\\d{1}\\d{1}|[1-9]{1}\\d{1}|\\d{1})($|(?!\\.$)\\.)){4}$");
            if(reip.exactMatch(ip)){
                if(port != 0){
                    QNetworkProxy networkProxy;
//                    QString ip = "10.21.143.210";
//                    quint16 port = 3128;
                    networkProxy.setType(QNetworkProxy::HttpProxy);
                    networkProxy.setHostName(ip);
                    networkProxy.setPort(port);

                    settings.setValue(SETTINGS_KEY_OPEN_PROXY,true);
                    settings.setValue(SETTINGS_KEY_PROXY_IP,ip);
                    settings.setValue(SETTINGS_KEY_PROXY_PORT,port);
                    if(username.length()>0 && password.length()>0){
                        settings.setValue(SETTINGS_KEY_PROXY_USERNAME,username);
                        settings.setValue(SETTINGS_KEY_PROXY_PASSWORD,password);

                    }

                    QNetworkProxy::setApplicationProxy(networkProxy);

                    ComMessageBox::success(this,"代理设置成功");
                }else{
                    ComMessageBox::error(this,"端口不合法");
                }
            }else{
                ComMessageBox::error(this,"ip地址不合法");
            }

        }else{
            settings.setValue(SETTINGS_KEY_OPEN_PROXY,false);
            settings.setValue(SETTINGS_KEY_PROXY_IP,ip);
            settings.setValue(SETTINGS_KEY_PROXY_PORT,port);
            settings.setValue(SETTINGS_KEY_PROXY_USERNAME,username);
            settings.setValue(SETTINGS_KEY_PROXY_PASSWORD,password);

            QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::ProxyType::NoProxy));
            QNetworkProxyFactory::setUseSystemConfiguration(false);

            ComMessageBox::success(this,"代理已关闭");
        }

    });

    QPushButton *cancelBtn = new QPushButton(bottomWidget);
    cancelBtn->setText("取消");
    cancelBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    cancelBtn->setCursor(Qt::PointingHandCursor);
    cancelBtn->setFixedSize(90,32);
    connect(cancelBtn,&QPushButton::clicked,this,[this](){
        this->close();
    });



    bottomHLayout->addStretch(10);
    bottomHLayout->addWidget(okBtn);
    bottomHLayout->addSpacing(20);
    bottomHLayout->addWidget(cancelBtn);
    bottomHLayout->addSpacing(20);


    boxLayout->addWidget(bottomWidget);

}
