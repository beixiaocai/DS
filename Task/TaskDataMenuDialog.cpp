#include "TaskDataMenuDialog.h"
#include "Utils/ComOptionsBox.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QFileDialog>
#include "style.h"
#include <QsLog.h>

TaskDataMenuDialog::TaskDataMenuDialog(QWidget *parent,const QString &field,int fieldIndex) : QDialog(parent),
    field(field),fieldIndex(fieldIndex)
{
    QLOG_INFO() <<"TaskDataMenuDialog::TaskDataMenuDialog() field="<<field;

    //设置模态
    setWindowModality(Qt::ApplicationModal);
    // 隐藏？号
    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setWindowTitle("高级选项");
    setStyleSheet(".TaskDataMenuDialog{background-color:rgb(246,247,251);}");

    setFixedSize(340,340);

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);

    ComOptionsBox *optionsBox = new ComOptionsBox(this,"下载选项");
    // 1
    QLabel *nameLabel = new QLabel(optionsBox->gWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel14);
    nameLabel->setText("下载字段名称");

    QLabel *nameVLabel = new QLabel(optionsBox->gWidget);
    nameVLabel->setStyleSheet(m_stylesheet_QLabel14);
    nameVLabel->setText(field);

    optionsBox->gLayout->addWidget(nameLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(nameVLabel,optionsBox->rowStart,1);
    ++optionsBox->rowStart;

    // 2

    QLabel *saveSuffixLabel = new QLabel(optionsBox->gWidget);
    saveSuffixLabel->setStyleSheet(m_stylesheet_QLabel14);
    saveSuffixLabel->setText("下载后文件后缀");

    saveSuffixLine = new QLineEdit(optionsBox->gWidget);
    saveSuffixLine->setFixedSize(100,30);
    saveSuffixLine->setText(".jpg");
    saveSuffixLine->setStyleSheet(m_stylesheet_QLineEdit);
    saveSuffixLine->setClearButtonEnabled(true);
    saveSuffixLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    optionsBox->gLayout->addWidget(saveSuffixLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(saveSuffixLine,optionsBox->rowStart,1);
    ++optionsBox->rowStart;


    // 3
    QLabel *threadCountLabel = new QLabel(optionsBox->gWidget);
    threadCountLabel->setStyleSheet(m_stylesheet_QLabel14);
    threadCountLabel->setText("下载线程");

    threadCountSpin = new QSpinBox(optionsBox->gWidget);
    threadCountSpin->setMinimum(1);
    threadCountSpin->setMaximum(1);
//    threadCountSpin->setSuffix("并发");
    threadCountSpin->setMaximumWidth(100);
    threadCountSpin->setStyleSheet(m_stylesheet_QSpinBox);
    threadCountSpin->setValue(1);

    optionsBox->gLayout->addWidget(threadCountLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(threadCountSpin,optionsBox->rowStart,1);
    ++optionsBox->rowStart;

    // 4
    QLabel *threadIntervalLabel = new QLabel(optionsBox->gWidget);
    threadIntervalLabel->setStyleSheet(m_stylesheet_QLabel14);
    threadIntervalLabel->setText("线程休眠间隔");

    threadIntervalSpin = new QSpinBox(optionsBox->gWidget);
    threadIntervalSpin->setMinimum(1);
    threadIntervalSpin->setMaximum(30);
    threadIntervalSpin->setSuffix("秒");
    threadIntervalSpin->setMaximumWidth(100);
    threadIntervalSpin->setStyleSheet(m_stylesheet_QSpinBox);
    threadIntervalSpin->setValue(1);

    optionsBox->gLayout->addWidget(threadIntervalLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(threadIntervalSpin,optionsBox->rowStart,1);
    ++optionsBox->rowStart;


    // 5
    QLabel *dirLabel = new QLabel(optionsBox->gWidget);
    dirLabel->setStyleSheet(m_stylesheet_QLabel14);
    dirLabel->setText("请选择下载路径");

    QPushButton *dirBtn= new QPushButton(optionsBox->gWidget);
    dirBtn->setStyleSheet( ".QPushButton {color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:13px;background-color:white; border:1px solid rgb(64,65,66); border-radius: 0px;padding: 0px;}\
                           .QPushButton:pressed {background-color: rgba(240,240,240,0.8);}\
                           .QPushButton:hover {background-color: rgba(240,240,240,0.4); border-color:rgba(64,65,66,0.5);}\
                           .QPushButton:focus{outline: none;}");
    dirBtn->setFixedSize(140,22);
    dirBtn->setText("选择");
    connect(dirBtn,&QPushButton::clicked,this,[this,dirBtn](){
        QString dir = QFileDialog::getExistingDirectory(this,"选择文件夹","");
        if(dir.length()>0){
            saveDir = dir;
            dirBtn->setText(dir.mid(0,15)+"...");
        }

    });

    optionsBox->gLayout->addWidget(dirLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(dirBtn,optionsBox->rowStart,1);
    ++optionsBox->rowStart;

    //下载按钮
    QWidget * bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(30);
    QHBoxLayout *bottomHLayout = new QHBoxLayout(bottomWidget);
    bottomHLayout->setContentsMargins(0,0,0,0);


    downloadBtn = new QPushButton(this);
    downloadBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    downloadBtn->setFixedSize(100,28);
    downloadBtn->setText("开始下载");

    bottomHLayout->addStretch(10);
    bottomHLayout->addWidget(downloadBtn);
    bottomHLayout->addStretch(10);


    boxLayout->addSpacing(10);
    boxLayout->addWidget(optionsBox);
    boxLayout->addSpacing(2);
    boxLayout->addWidget(bottomWidget);
    boxLayout->addStretch(10);
}
TaskDataMenuDialog::~TaskDataMenuDialog(){
    QLOG_INFO() <<"TaskDataMenuDialog::~TaskDataMenuDialog() field="<<field;
}
