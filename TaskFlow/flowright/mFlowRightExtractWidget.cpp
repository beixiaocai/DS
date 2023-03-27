#include "mFlowRightExtractWidget.h"
#include "Utils/models.h"
#include "mOptionBox.h"
#include "mRightSave.h"
#include "style.h"
#include "TaskFlow/mFcModels.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>

MFieldMenu::MFieldMenu(QWidget *parent) : QWidget(parent)
{

    setFixedHeight(36);
    QHBoxLayout *boxLayout = new QHBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);

    editBtn = new QToolButton(this);
    editBtn->setStyleSheet(m_stylesheet_QToolButton_hollow);
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setText(tr("编辑"));

    delBtn = new QToolButton(this);
    delBtn->setStyleSheet(m_stylesheet_QToolButton_hollow);
    delBtn->setText(tr("删除"));
    delBtn->setCursor(Qt::PointingHandCursor);

    boxLayout->addStretch(10);
    boxLayout->addWidget(editBtn);
    boxLayout->addWidget(delBtn);

    boxLayout->addStretch(10);

}
MFieldMenu::~MFieldMenu(){
    qDebug()<<"MFieldMenu::~MFieldMenu()";
}

MFieldEditDialog::MFieldEditDialog(MParamsField * field,QWidget *parent) : QDialog(parent)
{

    //设置模态
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setWindowTitle("编辑字段");
    setStyleSheet(".MFieldEditDialog{background-color:rgb(255,255,255);}");
    setFixedSize(220,130);

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);


    QWidget *gWidget = new QWidget(this);
    gWidget->setFixedHeight(90);
    QGridLayout *gLayout = new QGridLayout(gWidget);
    gLayout->setSpacing(5);
    gLayout->setColumnStretch(2,10);

    // 字段名称
    QLabel *nameLabel = new QLabel(gWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel);
    nameLabel->setText("字段名称");

    QLineEdit *nameLine = new QLineEdit(gWidget);
    nameLine->setStyleSheet(m_stylesheet_QLineEdit);
    nameLine->setText(field->fieldName);
    //字段提取示例

    QLabel *valLabel = new QLabel(gWidget);
    valLabel->setStyleSheet(m_stylesheet_QLabel);
    valLabel->setText("示例数据");

    QLineEdit *valLine = new QLineEdit(gWidget);
    valLine->setStyleSheet(m_stylesheet_QLineEdit);
    valLine->setText(field->fieldVal);
    valLine->setCursorPosition(0);


    gLayout->addWidget(nameLabel,0,0);
    gLayout->addWidget(nameLine,0,1);
    gLayout->addWidget(valLabel,1,0);
    gLayout->addWidget(valLine,1,1);


    // 线
    QWidget *line = new QWidget(this);
    line->setStyleSheet(".QWidget{background-color:rgb(233,233,233);}");
    line->setFixedHeight(1);

    // 保存
    QPushButton * saveBtn = new QPushButton(this);
    saveBtn->setFixedSize(76,22);
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
    saveBtn->setText("保存");
    connect(saveBtn,&QPushButton::clicked,this,[this,field,nameLine,valLine](){
        field->fieldName = nameLine->text().trimmed();
        field->fieldVal = valLine->text().trimmed();
        this->close();
    });


    boxLayout->addWidget(gWidget);
    boxLayout->addWidget(line);
    boxLayout->addWidget(saveBtn,0,Qt::AlignHCenter);
    boxLayout->addSpacing(10);
}

MFlowRightExtractWidget::MFlowRightExtractWidget(QWidget *parent,MFlowStepData* data,MTask *task) : QWidget(parent),
    m_data(data),
    m_task(task)
{

    m_params = static_cast<MFlowStepParamsExtract*>(data->stepparams);

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(10,5,10,5);
    initInfoUI();
    initFieldsUI();
    initSenierOptionUI();

    MRightSave *rightSave = new MRightSave(this);
    connect(rightSave,&MRightSave::notifySave,this,&MFlowRightExtractWidget::onSave);
    boxLayout->addWidget(rightSave);
    boxLayout->addStretch(10);

    for (int i = 0; i < m_params->pfs.count(); ++i) {
        MParamsField *field = m_params->pfs[i];
        field->currentRow = i;
        addField(field);
    }
    tableWidget->scrollToBottom();

}
MFlowRightExtractWidget::~MFlowRightExtractWidget()
{

}

void MFlowRightExtractWidget::onSave(bool clicked){
    Q_UNUSED(clicked);
    m_params->name = nameLine->text();
    m_params->waitSleep = waitSpin->value();
    m_params->isLoop = loopCheck->checkState()==Qt::CheckState::Checked?true:false;
}
void MFlowRightExtractWidget::initInfoUI(){
    QWidget *infoWidget = new QWidget(this);
    infoWidget->setFixedHeight(40);
    QHBoxLayout *infoLayout = new QHBoxLayout(infoWidget);

    // 第一行
    QLabel * nameLabel = new QLabel(infoWidget);
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

void MFlowRightExtractWidget::initFieldsUI(){

    //字段UI
    QWidget *fieldsWidget = new QWidget(this);
    QVBoxLayout *fieldsLayout = new QVBoxLayout(fieldsWidget);
    fieldsLayout->setContentsMargins(0,0,0,0);
    fieldsWidget->setFixedHeight(200);


    // 第一行 操作字段的菜单
    QWidget *menuWidget = new QWidget(fieldsWidget);
    menuWidget->setFixedHeight(30);
    QHBoxLayout *menuLayout = new QHBoxLayout(menuWidget);
    menuLayout->setContentsMargins(0,0,0,0);


    QPushButton *addFieldBtn = new QPushButton(menuWidget);
    connect(addFieldBtn,&QPushButton::clicked,this,[this](){
        MParamsField * field = new MParamsField();
        field->currentRow = tableWidget->rowCount();
        m_task->fieldCount +=1;

        field->fieldName =QString("字段_%1").arg(m_task->fieldCount);
        field->fieldVal = "默认的字段数据。";
        m_params->pfs.append(field);

        addField(field);
        tableWidget->scrollToBottom();
    });

    addFieldBtn->setCursor(Qt::PointingHandCursor);
    addFieldBtn->setText(tr("添加字段"));
    addFieldBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    addFieldBtn->setFixedSize(80,22);
    menuLayout->addWidget(addFieldBtn,0,Qt::AlignLeft);


    // 第二行 字段tablewidget

    tableWidget = new QTableWidget(fieldsWidget);

//    tableWidget->setFrameShape(QFrame::NoFrame);//设置无边框
    tableWidget->setFocusPolicy(Qt::NoFocus); //去除虚边框
    tableWidget->setStyleSheet(".QTableWidget{border: 1px solid rgb(217,217,217); border-radius: 0px;padding: 1px;}");

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<"字段名称"<<"示例数据"<<"操作");
    tableWidget->verticalHeader()->setVisible(true);
    tableWidget->horizontalHeader()->setVisible(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中一行(整行选中)
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表格


    fieldsLayout->addWidget(menuWidget);
    fieldsLayout->addWidget(tableWidget);
    boxLayout->addWidget(fieldsWidget);


}
void MFlowRightExtractWidget::addField(MParamsField *field){

    tableWidget->setRowCount(field->currentRow+1);
    tableWidget->setRowHeight(field->currentRow,36);


    QTableWidgetItem *itemName = new QTableWidgetItem(field->fieldName);
    tableWidget->setItem(field->currentRow,0,itemName);
    QTableWidgetItem *itemVal = new QTableWidgetItem(field->fieldVal);
    tableWidget->setItem(field->currentRow,1,itemVal);

    MFieldMenu *menu = new MFieldMenu(tableWidget);
    tableWidget->setCellWidget(field->currentRow,2,menu);

    connect(menu->editBtn,&QToolButton::clicked, [this,field,itemName,itemVal](){

        MFieldEditDialog editDlg(field,this);
        editDlg.move(QCursor::pos());
        editDlg.exec();

        itemName->setText(field->fieldName);
        itemVal->setText(field->fieldVal);

    });
    connect(menu->delBtn,&QToolButton::clicked, [this,field,menu](){
         // 小于当前行的row不变，大于当前行的row减1
         for (int i = 0; i < m_params->pfs.count(); ++i) {
             if(m_params->pfs[i]->currentRow>field->currentRow){

                 m_params->pfs[i]->currentRow-=1;
             }
         }
         // 删除本行数据
         tableWidget->removeRow(field->currentRow);
         // 删除本行的按钮widget
         menu->deleteLater();
         // 删除本行的字段参数
         m_params->pfs.removeOne(field);
         // 删除字段参数的内存
         delete field;


 });

}

void MFlowRightExtractWidget::initSenierOptionUI(){

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




