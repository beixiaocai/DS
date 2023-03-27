#include "TaskData.h"
#include "Utils/database.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLineWidget.h"
#include "Task/TaskDataExport.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QTimer>
#include <QDebug>

const static int PAGESIZE = 30; // 执行页面的数据分页数

TaskData::TaskData(const QString &taskName,const QString &taskCode,QWidget *parent) : QWidget(parent)
{
    m_taskName = taskName;
    m_taskCode = taskCode;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,2,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(new ComLineWidget(this));

    QWidget * boxWidget = new QWidget(this);
    mainLayout->addWidget(boxWidget);
    boxLayout = new QVBoxLayout(boxWidget);

    boxLayout->setContentsMargins(20,0,20,0);
    boxLayout->setSpacing(0);

    initMenuUi();
    initTableWidget();

    m_tbfields = Database::getInstance()->getTableFields(taskCode);

    // 设置header（这里是数据库的字段名称为准）
    tableWidget->setColumnCount(m_tbfields.length());
    tableWidget->setHorizontalHeaderLabels(m_tbfields);


    getData();

}
TaskData::~TaskData(){

}

void TaskData::getData(int p){
    m_currentPage = p;
    tableWidget->setRowCount(0);
    tableWidget->clearContents();
    loadingLabel->show();

    QTimer::singleShot(500,this,[this](){
      updateTurnPage(Database::getInstance()->getTableRowCount(m_taskCode),false);
      int queryColumnCount = m_tbfields.length();

      QVector<QVector<QString>> data = Database::getInstance()->select(queryColumnCount,
              QString("select %1 from %2 limit %3,%4 ").arg(m_tbfields.join(",")).arg(m_taskCode).arg(PAGESIZE * (m_currentPage-1)).arg(PAGESIZE)
           );

      tableWidget->setRowCount(data.length());
      QTableWidgetItem *item;
      for (int row = 0; row < data.length(); ++row) {
          tableWidget->setRowHeight(row,30);
          for (int column = 0; column < queryColumnCount; ++column) {
              item = new QTableWidgetItem(data[row][column]);
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
    rLabel->setText(m_taskName);



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

            Database::getInstance()->clearTable(m_taskCode);
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
void TaskData::initTableWidget(){
    tableWidget = new QTableWidget(this);
    tableWidget->setFrameShape(QFrame::NoFrame);//设置无边框
    tableWidget->setFocusPolicy(Qt::NoFocus); //去除虚边框
    QString style = ".QTableWidget{border:1px solid rgb(217,217,217); border-radius: 0px;padding: 1px;}";
    tableWidget->setStyleSheet(style);

//    tableWidget->setTabPosition(QTabWidget::North);//设置选项卡的方位东南西北，默认在上方
//    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->verticalHeader()->setVisible(true);
    tableWidget->horizontalHeader()->setVisible(true);



    QWidget *turnWidget = new QWidget(this);
    turnWidget->setFixedHeight(40);
    QHBoxLayout *turnHLayout = new QHBoxLayout(turnWidget);
    turnHLayout->setContentsMargins(10,0,10,0);

    pageLogLabel = new QLabel(this);
    pageLogLabel->setStyleSheet(m_stylesheet_QLabel);

    QToolButton *last = new QToolButton(this);
    last->setText("<");
    pageLabel = new QLabel(this);
    pageLabel->setStyleSheet(m_stylesheet_QLabel);
    QToolButton *next = new QToolButton(this);
    next->setText(">");

    connect(last,&QToolButton::clicked,this,[this](){
        if(m_currentPage>1){
            getData(m_currentPage-1);
        }
    });
    connect(next,&QToolButton::clicked,this,[this](){
        if(m_currentPage<m_totalPage){
            getData(m_currentPage+1);
        }
    });


    QPushButton *exportBtn = new QPushButton(turnWidget);
    exportBtn->setCursor(Qt::PointingHandCursor);
    exportBtn->setStyleSheet(m_stylesheet_QPushButton);
    exportBtn->setText("导出数据");
    exportBtn->setFixedSize(100,28);

    connect(exportBtn,&QPushButton::clicked,this,[this](){
        TaskDataExport *exp = new TaskDataExport(m_taskName,m_taskCode,this);
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

    boxLayout->addWidget(tableWidget);
    boxLayout->addWidget(turnWidget);
}
void TaskData::updateTurnPage(int num, bool isFinal){
    if(num==0){// num == 0 恢复为默认参数
        m_currentPage = 1;
    }
    int totalPage = num / PAGESIZE; // 总页数
    if((num % PAGESIZE) > 0){
        totalPage +=1;
    }
    m_totalPage = totalPage;

    if(isFinal){// 是否显示最后的数据对应的页面
        m_currentPage = m_totalPage;
    }

    pageLabel->setText(QString::number(m_currentPage));
    pageLogLabel->setText(QString("共计%1条，%2页").arg(num).arg(m_totalPage));
}
