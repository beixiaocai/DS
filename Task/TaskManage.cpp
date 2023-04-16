#include "TaskManage.h"
#include "TaskData.h"
#include "TaskImport.h"
#include "TaskDataExportDialog.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "Utils/ComLoadingLabel.h"
#include "Utils/ComLoadingWidget.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComMessageBox.h"
#include "Run/StartupDialog.h"
#include "style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStyledItemDelegate>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QsLog.h>

TaskManage::TaskManage(QWidget *parent) :QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,2,0,0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(new ComLineWidget(this));

    QWidget * boxWidget = new QWidget(this);
    mainLayout->addWidget(boxWidget);

    boxLayout = new QVBoxLayout(boxWidget);
    boxLayout->setContentsMargins(20,0,20,0);
    boxLayout->setSpacing(0);


    initTopMenuUi();
    initRowMenu();
    initTableUi();

    loading = new ComLoadingWidget(this);
    getTasks();
}
TaskManage::~TaskManage(){

    for (int i = 0; i < mTasks.length(); ++i) {
        delete mTasks[i];
        mTasks[i] = nullptr;
    }
    mTasks.clear();
    mCurTask = nullptr;

}
void TaskManage::initTopMenuUi(){

    QWidget *menuWidget = new QWidget(this);
    menuWidget->setFixedHeight(50);
    QHBoxLayout *menuHLayout = new QHBoxLayout(menuWidget);
    menuHLayout->setContentsMargins(0,0,0,0);
    menuHLayout->setSpacing(0);

    groupComboBox = new QComboBox(menuWidget);
    groupComboBox->setFixedSize(150,30);
    QStyledItemDelegate *groupDelegate = new QStyledItemDelegate(groupComboBox);
    groupComboBox->setItemDelegate(groupDelegate);
    groupComboBox->setStyleSheet(m_stylesheet_QComboBox);
    groupComboBox->clear();

    m_taskGroups = Database::getInstance()->taskGroups;

    for (int i = 0; i < m_taskGroups.length(); ++i) {
        if(i==0){
            mCurGroupId = m_taskGroups[i].id;
        }
        groupComboBox->addItem(m_taskGroups[i].name);
    }

    connect(groupComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [this](int index){
         mCurGroupId = m_taskGroups[index].id;

    });

    refreshBtn = new QPushButton(menuWidget);
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    refreshBtn->setText("刷新");
    refreshBtn->setFixedSize(90,28);
    connect(refreshBtn,&QPushButton::clicked,this,[this](){
        getTasks();
    });
    QPushButton *importBtn = new QPushButton(menuWidget);
    importBtn->setCursor(Qt::PointingHandCursor);
    importBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    importBtn->setText("导入任务");
    importBtn->setFixedSize(90,28);
    connect(importBtn,&QPushButton::clicked,this,[this](){
        TaskImport dlg(this);
        dlg.exec();
    });


    loadingLabel = new ComLoadingLabel(this);

    menuHLayout->addWidget(groupComboBox);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(refreshBtn);
    menuHLayout->addSpacing(10);
    menuHLayout->addWidget(importBtn);
    menuHLayout->addSpacing(20);
    menuHLayout->addWidget(loadingLabel);
    menuHLayout->addStretch(10);
    boxLayout->addWidget(menuWidget);
}
void TaskManage::initRowMenu(){
    rowMenu = new QMenu(this);
//    rowMenu->setStyleSheet(m_stylesheet_QMenu);
    QAction *executeA = rowMenu->addAction(tr("执行任务"));
    QAction *editA =  rowMenu->addAction(tr("编辑任务"));
    QAction *delA =   rowMenu->addAction(tr("删除任务"));
    QAction *exportA= rowMenu->addAction(tr("导出任务"));
    QAction *dataA =  rowMenu->addAction(tr("管理数据"));
    QAction *exportdataA = rowMenu->addAction(tr("导出数据"));


    rowMenu->addAction(executeA);
    rowMenu->addSeparator();
    rowMenu->addAction(editA);
    rowMenu->addSeparator();
    rowMenu->addAction(delA);
    rowMenu->addSeparator();
    rowMenu->addAction(exportA);
    rowMenu->addSeparator();
    rowMenu->addAction(dataA);
    rowMenu->addSeparator();
    rowMenu->addAction(exportdataA);

    connect(executeA, &QAction::triggered, this,[this](){
        // 执行任务
        m_useType = GetTaskUseType::ExecuteTask;
        getTask(mCurTask->code);

    });
    connect(editA, &QAction::triggered, this,[this](){
        // 编辑任务
        m_useType = GetTaskUseType::EditTask;
        getTask(mCurTask->code);

    });

    connect(delA, &QAction::triggered, this,[this](){
        switch (QMessageBox::information(this,"提示","确认要删除吗？","确定","取消",0)){
            case 0:{
                    QString msg;
                    bool state = Database::getInstance()->delTask(mCurTask->code,msg);
                    this->onDelTask(state,msg);
                    break;
                }
            case 1:
                break;
        }
    });
    connect(exportA, &QAction::triggered, this,[this](){
        QString dir = QFileDialog::getExistingDirectory(this,"选择文件夹","");
        if(dir.length()>0){
            // 执行导出文件
            m_exportTaskDir = dir;
            m_useType = GetTaskUseType::ExportTask;
            getTask(mCurTask->code);

        }
    });

    connect(dataA, &QAction::triggered, this,[this](){
        // 查看数据
        emit notifyPageTaskData(mCurTask->name,mCurTask->code);
    });
    connect(exportdataA, &QAction::triggered, this,[this](){
        // 导出数据
        TaskDataExportDialog *dlg = new TaskDataExportDialog(this,mCurTask->name,mCurTask->code);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();
    });


}

void TaskManage::initTableUi(){

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    tableWidget->setColumnWidth(0,80);
    tableWidget->setColumnWidth(2,270);
    tableWidget->setColumnWidth(3,100);
    tableWidget->setColumnWidth(4,150);
    tableWidget->setAlternatingRowColors(true);


    tableWidget->setFrameShape(QFrame::NoFrame);//设置无边框
    tableWidget->setFocusPolicy(Qt::NoFocus); //去除虚边框
    tableWidget->setShowGrid(false);//隐藏栅格
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:13px;background:rgb(251,252,255);alternate-background-color:rgba(217,231,255,0.4);gridline-color:rgb(251,252,255);}\
       QTableWidget::item{margin-left:10px;}\
       QTableWidget::item:selected{color:rgb(0,0,0);text-align:center;background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:0,stop:0 rgba(0,0,0,0),stop:1 rgba(0,0,0,0));}\
       QTableWidget::item:hover{background:rgba(0,0,0,0);}\
       QHeaderView::section{text-align:center;background:rgb(64,65,66);color:rgb(255,255,255);padding:3px;border:1px solid #242424;border-left-width:0;}\
       QScrollBar::vertical{background:#484848;padding:0px;border-radius:0px;max-width:8px;}\
       QScrollBar::handle:vertical{background:#CCCCCC;}\
       QScrollBar::handle:hover:vertical,QScrollBar::handle:pressed:vertical{background:#A7A7A7;}\
       QScrollBar::sub-page:vertical{background:444444;}\
       QScrollBar::add-page:vertical{background:5B5B5B;}\
       QScrollBar::add-line:vertical{background:none;}\
       QScrollBar::sub-line:vertical{background:none;}");

//  tabWidget->setTabPosition(QTabWidget::North);//设置选项卡的方位东南西北，默认在上方

    tableWidget->setHorizontalHeaderLabels(QStringList()<<"序号"<<"任务名"<<"采集状态"<<"任务组"<<"最近编辑时间"<<"操作");
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setVisible(true);
//    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中一行(整行选中)
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表格

    connect(tableWidget,&QTableWidget::itemClicked,this,[this](QTableWidgetItem *item){
        if(item->column()==5){
             mCurTask = mTasks[item->row()];
             rowMenu->exec(QCursor::pos());
        }

    });
//    connect(tableWidget,&QTableWidget::clicked,this,[](const QModelIndex &index){
//        QLOG_INFO()<<"QTableWidget::clicked"<<index;;
//    });

    boxLayout->addWidget(tableWidget);
}


void TaskManage::getTasks(){
    tableWidget->setRowCount(0);
    tableWidget->clearContents();
    refreshBtn->setEnabled(false);
    loadingLabel->show();

    // 当前数据销毁
    for (int i = 0; i < mTasks.length(); ++i) {
        delete mTasks[i];
        mTasks[i] = nullptr;
    }
    mTasks.clear();
    mCurTask = nullptr;

    QTimer::singleShot(500,this,[this](){
        // 请求数据
        QString msg;
        bool state = Database::getInstance()->getTasks(mCurGroupId,msg,mTasks);
        loadingLabel->hide();
        refreshBtn->setEnabled(true);

        if(state){
            showTasks();
        }else{
            // 获取失败
            ComMessageBox::error(this,msg);
        }
    });
}
void TaskManage::showTasks(){
    if(mTasks.length()==0){
        tableWidget->setRowCount(0);
        ComMessageBox::error(this,"暂无数据");

    }else {
        QTableWidgetItem *item = new QTableWidgetItem();
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(10);
        item->setFont(font);

        QIcon ic(":/res/images/icon/user/menu.png");
        QIcon state_ok(":/res/images/icon/success.png");
        QIcon state_error(":/res/images/icon/error.png");
        QIcon state_stop(":/res/images/icon/stop.png");


        tableWidget->setRowCount(mTasks.length());
        for (int i = 0; i < mTasks.length(); ++i) {
            MTask *task = mTasks[i];
            task->tempIndex = i;
            int row = i;
            tableWidget->setRowHeight(row,70);
            QTableWidgetItem *indexItem = item->clone();
            indexItem->setText(QString::number(row+1));

            tableWidget->setItem(row,0,indexItem);


            QTableWidgetItem *nameItem = item->clone();
            nameItem->setText(task->name);
            tableWidget->setItem(row,1,nameItem);
            QTableWidgetItem *stateItem = item->clone();

            if(task->tempIsRun){
                if (task->tempData.state==MTaskData::FINISH){
                    stateItem->setIcon(state_ok);
                    stateItem->setText(QString("已完成 已采集%1条,重复%2条").arg(task->tempData.num).arg(task->tempData.repeatNum));//rgb(76,175,80)
                }else {
                    stateItem->setIcon(state_stop);
                    stateItem->setText(QString("已停止 已采集%1条,重复%2条").arg(task->tempData.num).arg(task->tempData.repeatNum));//rgb(255,80,80)
                }

            }else{
                stateItem->setIcon(state_error);
                stateItem->setText(tr("未运行 未采集到数据"));//rgb(0,0,0)
            }


            tableWidget->setItem(row,2,stateItem);
            QTableWidgetItem *groupItem = item->clone();
            groupItem->setText(task->groupName);
            tableWidget->setItem(row,3,groupItem);


            QTableWidgetItem *lastUpdateItem = item->clone();
            lastUpdateItem->setText(task->lastUpdateTime);
            tableWidget->setItem(row,4,lastUpdateItem);

            QTableWidgetItem *handleItem = item->clone();
            handleItem->setIcon(ic);
            tableWidget->setItem(row,5,handleItem);
        }
        tableWidget->scrollToBottom();
    }
}
void TaskManage::onDelTask(bool state,QString &msg){
    if(state){
        Database::getInstance()->delTaskData(mCurTask->code);

        // 小于当前行的row不变，大于当前行的row减1
        for (int i = 0; i < mTasks.count(); ++i) {
            if(mTasks[i]->tempIndex > mCurTask->tempIndex){
                mTasks[i]->tempIndex-=1;
            }
        }
        tableWidget->removeRow(mCurTask->tempIndex);//删除table中的显示
        mTasks.removeAt(mCurTask->tempIndex);
        delete mCurTask;
        mCurTask = nullptr;

//        tableWidget->scrollToBottom();

       ComMessageBox::success(this,msg);
    }else{
        ComMessageBox::error(this,msg);
    }

}
void TaskManage::getTask(const QString &code){
    QString msg;
    MTask * task = new MTask;
    loading->resize(this->size());
    loading->show();
    bool state = Database::getInstance()->getTask(code,msg,task);
    // 返回结果
    loading->hide();
    if(state){
        if(m_useType==GetTaskUseType::ExecuteTask){
            // 执行任务
            StartupDialog *startup = new StartupDialog(task);// 交给StartupDialog释放task
            startup->setAttribute(Qt::WA_DeleteOnClose);
            startup->show();
//             StartupDialog dlg(task);
//             dlg.exec();

        }else if (m_useType==GetTaskUseType::EditTask) {
            // 编辑任务
            emit notifyEditCustomTask(task);// 交给编辑界面释放task

        }else if (m_useType==GetTaskUseType::ExportTask) {
            // 导出任务
            QString filename = QString("%1/%2.ds").arg(m_exportTaskDir).arg(task->name);
            task->toFile(filename);
            delete task;
            task = nullptr;
            ComMessageBox::success(this,filename);

        }else{
            delete task;
            task = nullptr;
            ComMessageBox::error(this,msg);
        }
    }else{
        delete task;
        task = nullptr;
        ComMessageBox::error(this,msg);
    }
}


