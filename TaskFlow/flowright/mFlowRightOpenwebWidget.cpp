#include "mFlowRightOpenwebWidget.h"
#include "mRollWidget.h"
#include "mOptionBox.h"
#include "mRightSave.h"
#include "style.h"
#include "TaskFlow/mFcModels.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

MFlowRightOpenWebWidget::MFlowRightOpenWebWidget(QWidget *parent,MFlowStepData* data) :
    QWidget(parent)
{
    m_data = data;
    m_params =static_cast<MFlowStepParamsOpenWeb*>(data->stepparams);

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(10,5,10,5);
    initInfoUI();
    initSenierOptionUI();
//    initRetryUI();

    MRightSave *rightSave = new MRightSave(this);
    connect(rightSave,&MRightSave::notifySave,this,&MFlowRightOpenWebWidget::onSave);
    boxLayout->addWidget(rightSave);
    boxLayout->addStretch(10);

}
void MFlowRightOpenWebWidget::onSave(bool clicked){
     Q_UNUSED(clicked);
    // 保存基本信息

    m_params->name = nameLine->text();
    m_params->url = siteLine->text();
    m_params->overtime = overtimeSpin->value();

    m_params->waitSleep = waitSpin->value();

//    if(autoTryCheck->checkState()==Qt::CheckState::Checked){
//        m_params->isAutoTry = true;
//    }else {
//        m_params->isAutoTry = false;
//    }
    m_params->isCustomCookie = customCookieCheck->checkState()==Qt::CheckState::Checked?true:false;

    m_params->cookie = cookieLine->text().trimmed();
    m_params->isLoop = loopCheck->checkState()==Qt::CheckState::Checked?true:false;
    m_params->isPreventWindow = preventCheck->checkState()==Qt::CheckState::Checked?true:false;
    m_params->isClearCacheBeforeOpenWeb = clearCacheCheck->checkState()==Qt::CheckState::Checked?true:false;
    m_params->isRoll = rollCheck->checkState()==Qt::CheckState::Checked?true:false;

    m_params->rollCount = rollWidget->rollCountSpin->value();
    m_params->rollInterval = rollWidget->rollIntervalSpin->value();
    m_params->rollTypeIndex = rollWidget->rollTypeCombo->currentIndex();


}
void MFlowRightOpenWebWidget::initInfoUI(){
    QWidget *infoWidget = new QWidget(this);
    infoWidget->setFixedHeight(80);
    QGridLayout *infoLayout = new QGridLayout(infoWidget);
    // 第一行 4列
    QLabel * nameLabel = new QLabel(infoWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel);
    nameLabel->setText(tr("操作名称"));
    nameLine = new QLineEdit(infoWidget);
    nameLine->setStyleSheet(m_stylesheet_QLineEdit);
    nameLine->setText(m_params->name);

    QLabel *overtimeLabel = new QLabel(infoWidget);
    overtimeLabel->setStyleSheet(m_stylesheet_QLabel);
    overtimeLabel->setText(tr("超时"));
    overtimeSpin = new QSpinBox(infoWidget);
    overtimeSpin->setSuffix("秒");
    overtimeSpin->setStyleSheet(m_stylesheet_QSpinBox);
    overtimeSpin->setMaximum(600);
    overtimeSpin->setMinimum(5);
    overtimeSpin->setMinimumWidth(50);
    overtimeSpin->setValue(m_params->overtime);

    // 第二行 2 列
    QLabel *siteNameLabel = new QLabel(infoWidget);
    siteNameLabel->setStyleSheet(m_stylesheet_QLabel);
    siteNameLabel->setText(tr("网址"));
    siteLine = new QLineEdit(infoWidget);
    siteLine->setStyleSheet(m_stylesheet_QLineEdit);
    siteLine->setText(m_params->url);

    infoLayout->addWidget(nameLabel,0,0);
    infoLayout->addWidget(nameLine,0,1);
    infoLayout->addWidget(overtimeLabel,0,2);
    infoLayout->addWidget(overtimeSpin,0,3);
    infoLayout->addWidget(siteNameLabel,1,0);
    infoLayout->addWidget(siteLine,1,1);

    boxLayout->addWidget(infoWidget);
}

void MFlowRightOpenWebWidget::initSenierOptionUI(){
    // 高级选项
    MOptionBox *optionBox = new MOptionBox(this);

    // 执行前等待
    QLabel *waitLabel = new QLabel(optionBox->gWidget);
    waitLabel->setStyleSheet(m_stylesheet_QLabel);
    waitLabel->setText("执行前等待");
    waitSpin = new QSpinBox(optionBox->gWidget);
    waitSpin->setMinimum(0);
    waitSpin->setMaximum(30);
    waitSpin->setSuffix("秒");
    waitSpin->setMaximumWidth(160);
    waitSpin->setStyleSheet(m_stylesheet_QSpinBox);
    waitSpin->setValue(m_params->waitSleep);
    optionBox->gLayout->addWidget(waitLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(waitSpin,optionBox->rowStart,1);

//    ++optionBox->rowStart;
//    QLabel *autoTryLabel = new QLabel(optionBox->gWidget);
//    autoTryLabel->setStyleSheet(m_stylesheet_QLabel);
//    autoTryLabel->setText(tr("自动重试"));
//    autoTryCheck = new QCheckBox(optionBox->gWidget);
//    autoTryCheck->setText(tr("加载失败后，自动重试"));
//    autoTryCheck->setChecked(m_params->isAutoTry);
//    optionBox->gLayout->addWidget(autoTryLabel,optionBox->rowStart,0);
//    optionBox->gLayout->addWidget(autoTryCheck,optionBox->rowStart,1);


    ++optionBox->rowStart;
    QLabel *cookieLabel = new QLabel(optionBox->gWidget);
    cookieLabel->setStyleSheet(m_stylesheet_QLabel);
    cookieLabel->setText(tr("自定义Cookie"));

        QWidget *cookieWidget = new QWidget(optionBox->gWidget);
        QHBoxLayout *cookieLayout = new QHBoxLayout(cookieWidget);
        cookieLayout->setContentsMargins(0,0,0,0);
        customCookieCheck = new QCheckBox(cookieWidget);
        customCookieCheck->setText(tr("使用指定的Cookie"));
        customCookieCheck->setChecked(m_params->isCustomCookie);

        cookieLine = new QLineEdit(cookieWidget);
        cookieLine->setStyleSheet(m_stylesheet_QLineEdit);
        cookieLine->setText(m_params->cookie);
        cookieLayout->addWidget(customCookieCheck);
        cookieLayout->addWidget(cookieLine);
        cookieLayout->addStretch(10);
        m_params->isCustomCookie?cookieLine->show():cookieLine->hide();

        connect(customCookieCheck,&QCheckBox::clicked,this,[this](bool checked){
              checked?cookieLine->show():cookieLine->hide();
        });




    optionBox->gLayout->addWidget(cookieLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(cookieWidget,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *loopLabel = new QLabel(optionBox->gWidget);
    loopLabel->setStyleSheet(m_stylesheet_QLabel);
    loopLabel->setText(tr("使用循环"));
    loopCheck = new QCheckBox(optionBox->gWidget);
    loopCheck->setText(tr("点击当前循环中设置的元素"));
    loopCheck->setChecked(m_params->isLoop);
    optionBox->gLayout->addWidget(loopLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(loopCheck,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *preventLabel = new QLabel(optionBox->gWidget);
    preventLabel->setStyleSheet(m_stylesheet_QLabel);
    preventLabel->setText(tr("阻止弹出"));
    preventCheck = new QCheckBox(optionBox->gWidget);
    preventCheck->setText(tr("阻止弹出窗口（可能是广告）"));
    preventCheck->setChecked(m_params->isPreventWindow);
    optionBox->gLayout->addWidget(preventLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(preventCheck,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *clearCacheLabel = new QLabel(optionBox->gWidget);
    clearCacheLabel->setStyleSheet(m_stylesheet_QLabel);
    clearCacheLabel->setText(tr("清理缓存"));
    clearCacheCheck = new QCheckBox(optionBox->gWidget);
    clearCacheCheck->setText(tr("打开网页前先清理浏览器缓存"));
    clearCacheCheck->setChecked(m_params->isClearCacheBeforeOpenWeb);
    optionBox->gLayout->addWidget(clearCacheLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(clearCacheCheck,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *rollLabel = new QLabel(optionBox->gWidget);
    rollLabel->setStyleSheet(m_stylesheet_QLabel);
    rollLabel->setText(tr("滚动页面"));
    rollCheck = new QCheckBox(optionBox->gWidget);
    rollCheck->setText(tr("页面加载完成后向下滚动"));
    rollCheck->setChecked(m_params->isRoll);


    optionBox->gLayout->addWidget(rollLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(rollCheck,optionBox->rowStart,1);
        ++optionBox->rowStart;

    rollWidget = new MRollWidget(optionBox->gWidget);
    optionBox->gLayout->addWidget(rollWidget,optionBox->rowStart,1);
    rollWidget->rollCountSpin->setValue(m_params->rollCount);
    rollWidget->rollIntervalSpin->setValue(m_params->rollInterval);
    rollWidget->rollTypeCombo->setCurrentIndex(m_params->rollTypeIndex);

    if(m_params->isRoll){
        //显示rollWidget
        rollWidget->show();
    }else{
        rollWidget->hide();
    }
    connect(rollCheck,&QCheckBox::stateChanged,this,[=](int state){
       if(Qt::CheckState::Checked==state){
           rollWidget->show();
       }else{
           rollWidget->hide();
       }
   });

    boxLayout->addWidget(optionBox);

}
void MFlowRightOpenWebWidget::initRetryUI(){}



MFlowRightOpenWebWidget::~MFlowRightOpenWebWidget()
{

}
