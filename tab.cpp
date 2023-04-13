#include "tab.h"
#include "Index/Index.h"
#include "Index/Settings.h"
#include "Index/About.h"
#include "Task/Task.h"
#include "Task/TaskManage.h"
#include "Task/TaskData.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include <QLabel>
#include <QTabBar>
#include <QMenu>
#include <QRandomGenerator>
//#include <QsLog.h>
Tab::Tab(QWidget *parent) : QTabWidget(parent)
{

    QString style =  "QTabWidget::pane{} \
              QTabBar::tab {color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:13px;min-width:110px;height:30px;padding:0px 5px;border-radius:4px;margin-left:6px;margin-top:4px;margin-bottom:2px;} \
              QTabBar::tab:!selected {background:rgb(255,255,255);border:1px solid rgb(233,233,233);} \
              QTabBar::tab:hover{background:rgba(255,255,255,0.8);border-radius: 0px;} \
              QTabBar::tab:selected  {background:rgb(219,228,241);border-radius: 0px;}";

    setStyleSheet(style);
    QTabBar *tabBar = this->tabBar();
    tabBar->setFocusPolicy(Qt::NoFocus);//去除虚线框

    tabBar->setTabsClosable(true);
    tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    tabBar->setMovable(true);
    tabBar->setContextMenuPolicy(Qt::CustomContextMenu);

//    connect(tabBar, &QTabBar::customContextMenuRequested, this, &Tab::onHandleContextMenuRequested);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &Tab::onCloseTab);
    setDocumentMode(true);
//    setElideMode(Qt::ElideRight);

}


void Tab::createIndex(){

    bool create = true;
    for (int i = 0; i < this->count(); ++i) {
        if("Index"==QString(this->widget(i)->metaObject()->className())){
            create = false;
            this->setCurrentIndex(i);
            break;
        }
    }
    if(create){
        Index *page = new Index(this);
        addTab(page,formatTabName("首页"));
        setCurrentWidget(page);
        connect(page,&Index::notifyCreateTask,this,[this](const QString address){
            createTask(address);
        });
    }
}
void Tab::createSettings(){

    bool create = true;
    for (int i = 0; i < this->count(); ++i) {
        if("Settings"==QString(this->widget(i)->metaObject()->className())){
            create = false;
            this->setCurrentIndex(i);
            break;
        }
    }
    if(create){
        Settings *page = new Settings(this);
        addTab(page,formatTabName("设置"));
        setCurrentWidget(page);
    }
}
void Tab::createAbout(){

    bool create = true;
    for (int i = 0; i < this->count(); ++i) {
        if("About"==QString(this->widget(i)->metaObject()->className())){
            create = false;
            this->setCurrentIndex(i);
            break;
        }
    }
    if(create){
        About *page = new About(this);
        addTab(page,formatTabName("关于"));
        setCurrentWidget(page);
    }
}

void Tab::createTask(const QString address){
    // 新增任务
   int r = QRandomGenerator::global()->bounded(100000,999999);
//    qsrand(time(NULL));
//    int r = qrand() % 1000000 + 1000000;

    QString code = "code"+QString::number(r);
    MTask *task = new MTask();
    task->name = "新建任务";
    task->code = code;
    task->defaultUserAgent = Database::getInstance()->getRandomUserAgent();

    if(!address.isNull()){
        task->addressList = address.split("\n");
    }

    setTask(task);


}
void Tab::createTaskManage(){
    bool create = true;
    for (int i = 0; i < this->count(); ++i) {
        if("TaskManage"==QString(this->widget(i)->metaObject()->className())){
            create = false;
            this->setCurrentIndex(i);
            break;
        }
    }


    if(create){
        TaskManage *page = new TaskManage(this);
        addTab(page,formatTabName("任务管理"));
        setCurrentWidget(page);
        connect(page,&TaskManage::notifyPageTaskData,this,[this](const QString &taskName,const QString &taskCode){
            TaskData *page = new TaskData(this,taskName,taskCode);
            addTab(page,formatTabName(taskName));
            setCurrentWidget(page);
        });

        connect(page,&TaskManage::notifyEditCustomTask,this,[this](MTask *task){
            // 编辑任务
            setTask(task);
        });
    }

}
QString Tab::formatTabName(const QString &name){
    if(name.length()>15){
        return " "+name.mid(0,15) + "...  ";
    }else{
        return " "+name+"  ";
    }
}
void Tab::setTask(MTask *task){
    Task *page = new Task(this,task);
    addTab(page,formatTabName(task->name));

    setCurrentWidget(page);
    connect(page,&Task::notifyChangeTabName,this,[this,page](const QString &name){
       this->setTabText(this->indexOf(page),formatTabName(name));

    });
    connect(page,&Task::notifyChangeTabIcon,this,[this,page](const QIcon &icon){
       this->setTabIcon(this->indexOf(page),icon);

    });

}
void Tab::onCloseTab(int index){
    QWidget *page = widget(index);
    if("Index"==QString(page->metaObject()->className())){

    }else{
        removeTab(index);

        page->deleteLater();
//        delete currentWidget;

    //    if (WebView *view = webView(index)) {
    //        bool hasFocus = view->hasFocus();
    //        removeTab(index);
    //        if (hasFocus && count() > 0)
    //            currentWebView()->setFocus();
    //        if (count() == 0)
    //            createTab();
    //        view->deleteLater();
    //    }


    }


}



