#include "mRollWidget.h"
#include "style.h"
#include "TaskFlow/mFcModels.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>

MRollWidget::MRollWidget(QWidget *parent) :
    QWidget(parent)
{

    setFixedHeight(30);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0,0,0,0);


    QLabel *rollCountLabel = new QLabel(this);
    rollCountLabel->setStyleSheet(m_stylesheet_QLabel12);
    rollCountLabel->setText("滚动次数");
    rollCountSpin = new QSpinBox(this);
    rollCountSpin->setStyleSheet(m_stylesheet_QSpinBox);
    rollCountSpin->setFixedWidth(40);

    QLabel *rollIntervalLabel = new QLabel(this);
    rollIntervalLabel->setStyleSheet(m_stylesheet_QLabel12);
    rollIntervalLabel->setText("每次间隔");
    rollIntervalSpin = new QSpinBox(this);
    rollIntervalSpin->setStyleSheet(m_stylesheet_QSpinBox);
    rollIntervalSpin->setMinimum(0);
    rollIntervalSpin->setMaximum(30);
    rollIntervalSpin->setSuffix("秒");
    rollIntervalSpin->setFixedWidth(50);

    QLabel *rollTypeLabel = new QLabel(this);
    rollTypeLabel->setStyleSheet(m_stylesheet_QLabel12);
    rollTypeLabel->setText("滚动方式");
    rollTypeCombo = new QComboBox(this);
    rollTypeCombo->setStyleSheet(m_stylesheet_QComboBox);

    QStringList rollTypes;
    rollTypes<<"直接滚动到底部"<<"向下滚动一屏";
    for (int i = 0; i < rollTypes.length(); ++i) {
         rollTypeCombo->addItem(rollTypes[i]);
    }

    hLayout->addWidget(rollCountLabel);
    hLayout->addWidget(rollCountSpin);

    hLayout->addWidget(rollIntervalLabel);
    hLayout->addWidget(rollIntervalSpin);

    hLayout->addWidget(rollTypeLabel);
    hLayout->addWidget(rollTypeCombo);

    hLayout->addStretch(10);



}

MRollWidget::~MRollWidget()
{

}
