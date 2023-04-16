#include "TaskDataExportDialog.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComMessageBox.h"
#include "Utils/database.h"
#include "Utils/ComOptionsBox.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QDateTime>
#include <QTimer>
#include <QFileDialog>
#include <QSpinBox>
#include "xlsxdocument.h"
#include <QsLog.h>
#include <QDebug>

QXLSX_USE_NAMESPACE


TaskDataExportDialog::TaskDataExportDialog(QWidget *parent,const QString &taskName,const QString &taskCode) :
    QDialog(parent),mTaskName(taskName),mTaskCode(taskCode){

    QLOG_INFO() <<"TaskDataExportDialog::TaskDataExportDialog()";
    //设置模态
    setWindowModality(Qt::ApplicationModal);
    // 隐藏？号
    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setFixedSize(520,260);
    setStyleSheet(".TaskDataExportDialog{background-color:rgb(255,255,255);}");
    setWindowTitle("导出："+taskName);

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(20,10,20,0);

    ComOptionsBox *optionsBox = new ComOptionsBox(this,"导出选项");

    // 选择导出方式start

    xlsxCb = new QCheckBox(optionsBox->gWidget);
    xlsxCb->setText("Excel(xlsx)");
    xlsxCb->setChecked(true);

    fileCb = new QCheckBox(optionsBox->gWidget);
    fileCb->setText("文件");


    optionsBox->gLayout->addWidget(xlsxCb,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(fileCb,optionsBox->rowStart,1);
    ++optionsBox->rowStart;

     // 选择导出方式end

    //每一个sheet的最大长度 start
    unitLabel = new QLabel(optionsBox->gWidget);
    unitLabel->setStyleSheet(m_stylesheet_QLabel);
    unitLabel->setText("每一个sheet的最大长度");

    unitSpin = new QSpinBox(optionsBox->gWidget);
    unitSpin->setMinimum(4000);
    unitSpin->setMaximum(5000);
//    unitSpin->setSuffix("");
    unitSpin->setMaximumWidth(160);
    unitSpin->setStyleSheet(m_stylesheet_QSpinBox);
    unitSpin->setValue(10);

    optionsBox->gLayout->addWidget(unitLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(unitSpin,optionsBox->rowStart,1);
    ++optionsBox->rowStart;

    //每一个sheet的最大长度 end

    // 选择下载路径 start
    QLabel *dirLabel = new QLabel(optionsBox->gWidget);
    dirLabel->setStyleSheet(m_stylesheet_QLabel14);
    dirLabel->setText("选择导出路径*");

    QPushButton *dirBtn= new QPushButton(optionsBox->gWidget);
    dirBtn->setStyleSheet( ".QPushButton {color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:13px;background-color:white; border:1px solid rgb(64,65,66); padding: 0px;}\
                           .QPushButton:pressed {background-color: rgba(240,240,240,0.8);}\
                           .QPushButton:hover {background-color: rgba(240,240,240,0.4); border-color:rgba(64,65,66,0.5);}\
                           .QPushButton:focus{outline: none;}");
    dirBtn->setFixedSize(240,26);
    dirBtn->setText("选择");
    connect(dirBtn,&QPushButton::clicked,this,[this,dirBtn](){
        QString dir = QFileDialog::getExistingDirectory(this,"选择文件夹","");
        if(dir.length()>0){
            mSaveDir = dir;
            dirBtn->setText(dir.mid(0,30)+"...");
        }

    });

    optionsBox->gLayout->addWidget(dirLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(dirBtn,optionsBox->rowStart,1);
    ++optionsBox->rowStart;
     // 选择下载路径 end

    // 底部菜单
    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomHLayout = new QHBoxLayout(bottomWidget);
    bottomHLayout->setContentsMargins(0,0,0,0);
    bottomHLayout->setSpacing(0);

    loadingLabel = new ComLoadingLabel(bottomWidget);
    loadingMsgLabel = new QLabel(bottomWidget);
    loadingMsgLabel->setStyleSheet(m_stylesheet_QLabel12);

    QPushButton *okBtn = new QPushButton(bottomWidget);
    okBtn->setText("确定");
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setStyleSheet(m_stylesheet_QPushButton);
    okBtn->setFixedSize(80,28);


    connect(xlsxCb,&QCheckBox::clicked,this,[this](bool checked){
        if(checked){
            unitLabel->setText("每一个sheet的最大长度");
            fileCb->setChecked(false);
        }else{
            fileCb->setChecked(true);
        }
     });
    connect(fileCb,&QCheckBox::clicked,this,[this](bool checked){
        if(checked){
            unitLabel->setText("每一个文件的最大长度");
            xlsxCb->setChecked(false);
        }else{
            xlsxCb->setChecked(true);
        }
     });


    connect(okBtn,&QPushButton::clicked,this,[this](){
        if(mSaveDir.isEmpty()){
            ComMessageBox::error(this,"请选择导出路径");
            return;
        }
        if(xlsxCb->isChecked()){
            exportXlsx();
        }else if(fileCb->isChecked()){
            ComMessageBox::error(this,"暂不支持文件导出格式");
        }else{
            ComMessageBox::error(this,"不支持的导出格式");
        }
    });

    QPushButton *cancelBtn = new QPushButton(bottomWidget);
    cancelBtn->setText("取消");
    cancelBtn->setCursor(Qt::PointingHandCursor);
    cancelBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    cancelBtn->setFixedSize(80,28);
    connect(cancelBtn,&QPushButton::clicked,this,[this](){
        this->close();
    });

    bottomHLayout->addStretch(10);
    bottomHLayout->addWidget(loadingLabel);
    bottomHLayout->addSpacing(10);
    bottomHLayout->addWidget(loadingMsgLabel);
    bottomHLayout->addSpacing(10);
    bottomHLayout->addWidget(okBtn);
    bottomHLayout->addSpacing(10);
    bottomHLayout->addWidget(cancelBtn);
    bottomHLayout->addSpacing(20);


    boxLayout->addWidget(optionsBox);
    boxLayout->addStretch(10);
    boxLayout->addWidget(bottomWidget);
    boxLayout->addSpacing(10);
}

TaskDataExportDialog::~TaskDataExportDialog(){
    QLOG_INFO() <<"TaskDataExportDialog::~TaskDataExportDialog()";
}


void TaskDataExportDialog::exportXlsx(){
    if(!mSaveDir.isEmpty()){
        // 执行导出文件
        loadingLabel->show();
        loadingMsgLabel->setText("");
        QTimer::singleShot(200,[this](){
            int sheetSize = unitSpin->value();
            // 查询数据
            QStringList fields = Database::getInstance()->getTableFields(mTaskCode);
            int queryColumnCount = fields.length();

            int dbsize = 30;// 每页查询数据的数量
            int dbstart = 0;// 本页查询起始点
            int dbtotal = 0;// 当前数据库总数量

            int sheetNum = 1; // 起始sheet
            int sheetRow = 1; //当前sheet起始行

            Document xlsx;
            xlsx.addSheet("sheet"+QString::number(sheetNum));
            xlsx.setColumnWidth(1,queryColumnCount,20);

            for (int column = 1; column <= queryColumnCount; ++column) {
                xlsx.write(sheetRow,column,fields[column-1]);
            }

            while (true) {
                QVector<QVector<QString>> data = Database::getInstance()->select(queryColumnCount,
                        QString("select %1 from %2 limit %3,%4 ").arg(fields.join(",")).arg(mTaskCode).arg(dbstart).arg(dbsize)
                     );
                for (int d = 0; d < data.length(); ++d) {
                    dbtotal +=1;
                    sheetRow +=1;
                    for (int column = 1; column <= queryColumnCount; ++column) {
                        xlsx.write(sheetRow,column,data[d][column-1]);
                    }
                    if(sheetRow % sheetSize == 0){
                        sheetNum +=1;
                        sheetRow = 0;
                        xlsx.addSheet("sheet"+QString::number(sheetNum));
                        xlsx.setColumnWidth(1,queryColumnCount,40);
                    }
                }
                if(data.length()<dbsize){
                    break;
                }else{
                    dbstart +=dbsize;
                }
            }
            QString filename = QString("%1/%2-%3.xlsx").
                   arg(mSaveDir).
                   arg(mTaskName).
                   arg(QDateTime::currentDateTime().toLocalTime().toString("yyyyMMddhhmm"));

            loadingLabel->hide();
            xlsx.saveAs(filename);
            loadingMsgLabel->setText(QString("导出完成（共计%1条数据）").arg(dbtotal));

//           this->close();
        });
    }

}
