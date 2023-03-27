#include "TaskDataExport.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "Utils/ComSpinWidget.h"
#include "Utils/database.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDateTime>
#include <QTimer>
#include <QFileDialog>
#include "xlsxdocument.h"
#include <QDebug>

QXLSX_USE_NAMESPACE


TaskDataExport::TaskDataExport(const QString &taskName,const QString &taskCode,QWidget *parent) : QDialog(parent){

    setFixedSize(520,200);

    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    m_taskName = taskName;
    m_taskCode = taskCode;
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".TaskDataExport{background-color:rgb(255,255,255);}");
    setWindowTitle("导出："+taskName);

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(20,10,20,0);

    // 第一行
    QWidget *titleWidget = new QWidget(this);
    QHBoxLayout *titleHLayout = new QHBoxLayout(titleWidget);
    titleHLayout->setContentsMargins(10,0,10,0);
    titleHLayout->setSpacing(0);


    QLabel *titleLabel = new QLabel(titleWidget);
    titleLabel->setStyleSheet(m_stylesheet_QLabel14);
    titleLabel->setText(tr("选择导出方式"));
    loadingLabel = new ComLoadingLabel(titleWidget);

    titleHLayout->addWidget(titleLabel);
    titleHLayout->addWidget(loadingLabel);
    titleHLayout->addStretch(10);

    boxLayout->addWidget(titleWidget);


    // 第二行
    QGroupBox *exportBox = new QGroupBox(this);
    exportBox->setStyleSheet(".QGroupBox{border:0px;}");
    QHBoxLayout *exportBoxLayout = new QHBoxLayout(exportBox);
    exportBoxLayout->setContentsMargins(10,0,10,0);
    exportBoxLayout->setSpacing(0);


    QRadioButton *xlsxRadio = new QRadioButton(exportBox);
    xlsxRadio->setText("Excel(xlsx)");
    QRadioButton *csvRadio = new QRadioButton(exportBox);
    csvRadio->setText("CSV文件");
    xlsxRadio->setChecked(true);

    exportBoxLayout->addWidget(xlsxRadio);
    exportBoxLayout->addWidget(csvRadio);
    exportBoxLayout->addStretch(10);
    QButtonGroup *gs = new QButtonGroup(this);
    gs->addButton(xlsxRadio);
    gs->addButton(csvRadio);

    boxLayout->addWidget(exportBox);
    boxLayout->addSpacing(10);



    // 第三行（第二行的附属行）

    ComSpinWidget *sheetSpin = new ComSpinWidget("每一个sheet的最大长度",2000,50000,5000,10,10,this);
    boxLayout->addWidget(sheetSpin);



    boxLayout->addStretch(10);
    boxLayout->addWidget(new ComLineWidget(this));

    // 第四行（底部菜单）
    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomHLayout = new QHBoxLayout(bottomWidget);
    bottomHLayout->setContentsMargins(0,0,0,0);
    bottomHLayout->setSpacing(0);
\
    QPushButton *okBtn = new QPushButton(bottomWidget);
    okBtn->setText("确定");
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setStyleSheet(m_stylesheet_QPushButton);
    okBtn->setFixedSize(80,28);


    connect(gs, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),[xlsxRadio,sheetSpin](QAbstractButton *button){
        QRadioButton *clickedRadio = static_cast<QRadioButton *>(button);
        if(clickedRadio==xlsxRadio){
             sheetSpin->show();
        }else{
             sheetSpin->hide();
        }

    });
    connect(okBtn,&QPushButton::clicked,this,[this,gs,xlsxRadio,sheetSpin](){
        if(gs->checkedButton()==xlsxRadio){
            exportXlsx(sheetSpin->value);
        }else{
            ComMessageBox::error(this,"暂时只支持xlsx格式导出");
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
    bottomHLayout->addWidget(okBtn,0,Qt::AlignRight);
    bottomHLayout->addSpacing(10);
    bottomHLayout->addWidget(cancelBtn,0,Qt::AlignRight);
    bottomHLayout->addSpacing(20);
    boxLayout->addWidget(bottomWidget);

}

TaskDataExport::~TaskDataExport(){

}
void TaskDataExport::exportXlsx(int sheetSize){

    // 导出xlsx
    QString dir = QFileDialog::getExistingDirectory(this,"选择文件夹","");
    if(dir.length()>0){
        // 执行导出文件
        loadingLabel->show();

        QTimer::singleShot(200,[this,dir,sheetSize](){

            // 查询数据
            QStringList tbfields = Database::getInstance()->getTableFields(m_taskCode);
            int queryColumnCount = tbfields.length();

            int dbsize = 30;// 每页查询数据的数量
            int dbstart = 0;// 本页查询起始点
            int dbtotal = 0;// 当前数据库总数量

            int sheetNum = 1; // 起始sheet
            int sheetRow = 1; //当前sheet起始行

            Document xlsx;
            xlsx.addSheet("sheet"+QString::number(sheetNum));
            xlsx.setColumnWidth(1,queryColumnCount,20);

            for (int column = 1; column <= queryColumnCount; ++column) {
                xlsx.write(sheetRow,column,tbfields[column-1]);
            }

            while (true) {
                QVector<QVector<QString>> data = Database::getInstance()->select(queryColumnCount,
                        QString("select %1 from %2 limit %3,%4 ").arg(tbfields.join(",")).arg(m_taskCode).arg(dbstart).arg(dbsize)
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
                   arg(dir).
                   arg(m_taskName).
                   arg(QDateTime::currentDateTime().toLocalTime().toString("yyyyMMddhhmm"));

           xlsx.saveAs(filename);
           loadingLabel->hide();
           ComMessageBox::success(this,QString("导出完成（共计%1条数据）").arg(dbtotal));
           this->close();
        });


    }

}
