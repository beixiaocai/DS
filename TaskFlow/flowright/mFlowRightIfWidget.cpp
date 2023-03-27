#include "mFlowRightIfWidget.h"
#include "Utils/models.h"
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

MFlowRightIfWidget::MFlowRightIfWidget(QWidget *parent,MFlowStepData* data) :
    QWidget(parent)
{
    m_data = data;
    m_params = static_cast<MFlowStepParamsIf*>(data->stepparams);

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(10,5,10,5);
    initInfoUI();
    initSenierOptionUI();
//    initRetryUI();
    MRightSave *rightSave = new MRightSave(this);
    connect(rightSave,&MRightSave::notifySave,this,&MFlowRightIfWidget::onSave);
    boxLayout->addWidget(rightSave);
    boxLayout->addStretch(10);

}
void MFlowRightIfWidget::onSave(bool clicked){
    Q_UNUSED(clicked);
    m_params->name = nameLine->text();
    m_params->waitSleep = waitSpin->value();

    m_params->isLoop = loopCheck->checkState()==Qt::CheckState::Checked?true:false;
}
void MFlowRightIfWidget::initInfoUI(){
    QWidget *infoWidget = new QWidget(this);
    infoWidget->setFixedHeight(50);
    QGridLayout *infoLayout = new QGridLayout(infoWidget);

    QLabel * nameLabel = new QLabel(infoWidget);
    nameLabel->setText(tr("操作名称"));
    nameLine = new QLineEdit(infoWidget);
    nameLine->setStyleSheet(m_stylesheet_QLineEdit);
    nameLine->setText(m_params->name);
    infoLayout->addWidget(nameLabel,0,0);
    infoLayout->addWidget(nameLine,0,1);

    boxLayout->addWidget(infoWidget);
}
void MFlowRightIfWidget::initSenierOptionUI(){

    // 高级选项
    MOptionBox *optionBox = new MOptionBox(this);
    // 执行前等待
    QLabel *waitLabel = new QLabel(optionBox->gWidget);
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
    QLabel *loopLabel = new QLabel(optionBox->gWidget);
    loopLabel->setText(tr("使用循环"));
    loopCheck = new QCheckBox(optionBox->gWidget);
    loopCheck->setText(tr("点击当前循环中设置的元素"));
    loopCheck->setChecked(m_params->isLoop);
    optionBox->gLayout->addWidget(loopLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(loopCheck,optionBox->rowStart,1);


    boxLayout->addWidget(optionBox);

}
MFlowRightIfWidget::~MFlowRightIfWidget()
{

}
