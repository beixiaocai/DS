#include "mFlowRightLoopWidget.h"
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
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>

MFlowRightLoopWidget::MFlowRightLoopWidget(QWidget *parent,MFlowStepData* data) : QWidget(parent)
{

    m_data = data;
    m_params = static_cast<MFlowStepParamsLoop*>(data->stepparams);

    m_tmap.insert(MCONSTANT_RADIO_SINGLE,"单个元素");
    m_tmap.insert(MCONSTANT_RADIO_FIXED,"固定元素");
    m_tmap.insert(MCONSTANT_RADIO_UNFIXED,"不固定元素");
    m_tmap.insert(MCONSTANT_RADIO_SITEADDRESS,"网址列表");

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(10,5,10,5);
    initInfoUI();
    initSenierOptionUI();
    initRetryUI();

    MRightSave *rightSave = new MRightSave(this);
    connect(rightSave,&MRightSave::notifySave,this,&MFlowRightLoopWidget::onSave);
    boxLayout->addWidget(rightSave);
    boxLayout->addStretch(10);

    setTextList(this->m_params->loopTextList);

}
const QString MFlowRightLoopWidget::transferLoopTypeName(const QString name,bool isForward){

    if(isForward){
        //由编码转为显示的文字
        if(m_tmap.contains(name)){
             return m_tmap.find(name).value();
        }

    }else {
        QMap<QString,QString>::const_iterator it;
        for (it=m_tmap.constBegin();it!=m_tmap.constEnd();++it) {
            if(it.value()==name){
                return it.key();
            }
        }
    }
   return "";
}
void MFlowRightLoopWidget::onSave(bool clicked){
    Q_UNUSED(clicked);
    m_params->name = nameLine->text();

    m_params->waitSleep = waitSpin->value();


    //保存循环方式
    m_params->loopTypeName = transferLoopTypeName(loopTypeValueLabel->text(),false);
    //保存循环方式对应的值
    if(m_params->loopTypeName==MCONSTANT_RADIO_SITEADDRESS){
        QStringList m_addressList = loopTypeValueEdit->toPlainText().trimmed().split("\n");
        QStringList addressList;
        for (int i = 0; i < m_addressList.length(); ++i) {
            if(m_addressList[i].startsWith("http")){
                addressList.append(m_addressList[i].trimmed());
            }
        }
         m_params->loopTypeValue = addressList.join(",");
         m_params->loopTextList = addressList;
    }else {
        m_params->loopTypeValue =  loopTypeValueEdit->toPlainText().trimmed();
    }

    m_params->isEndLoopWithTimes = endLoopWithTimesCheck->checkState()==Qt::CheckState::Checked ?true:false;
    m_params->endLoopTimes = endLoopTimesSpin->value();

}
void MFlowRightLoopWidget::setTextList(const QStringList textList){
    listWidgetName->setText(QString("循环列表（总共%1条）").arg(textList.length()));

    // step1 清理原有字段
    QVector<QListWidgetItem *>items;
    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        items.append(item);
    }
    for (int i = 0; i < items.count(); ++i) {
        delete items[i];
    }
    // step2 设置新字段
    QSize s(220,26);
    QListWidgetItem *item;
    for (int i = 0; i < textList.length(); ++i) {
        item = new QListWidgetItem(textList[i]);
        item->setSizeHint(s);
        listWidget->addItem(item);
    }

}


void MFlowRightLoopWidget::initInfoUI(){

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


    listWidgetName = new QLabel(this);
    listWidget = new QListWidget(this);
    listWidget->setFixedHeight(100);
    listWidget->setMinimumWidth(this->width());


    boxLayout->addWidget(infoWidget);
    boxLayout->addWidget(listWidgetName);
    boxLayout->addWidget(listWidget);

}


void MFlowRightLoopWidget::initSenierOptionUI(){

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
    ++optionBox->rowStart;


    // 循环方式 ----------------------------- start
    QLabel *loopTypeLabel = new QLabel(optionBox->gWidget);
    loopTypeLabel->setStyleSheet(m_stylesheet_QLabel);
    loopTypeLabel->setText(tr("循环方式"));

    QGroupBox *loopTypeBox = new QGroupBox(optionBox->gWidget);

    loopTypeBox->setStyleSheet(".QGroupBox{border:0px;}");
    QHBoxLayout *loopTypeBoxLayout = new QHBoxLayout(loopTypeBox);
    QRadioButton *singleRadio = new QRadioButton(loopTypeBox);
    singleRadio->setText(transferLoopTypeName(MCONSTANT_RADIO_SINGLE));
    QRadioButton *fixedRadio = new QRadioButton(loopTypeBox);
    fixedRadio->setText(transferLoopTypeName(MCONSTANT_RADIO_FIXED));
    QRadioButton *unfixedRadio = new QRadioButton(loopTypeBox);
    unfixedRadio->setText(transferLoopTypeName(MCONSTANT_RADIO_UNFIXED));
    QRadioButton *siteAddressListRadio = new QRadioButton(loopTypeBox);
    siteAddressListRadio->setText(transferLoopTypeName(MCONSTANT_RADIO_SITEADDRESS));
    loopTypeBoxLayout->addWidget(singleRadio);
    loopTypeBoxLayout->addWidget(fixedRadio);
    loopTypeBoxLayout->addWidget(unfixedRadio);
    loopTypeBoxLayout->addWidget(siteAddressListRadio);

    // 设置循环方式的默认radio选中
    if(m_params->loopTypeName==MCONSTANT_RADIO_SINGLE){
        singleRadio->setChecked(true);
    }else if (m_params->loopTypeName==MCONSTANT_RADIO_FIXED) {
        fixedRadio->setChecked(true);
    }else if (m_params->loopTypeName==MCONSTANT_RADIO_UNFIXED) {
        unfixedRadio->setChecked(true);
    }else if (m_params->loopTypeName==MCONSTANT_RADIO_SITEADDRESS) {
        siteAddressListRadio->setChecked(true);
    }


    QButtonGroup *gs = new QButtonGroup(optionBox->gWidget);
    gs->addButton(singleRadio);
    gs->addButton(fixedRadio);
    gs->addButton(unfixedRadio);
    gs->addButton(siteAddressListRadio);



    // 循环方式的值显示控件
    loopTypeValueLabel = new QLabel(optionBox->gWidget);
    loopTypeValueLabel->setStyleSheet(m_stylesheet_QLabel);
    loopTypeValueLabel->setText(transferLoopTypeName(m_params->loopTypeName));

    loopTypeValueEdit = new QTextEdit(optionBox->gWidget);
    loopTypeValueEdit->setMaximumHeight(40);
    loopTypeValueEdit->setStyleSheet(m_stylesheet_QTextEdit);
    loopTypeValueEdit->setText(m_params->loopTypeValue.split(",").join("\n"));

    connect(gs, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
          [this](QAbstractButton *button){

         QRadioButton *clickedRadio = static_cast<QRadioButton *>(button);
         QString clickedRadioText = clickedRadio->text();

         //设置循环方式值的key
         loopTypeValueLabel->setText(clickedRadioText);

         //设置循环方式值得value
         if(clickedRadioText==transferLoopTypeName(m_params->loopTypeName)){// 当前选中与当前保存的参数一致
            loopTypeValueEdit->setText(m_params->loopTypeValue.split(",").join("\n"));
         }else {
            loopTypeValueEdit->setText("");
          }

    });

    optionBox->gLayout->addWidget(loopTypeLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(loopTypeBox,optionBox->rowStart,1);
    ++optionBox->rowStart;
    optionBox->gLayout->addWidget(loopTypeValueLabel,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(loopTypeValueEdit,optionBox->rowStart,1);

    // 循环方式 ----------------------------- end

    // 满足条件则退出循环 ----------------------------- start
    endLoopWithTimesCheck = new QCheckBox(optionBox->gWidget);
    endLoopWithTimesCheck->setText(tr("循环终止"));
    endLoopWithTimesCheck->setChecked(m_params->isEndLoopWithTimes);

    QWidget *endLoopBox = new QWidget(optionBox->gWidget);
    QHBoxLayout *endLoopLayout = new QHBoxLayout(endLoopBox);
    endLoopLayout->setContentsMargins(0,0,0,0);

    endLoopTimesSpin = new QSpinBox(endLoopBox);
    endLoopTimesSpin->setMinimum(1);
    endLoopTimesSpin->setFixedWidth(80);
    endLoopTimesSpin->setStyleSheet(m_stylesheet_QSpinBox);
    endLoopTimesSpin->setValue(m_params->endLoopTimes);

    QLabel *endLoopLabel= new QLabel(endLoopBox);
    endLoopLabel->setStyleSheet(m_stylesheet_QLabel);
    endLoopLabel->setText(tr("（满足指定次数则退出循环）"));
    endLoopLayout->addWidget(endLoopTimesSpin);
    endLoopLayout->addWidget(endLoopLabel);
++optionBox->rowStart;

    optionBox->gLayout->addWidget(endLoopWithTimesCheck,optionBox->rowStart,0);
    optionBox->gLayout->addWidget(endLoopBox,optionBox->rowStart,1);

    // 满足条件则退出循环 ----------------------------- end


    boxLayout->addWidget(optionBox);

}
void MFlowRightLoopWidget::initRetryUI(){}


MFlowRightLoopWidget::~MFlowRightLoopWidget(){

}
