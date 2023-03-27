#include "mFlowRightClickeleWidget.h"
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
MFlowRightClickEleWidget::MFlowRightClickEleWidget(QWidget *parent,MFlowStepData* data) :
    QWidget(parent)
{
    m_data = data;
    m_params = static_cast<MFlowStepParamsClickEle*>(data->stepparams);


    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(10,5,10,5);
    initInfoUI();
    initSenierOptionUI();
//    initRetryUI();
    MRightSave *rightSave = new MRightSave(this);
    connect(rightSave,&MRightSave::notifySave,this,&MFlowRightClickEleWidget::onSave);
    boxLayout->addWidget(rightSave);

    boxLayout->addStretch(10);



}
void MFlowRightClickEleWidget::onSave(bool clicked){
    Q_UNUSED(clicked);

    m_params->name = nameLine->text();
    m_params->eleText = eleTextLine->text();

    m_params->waitSleep = waitSpin->value();
    //元素xpath保存
    m_params->eleXpath = eleXpathLine->text();

    m_params->isLoop = loopCheck->checkState()==Qt::CheckState::Checked?true:false;
    m_params->isNewTab = isNewTabCheck->checkState()==Qt::CheckState::Checked?true:false;

//    if(isAjaxCheck->checkState()==Qt::CheckState::Checked){
//         m_params->isAjax = true;
//    }else {
//      m_params->isAjax = false;
//    }
//    m_params->ajaxOvertime = ajaxOvertimeSpin->value();
//    if(isPageSpeedCheck->checkState()==Qt::CheckState::Checked){
//         m_params->isPageSpeed = true;
//    }else {
//      m_params->isPageSpeed = false;
//    }
//    if(isAnchorCheck->checkState()==Qt::CheckState::Checked){
//         m_params->isAnchor = true;
//    }else {
//      m_params->isAnchor = false;
//    }
//    m_params->anchorId = anchorIdLine->text();
//    if(autoTryCheck->checkState()==Qt::CheckState::Checked){
//         m_params->isAutoTry = true;
//    }else {
//        m_params->isAutoTry = false;
//    }


    //保存滚动页面及其参数
    m_params->isRoll = rollCheck->checkState()==Qt::CheckState::Checked?true:false;
    m_params->rollCount = rollWidget->rollCountSpin->value();
    m_params->rollInterval = rollWidget->rollIntervalSpin->value();
    m_params->rollTypeIndex = rollWidget->rollTypeCombo->currentIndex();

}
void MFlowRightClickEleWidget::initInfoUI(){
    QWidget *infoWidget = new QWidget(this);
    infoWidget->setFixedHeight(40);
    QHBoxLayout *infoLayout = new QHBoxLayout(infoWidget);

    // 第一行
    QLabel * nameLabel = new QLabel(infoWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel);
    nameLabel->setText(tr("操作名称"));
    nameLine = new QLineEdit(infoWidget);

    nameLine->setStyleSheet(m_stylesheet_QLineEdit);
    nameLine->setText(m_params->name);
    nameLine->setFixedWidth(160);

    infoLayout->addWidget(nameLabel);
    infoLayout->addSpacing(20);
    infoLayout->addWidget(nameLine);
    infoLayout->addStretch(10);

    boxLayout->addWidget(infoWidget);
}
void MFlowRightClickEleWidget::initSenierOptionUI(){

    // 高级选项
    optionBox = new MOptionBox(this);
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
    ++optionBox->rowStart;
    QLabel *eleXpathLabel= new QLabel(optionBox->gWidget);
    eleXpathLabel->setStyleSheet(m_stylesheet_QLabel);
    eleXpathLabel->setText(tr("元素xpath"));
    eleXpathLine = new QLineEdit(optionBox->gWidget);
    eleXpathLine->setStyleSheet(m_stylesheet_QLineEdit);
    eleXpathLine->setMaximumWidth(160);
    eleXpathLine->setText(m_params->eleXpath);
    eleXpathLine->setCursorPosition(0);

    optionBox->gLayout->addWidget(eleXpathLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(eleXpathLine,optionBox->rowStart,1);

      ++optionBox->rowStart;
    QLabel *eleTextLabel = new QLabel(optionBox->gWidget);
    eleTextLabel->setStyleSheet(m_stylesheet_QLabel);
    eleTextLabel->setText(tr("元素标签"));
    eleTextLine = new QLineEdit(optionBox->gWidget);
    eleTextLine->setMaximumWidth(160);
    eleTextLine->setStyleSheet(m_stylesheet_QLineEdit);
    eleTextLine->setText(this->m_params->eleText);
    optionBox->gLayout->addWidget(eleTextLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(eleTextLine,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *loopLabel = new QLabel(optionBox->gWidget);
    loopLabel->setStyleSheet(m_stylesheet_QLabel);
    loopLabel->setText(tr("使用循环"));
    loopCheck = new QCheckBox(optionBox->gWidget);
    loopCheck->setStyleSheet(m_stylesheet_QCheckBox);
    loopCheck->setText(tr("点击当前循环中设置的元素"));
    loopCheck->setChecked(m_params->isLoop);
    optionBox->gLayout->addWidget(loopLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(loopCheck,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *isNewTabLabel = new QLabel(optionBox->gWidget);
    isNewTabLabel->setStyleSheet(m_stylesheet_QLabel);
    isNewTabLabel->setText(tr("开新标签"));
    isNewTabCheck = new QCheckBox(optionBox->gWidget);
    isNewTabCheck->setStyleSheet(m_stylesheet_QCheckBox);
    isNewTabCheck->setText(tr("在新标签中打开页面"));
    isNewTabCheck->setChecked(m_params->isNewTab);
    optionBox->gLayout->addWidget(isNewTabLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(isNewTabCheck,optionBox->rowStart,1);


/*
    ++optionBox->rowStart;
    QLabel *isAjaxLabel = new QLabel(optionBox->gWidget);
    isAjaxLabel->setStyleSheet(m_stylesheet_QLabel);
    isAjaxLabel->setText(tr("Ajax加载"));
        QWidget *ajaxWidget = new QWidget(optionBox->gWidget);
        QHBoxLayout *ajaxLayout = new QHBoxLayout(ajaxWidget);
        ajaxLayout->setContentsMargins(0,0,0,0);

        isAjaxCheck = new QCheckBox(optionBox->gWidget);
        isAjaxCheck->setStyleSheet(m_stylesheet_QCheckBox);
        isAjaxCheck->setText(tr("Ajax加载数据"));
        isAjaxCheck->setChecked(m_params->isAjax);
        QLabel *ajaxOvertimebLabel = new QLabel(optionBox->gWidget);
        ajaxOvertimebLabel->setStyleSheet(m_stylesheet_QLabel);
        ajaxOvertimebLabel->setText(tr("Ajax超时"));
        ajaxOvertimeSpin = new QSpinBox(optionBox->gWidget);
        ajaxOvertimeSpin->setStyleSheet(m_stylesheet_QSpinBox);
        ajaxOvertimeSpin->setMinimum(0);
        ajaxOvertimeSpin->setMaximum(30);
        ajaxOvertimeSpin->setSuffix("秒");
        ajaxOvertimeSpin->setFixedWidth(50);
        ajaxOvertimeSpin->setValue(m_params->ajaxOvertime);

        ajaxLayout->addWidget(isAjaxCheck);
        ajaxLayout->addWidget(ajaxOvertimebLabel);
        ajaxLayout->addWidget(ajaxOvertimeSpin);
        ajaxLayout->addStretch(10);

        if(m_params->isAjax){
            ajaxOvertimebLabel->show();
            ajaxOvertimeSpin->show();
        }else {
            ajaxOvertimebLabel->hide();
            ajaxOvertimeSpin->hide();
        }
        connect(isAjaxCheck,&QCheckBox::stateChanged,this,[=](int state){
           if(Qt::CheckState::Checked==state){
               ajaxOvertimebLabel->show();
               ajaxOvertimeSpin->show();
           }else{
               ajaxOvertimebLabel->hide();
               ajaxOvertimeSpin->hide();
           }
       });

    optionBox->gLayout->addWidget(isAjaxLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(ajaxWidget,optionBox->rowStart,1);


    ++optionBox->rowStart;
    QLabel *isPageSpeedLabel = new QLabel(optionBox->gWidget);
    isPageSpeedLabel->setStyleSheet(m_stylesheet_QLabel);
    isPageSpeedLabel->setText(tr("页面加速"));
    isPageSpeedCheck = new QCheckBox(optionBox->gWidget);
    isPageSpeedCheck->setStyleSheet(m_stylesheet_QCheckBox);
    isPageSpeedCheck->setText(tr("优化非Ajax页面加载速度"));
    isPageSpeedCheck->setChecked(m_params->isPageSpeed);
    optionBox->gLayout->addWidget(isPageSpeedLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(isPageSpeedCheck,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *isAnchorLabel = new QLabel(optionBox->gWidget);
    isAnchorLabel->setStyleSheet(m_stylesheet_QLabel);
    isAnchorLabel->setText(tr("定位锚点"));

        QWidget *anchorWidget = new QWidget(optionBox->gWidget);
        QHBoxLayout *anchorLayout = new QHBoxLayout(anchorWidget);
        anchorLayout->setContentsMargins(0,0,0,0);

        isAnchorCheck = new QCheckBox(optionBox->gWidget);
        isAnchorCheck->setStyleSheet(m_stylesheet_QCheckBox);
        isAnchorCheck->setText(tr("页面加载完成后定位锚点"));
        isAnchorCheck->setChecked(m_params->isAnchor);

        QLabel *anchorIdLabel = new QLabel(optionBox->gWidget);
        anchorIdLabel->setStyleSheet(m_stylesheet_QLabel);
        anchorIdLabel->setText(tr("锚点Id"));
        anchorIdLine = new QLineEdit(optionBox->gWidget);
        anchorIdLine->setStyleSheet(m_stylesheet_QLineEdit);
        anchorIdLine->setText(m_params->anchorId);

        anchorLayout->addWidget(isAnchorCheck);
        anchorLayout->addWidget(anchorIdLabel);
        anchorLayout->addWidget(anchorIdLine);
        anchorLayout->addStretch(10);

        if(m_params->isAnchor){
            anchorIdLabel->show();
            anchorIdLine->show();
        }else {
            anchorIdLabel->hide();
            anchorIdLine->hide();
        }
        connect(isAnchorCheck,&QCheckBox::stateChanged,this,[=](int state){
           if(Qt::CheckState::Checked==state){
               anchorIdLabel->show();
               anchorIdLine->show();
           }else{
               anchorIdLabel->hide();
               anchorIdLine->hide();
           }
       });

    optionBox->gLayout->addWidget(isAnchorLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(anchorWidget,optionBox->rowStart,1);

    ++optionBox->rowStart;
    QLabel *autoTryLabel = new QLabel(optionBox->gWidget);
    autoTryLabel->setStyleSheet(m_stylesheet_QLabel);
    autoTryLabel->setText(tr("自动重试"));
    autoTryCheck = new QCheckBox(optionBox->gWidget);
    autoTryCheck->setStyleSheet(m_stylesheet_QCheckBox);
    autoTryCheck->setText(tr("点击后若网页无变化时自动重试（Ajax点击请谨慎开启）"));
    autoTryCheck->setChecked(m_params->isAutoTry);
    optionBox->gLayout->addWidget(autoTryLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(autoTryCheck,optionBox->rowStart,1);
*/

        ++optionBox->rowStart;
    QLabel *rollLabel = new QLabel(optionBox->gWidget);
    rollLabel->setStyleSheet(m_stylesheet_QLabel);
    rollLabel->setText(tr("滚动页面"));
    rollCheck = new QCheckBox(optionBox->gWidget);
    rollCheck->setStyleSheet(m_stylesheet_QCheckBox);
    rollCheck->setText(tr("页面加载完成后向下滚动"));
    rollCheck->setChecked(m_params->isRoll);
    optionBox->gLayout->addWidget(rollLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(rollCheck,optionBox->rowStart,1);

    ++optionBox->rowStart;

    rollWidget = new MRollWidget(optionBox->gWidget);
    rollWidget->rollCountSpin->setValue(m_params->rollCount);
    rollWidget->rollIntervalSpin->setValue(m_params->rollInterval);
    rollWidget->rollTypeCombo->setCurrentIndex(m_params->rollTypeIndex);

    if(m_params->isRoll){
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

    optionBox->gLayout->addWidget(rollWidget,optionBox->rowStart,1);

    boxLayout->addWidget(optionBox);

}
void MFlowRightClickEleWidget::initRetryUI(){}


MFlowRightClickEleWidget::~MFlowRightClickEleWidget()
{

}
