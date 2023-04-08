#include "TaskSettings.h"
#include "Utils/database.h"
#include "Utils/models.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QScrollArea>
#include <QScrollBar>


TaskSettings::TaskSettings(QWidget *parent,MTask *task) : QWidget(parent),
    m_task(task)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    QScrollArea *mainScroll = new QScrollArea(this);
    mainScroll->setStyleSheet(m_stylesheet_QScrollArea);
    mainScroll->setVerticalScrollBar(new QScrollBar(mainScroll));

    QWidget *boxWidget = new QWidget(this);
    boxWidget->setAttribute(Qt::WA_StyledBackground,true);
    boxWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);}");
    mainScroll->setWidget(boxWidget);
    mainScroll->setWidgetResizable(true);
    mainLayout->addWidget(mainScroll);

    boxLayout = new QVBoxLayout(boxWidget);
    boxLayout->setContentsMargins(20,0,20,40);
    boxLayout->setSpacing(10);

    boxLayout->addSpacing(10);
    addTitle("采集设置");
    initSettingUi();
    boxLayout->addStretch(10);
    initMenuUi();
}
TaskSettings::~TaskSettings(){


}


void TaskSettings::addTitle(const QString &title){
    QWidget *menuWidget = new QWidget(this);
    QHBoxLayout *menuHLayout = new QHBoxLayout(menuWidget);
    menuHLayout->setContentsMargins(10,0,0,0);
    menuWidget->setFixedHeight(30);
    menuWidget->setStyleSheet(".QWidget{background-color:rgb(241,244,251);}");
    QLabel *titleLabel = new QLabel(menuWidget);
    titleLabel->setStyleSheet(".QLabel{font-family:Microsoft YaHei;font-size:14px;background-color: rgba(0,0,0,0);color:rgb(0,0,0);}");
    titleLabel->setText(title);
    menuHLayout->addWidget(titleLabel);

    boxLayout->addWidget(menuWidget);
}

QCheckBox *TaskSettings::addSettingCheckBox(const QString &name,const QString &tip){

    QWidget *widget = new QWidget(this);
    widget->setFixedHeight(40);
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(20,0,0,0);
    hLayout->setSpacing(0);
    QCheckBox * settingCb = new QCheckBox(widget);
    settingCb->setStyleSheet(m_stylesheet_QCheckBox);
    settingCb->setText(name);

    hLayout->addWidget(settingCb);

    if(tip!=""){
        QToolButton *hBtn = new QToolButton(widget);
        hBtn->setStyleSheet(m_stylesheet_QToolButton_empty);
        hBtn->setIcon(QIcon(":/res/images/icon/flow/help.png"));
        hBtn->setIconSize(QSize(30,30));
        hBtn->setCursor(Qt::PointingHandCursor);
        hBtn->setToolTip(tip);
        hLayout->addSpacing(5);
        hLayout->addWidget(hBtn);

    }
    hLayout->addStretch(10);

    boxLayout->addWidget(widget);

    return settingCb;
}
void TaskSettings::initSettingUi(){

    QCheckBox * autoLoadImagesCb = addSettingCheckBox("允许浏览器加载网页图片");
    QCheckBox * autoLoadIconsForPageCb = addSettingCheckBox("允许浏览器加载网页图标");
    QCheckBox * pluginsEnabledCb = addSettingCheckBox("允许浏览器加载插件");

    autoLoadImagesCb->setChecked(m_task->isBrowserAutoLoadImages);
    autoLoadIconsForPageCb->setChecked(m_task->isBrowserAutoLoadIconsForPage);
    pluginsEnabledCb->setChecked(m_task->isBrowserPluginsEnabled);

    connect(autoLoadImagesCb,&QCheckBox::toggled,this,[this](bool checked){
        m_task->isBrowserAutoLoadImages = checked;
    });
    connect(autoLoadIconsForPageCb,&QCheckBox::toggled,this,[this](bool checked){
        m_task->isBrowserAutoLoadIconsForPage = checked;
    });
    connect(pluginsEnabledCb,&QCheckBox::toggled,this,[this](bool checked){
        m_task->isBrowserPluginsEnabled = checked;
    });

    // userAgent 设置
    QWidget *uaWidget = new QWidget(this);
    uaWidget->setFixedHeight(40);
    QHBoxLayout *uaHLayout = new QHBoxLayout(uaWidget);
    uaHLayout->setContentsMargins(20,0,0,0);

    QLabel *uaLabel = new QLabel(uaWidget);
    uaLabel->setText("浏览器版本");
    QComboBox *uaComb = new QComboBox(uaWidget);
    uaComb->setFixedWidth(280);
    uaComb->setMinimumHeight(30);
    uaComb->setItemDelegate(new QStyledItemDelegate(uaComb));
    uaComb->setStyleSheet(m_stylesheet_QComboBox);

    QVector<MTaskUserAgent> uas = Database::getInstance()->userAgents;

    for (int i = 0; i < uas.count(); ++i) {
        MTaskUserAgent ua = uas.at(i);
        uaComb->addItem(ua.name);
        if(ua.userAgent==m_task->defaultUserAgent){
            uaComb->setCurrentIndex(i);
        }

    }
    connect(uaComb, QOverload<int>::of(&QComboBox::currentIndexChanged),[this,uas](int index){
        m_task->defaultUserAgent = uas.at(index).userAgent;

    });
    uaHLayout->addWidget(uaLabel);
    uaHLayout->addSpacing(10);
    uaHLayout->addWidget(uaComb);
    uaHLayout->addStretch(10);
    boxLayout->addWidget(uaWidget);
}

void TaskSettings::initMenuUi(){
    QWidget *menuWidget = new QWidget(this);
    menuWidget->setFixedHeight(40);
    QHBoxLayout *menuHLayout = new QHBoxLayout(menuWidget);
    menuHLayout->setContentsMargins(0,0,0,0);

    QPushButton *saveBtn = new QPushButton(menuWidget);
    saveBtn->setText("保存");
    saveBtn->setStyleSheet(m_stylesheet_QPushButton);
    saveBtn->setFixedSize(100,30);
    saveBtn->setCursor(Qt::PointingHandCursor);
    connect(saveBtn,&QPushButton::clicked,this,[this](){
        emit this->notifyBack();

    });


    QPushButton *backBtn = new QPushButton(menuWidget);
    backBtn->setText("返回");
    backBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    backBtn->setFixedSize(100,30);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn,&QPushButton::clicked,this,[this](){
        emit this->notifyBack();

    });

    menuHLayout->addSpacing(20);
    menuHLayout->addWidget(saveBtn);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(backBtn);
    menuHLayout->addStretch(10);

    boxLayout->addWidget(menuWidget);


}

