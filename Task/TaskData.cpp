#include "TaskData.h"
#include "TaskDataExport.h"
#include "TaskDataMenuDialog.h"
#include "Utils/database.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "style.h"
#include "TaskDataDownloadManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QTableWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimer>
#include <QsLog.h>

const static int PAGESIZE = 30; // 执行页面的数据分页数
TaskData::TaskData(QWidget *parent,const QString &taskName,const QString &taskCode) : QWidget(parent),mTaskName(taskName),mTaskCode(taskCode)
{
    QLOG_INFO()<<"TaskData::TaskData()";
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,2,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(new ComLineWidget(this));

    QWidget * boxWidget = new QWidget(this);
    mainLayout->addWidget(boxWidget);
    boxLayout = new QVBoxLayout(boxWidget);

    boxLayout->setContentsMargins(20,0,20,0);
    boxLayout->setSpacing(0);

    mFields = Database::getInstance()->getTableFields(mTaskCode);

    initMenuUi();
    initTableUi();
    initBottomUi();

    getData();

}
TaskData::~TaskData(){
    QLOG_INFO()<<"TaskData::~TaskData()";
    QHash<QString,TaskDataDownloadManager *>::const_iterator it = mDownloadManagerH.constBegin();
    while (it != mDownloadManagerH.constEnd()) {
        delete it.value();
        ++it;
    }
    mDownloadManagerH.clear();

    //清理logListWidget的item
    for (int i = 0; i < logListWidgetItems_clear.count(); ++i) {
        delete logListWidgetItems_clear[i];
        logListWidgetItems_clear[i] = nullptr;
    }
    logListWidgetItems_clear.clear();

}

void TaskData::getData(int p){
    mCurPage = p;
    tableWidget->setRowCount(0);
    tableWidget->clearContents();
    loadingLabel->show();
    QTimer::singleShot(200,this,[this](){
      updatePageData(Database::getInstance()->getTableRowCount(mTaskCode),false);//更新page
      int fieldCount = mFields.length();
      QString sql = QString("select %1 from %2 limit %3,%4 ").arg(mFields.join(",")).arg(mTaskCode).arg(PAGESIZE * (mCurPage-1)).arg(PAGESIZE);
      QVector<QVector<QString>> data = Database::getInstance()->select(fieldCount,sql);

      tableWidget->setRowCount(data.length());
      for (int row = 0; row < data.length(); ++row) {
          tableWidget->setRowHeight(row,30);
          for (int column = 0; column < fieldCount; ++column) {
              QTableWidgetItem *item = new QTableWidgetItem(data[row][column]);
              tableWidget->setItem(row,column,item);
          }
      }
      tableWidget->scrollToBottom();
      loadingLabel->hide();
    });
}
void TaskData::initMenuUi(){
    QWidget *menuWidget = new QWidget(this);
    menuWidget->setFixedHeight(50);
    QHBoxLayout *menuHLayout = new QHBoxLayout(menuWidget);
    menuHLayout->setContentsMargins(0,0,0,0);


    QLabel *lLabel = new QLabel(menuWidget);
    lLabel->setStyleSheet(m_stylesheet_QLabel12);
    lLabel->setText("查看数据：");

    QLabel *rLabel = new QLabel(menuWidget);
    rLabel->setStyleSheet(m_stylesheet_QLabel14);
    rLabel->setText(mTaskName);



    loadingLabel = new ComLoadingLabel(this);


    QPushButton *refreshBtn = new QPushButton(menuWidget);
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setStyleSheet(m_stylesheet_QPushButton);
    refreshBtn->setText("刷新");
    refreshBtn->setFixedSize(80,30);
    connect(refreshBtn,&QPushButton::clicked,this,[this](){
        getData();
    });


    QPushButton *clearBtn = new QPushButton(menuWidget);
    clearBtn->setCursor(Qt::PointingHandCursor);
    clearBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    clearBtn->setText("清空");
    clearBtn->setFixedSize(80,30);

    connect(clearBtn,&QPushButton::clicked,this,[this](){
        loadingLabel->show();
        QTimer::singleShot(500,this,[this](){
            QString msg;
            Database::getInstance()->clearTaskData(mTaskCode,msg);
            tableWidget->setRowCount(0);
            tableWidget->clearContents();
            loadingLabel->hide();

        });


    });

    menuHLayout->addWidget(lLabel);
    menuHLayout->addWidget(rLabel);
    menuHLayout->addStretch(10);
    menuHLayout->addWidget(loadingLabel);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(refreshBtn);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(clearBtn);

    boxLayout->addWidget(menuWidget);
}
void TaskData::initTableUi(){
    tableWidget = new QTableWidget(this);
    tableWidget->setFrameShape(QFrame::NoFrame);//设置无边框
    tableWidget->setFocusPolicy(Qt::NoFocus); //去除虚边框
    tableWidget->setStyleSheet(".QTableWidget{border:1px solid rgb(217,217,217); border-radius: 0px;padding: 1px;}");

//    tableWidget->setTabPosition(QTabWidget::North);//设置选项卡的方位东南西北，默认在上方
//    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->verticalHeader()->setVisible(true);
    tableWidget->horizontalHeader()->setVisible(true);

    connect(tableWidget->horizontalHeader(),&QHeaderView::sectionClicked,this,[this](int fieldIndex){
        QString field = this->mFields.at(fieldIndex);
        TaskDataMenuDialog *menu = new TaskDataMenuDialog(this,field,fieldIndex);

        connect(menu->downloadBtn,&QPushButton::clicked,[this,menu](){
            if(this->mDownloadManagerH.contains(menu->field)){
                //已经在下载中
                ComMessageBox::error(this,"正在下载中");
            }else{

                if(menu->saveDir.isEmpty()){
                     ComMessageBox::error(this,"请选择下载路径");
                }else{
                    TaskDataDownloadManager *downloadManager = new TaskDataDownloadManager(this,
                                                                   this->mTaskName,this->mTaskCode,this->mFields,
                                                                   menu->threadCountSpin->value(),
                                                                   menu->threadIntervalSpin->value(),
                                                                   menu->field,
                                                                   menu->fieldIndex,
                                                                   menu->fieldPrefixLine->text().trimmed(),
                                                                   menu->saveDir,
                                                                   menu->saveSuffixLine->text().trimmed());
                    //每一个字段的下载器显示两行日志
                    QString log1 = QString("下载任务 '%1'，线程数 %2，线程休眠间隔 %3，下载路径 %4").
                            arg(menu->field).
                            arg(menu->threadCountSpin->value()).
                            arg(menu->threadIntervalSpin->value()).
                            arg(menu->saveDir);
                    QListWidgetItem * logitem1 = new QListWidgetItem(log1,logListWidget);
                    logListWidgetItems_clear.push_back(logitem1);
                    logListWidget->addItem(logitem1);


                    QListWidgetItem * logitem2 = new QListWidgetItem("开始下载",logListWidget);
                    logListWidgetItems_clear.push_back(logitem2);
                    logListWidget->addItem(logitem2);

                    connect(downloadManager, &TaskDataDownloadManager::notifyLog, this, [logitem2](const QString &field,const QString &log) {
                        logitem2->setText(log);
                    });


                    this->mDownloadManagerH.insert(menu->field,downloadManager);
                    menu->deleteLater();
                }

            }

        });
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->move(QCursor::pos());
        menu->show();

    });

    // 设置header（这里是数据库的字段名称为准）
    tableWidget->setColumnCount(mFields.length());
    tableWidget->setHorizontalHeaderLabels(mFields);
    boxLayout->addWidget(tableWidget);

}
void TaskData::initBottomUi(){
    //字段处理时日志
    logListWidget = new QListWidget(this);
    logListWidget->setStyleSheet(".QListWidget{border:1px solid rgb(217,217,217); border-radius: 0px;padding: 2px;}");
    logListWidget->setFixedHeight(80);

    //页数
    QWidget *turnWidget = new QWidget(this);
    turnWidget->setFixedHeight(40);
    QHBoxLayout *turnHLayout = new QHBoxLayout(turnWidget);
    turnHLayout->setContentsMargins(10,0,10,0);

    pageLogLabel = new QLabel(turnWidget);
    pageLogLabel->setStyleSheet(m_stylesheet_QLabel);

    QToolButton *last = new QToolButton(turnWidget);
    last->setText("<");
    pageLabel = new QLabel(this);
    pageLabel->setStyleSheet(m_stylesheet_QLabel);
    QToolButton *next = new QToolButton(turnWidget);
    next->setText(">");

    connect(last,&QToolButton::clicked,this,[this](){
        if(mCurPage>1){
            getData(mCurPage-1);
        }
    });
    connect(next,&QToolButton::clicked,this,[this](){
        if(mCurPage<mTotalPage){
            getData(mCurPage+1);
        }
    });


    QPushButton *exportBtn = new QPushButton(turnWidget);
    exportBtn->setCursor(Qt::PointingHandCursor);
    exportBtn->setStyleSheet(m_stylesheet_QPushButton);
    exportBtn->setText("导出数据");
    exportBtn->setFixedSize(100,28);

    connect(exportBtn,&QPushButton::clicked,this,[this](){
        TaskDataExport *exp = new TaskDataExport(this,mTaskName,mTaskCode);
        exp->setAttribute(Qt::WA_DeleteOnClose);
        exp->show();
    });

    turnHLayout->addWidget(pageLogLabel);
    turnHLayout->addSpacing(10);
    turnHLayout->addWidget(last);
    turnHLayout->addWidget(pageLabel);
    turnHLayout->addWidget(next);
    turnHLayout->addStretch(10);
    turnHLayout->addWidget(exportBtn,0,Qt::AlignRight);

    boxLayout->addSpacing(5);
    boxLayout->addWidget(logListWidget);
    boxLayout->addWidget(turnWidget);
}
void TaskData::updatePageData(int num, bool isShowLastPage){
    if(num==0){// num == 0 恢复为默认参数
        mCurPage = 1;
    }
    int totalPage = num / PAGESIZE; // 总页数
    if((num % PAGESIZE) > 0){
        totalPage +=1;
    }
    mTotalPage = totalPage;

    if(isShowLastPage){// 是否显示最后的数据对应的页面
        mCurPage = mTotalPage;
    }

    pageLabel->setText(QString::number(mCurPage));
    pageLogLabel->setText(QString("共计%1条，%2页").arg(num).arg(mTotalPage));
}
