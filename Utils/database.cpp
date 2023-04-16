#include "database.h"
#include "models.h"
#include <QMutex>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <Windows.h>
#include <QFile>
#include <QsLog.h>

QAtomicPointer<Database> Database::m_instance = 0;
QMutex Database::m_instancemtx;
QMutex dbmtx;

// 任务运行结果表
const QString TABLE_TASK_DATA = "ds_task_data";// 存储采集结果
const QString TABLE_TASK = "ds_task";


Database::Database(): QThread(nullptr){

    taskGroups.clear();
    userAgents.clear();

    for (int i = 0; i < 1; ++i) {
        MTaskGroup taskGroup;
        taskGroup.id = i;
        taskGroup.name = "默认组";
        taskGroups.append(taskGroup);
    }

    for (int i = 0; i < 2; ++i) {
        MTaskUserAgent ua;
        ua.id = i;
        ua.name = "win10"+QString::number(i);
        ua.userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36";
        ua.terminal = 1;
        userAgents.append(ua);
    }


    m_sqldb = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_sqldb->setDatabaseName(QApplication::applicationDirPath()+"/"+"data.db");

    if (!m_sqldb->open()){
        QLOG_INFO() << "Database.cpp failed to connect sqlite :" << m_sqldb->lastError();
    }else{
        QLOG_INFO() << "Database.cpp succee to connect sqlite" ;

        QStringList tbs = m_sqldb->tables();
        bool tb_RunResult_exist = false;// 采集结果表是否存在
        bool tb_Task_exist = false;     // 采集结果表是否存在

        for (int i = 0; i < tbs.length(); ++i) {
            if(tbs[i]==TABLE_TASK_DATA){
                tb_RunResult_exist = true;
                break;
            }
        }
        for (int i = 0; i < tbs.length(); ++i) {
            if(tbs[i]==TABLE_TASK){
                tb_Task_exist = true;
                break;
            }
        }


        if(!tb_RunResult_exist){
            dbmtx.lock();
            QSqlQuery query(*m_sqldb);
            query.exec(QString("create table %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,tbName text, num int,repeatNum int,state int,startTime text,  endTime text)").
                       arg(TABLE_TASK_DATA));
            query.clear();
            dbmtx.unlock();
        }

        if(!tb_Task_exist){
            dbmtx.lock();
            QSqlQuery query(*m_sqldb);
            QString sql = "create table %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,code text,groupId int,name text, addressList text,program text, fieldCount int,";
                        sql += "isBrowserAllowRunningInsecureContent int,isBrowserAutoLoadImages int,isBrowserAutoLoadIconsForPage int,isBrowserPluginsEnabled int,";
                        sql += "defaultUserAgent text, addTime text,lastUpdateTime text)";


            query.exec(sql.arg(TABLE_TASK));
            query.clear();
            dbmtx.unlock();
        }

        start();

    }
}
Database::~Database(){

    if(m_sqldb){
        m_sqldb->close();
        delete m_sqldb;
        m_sqldb = nullptr;
    }

    terminate();
    wait();
}
void Database::release(){
    delete m_instance;
}
int Database::mem_ratio(){
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof (statex); GlobalMemoryStatusEx (&statex);
    // 物理内存使用率
    return int(statex.dwMemoryLoad);

}

Database * Database::getInstance(){
    //! testAndSetOrders操作保证在原子操作前和后的的内存访问 不会被重新排序。
    if(m_instance.testAndSetOrdered(0,0)){
        QMutexLocker locker(&m_instancemtx);
        m_instance.testAndSetOrdered(0,new Database());
    }
    return m_instance;

}
QString Database::read_file(QString filename){
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QString content = f.readAll();
    f.close();

    return content;

}

bool Database::addTask(MTask *task,QString &msg){

    bool state = false;

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);

    int count = 0;
    QString sql_select = QString("select count(1) from %1 where code='%2' limit 1").arg(TABLE_TASK).arg(task->code);
    if(!query.exec(sql_select)){
        QLOG_INFO()<<"Database::addTask Error:"<<query.lastError()<<","<<sql_select;
    }else{
        if(query.next()){
            count = query.value(0).toInt();
        }
    }
   query.clear();

   QDateTime curTime = QDateTime::currentDateTime();
   //auto curStamp = addTime.toTime_t();
   QString timeFmt = "yyyy.MM.dd hh:mm:ss";

    if(count > 0){
        // 该任务已经存在，需要更新字段
        QString sql_update = QString("update %1 set groupId=%2,name='%3',addressList='%4',program='%5',fieldCount=%6, "
"isBrowserAllowRunningInsecureContent=%7,isBrowserAutoLoadImages=%8,isBrowserAutoLoadIconsForPage=%9,isBrowserPluginsEnabled=%10,"
"defaultUserAgent='%11',lastUpdateTime='%12'"
"where code='%13'")
.arg(TABLE_TASK).arg(task->groupId).arg(task->name).arg(task->addressList.join(",")).arg(task->program).arg(task->fieldCount)
.arg(task->isBrowserAllowRunningInsecureContent).arg(task->isBrowserAutoLoadImages).arg(task->isBrowserAutoLoadIconsForPage).arg(task->isBrowserPluginsEnabled)
.arg(task->defaultUserAgent).arg(curTime.toString(timeFmt)).arg(task->code);

        if(!query.exec(sql_update)){
            QLOG_INFO()<<"Database::addTask Error:"<<query.lastError()<<","<<sql_update;
            msg = "更新任务失败: "+query.lastError().text();
        }else{
            state = true;
             msg = "更新任务成功";
        }
         query.clear();
    }else{


        // 该任务暂不存在，首次添加
        QString sql_add = "insert into "+TABLE_TASK+" (code ,groupId,name, addressList,program,fieldCount,"
                      "isBrowserAllowRunningInsecureContent,isBrowserAutoLoadImages,isBrowserAutoLoadIconsForPage,isBrowserPluginsEnabled,"
                      "defaultUserAgent,addTime,lastUpdateTime) "
                      "values('"+task->code+"',"+QString::number(task->groupId)+",'"+task->name+"','"+task->addressList.join(",")+"','"+task->program+"',"+QString::number(task->fieldCount)+
                      ","+QString::number(task->isBrowserAllowRunningInsecureContent)+","+QString::number(task->isBrowserAutoLoadImages)+","+QString::number(task->isBrowserAutoLoadIconsForPage)+","+QString::number(task->isBrowserPluginsEnabled)+
                      ",'"+task->defaultUserAgent+"','"+curTime.toString(timeFmt)+"','"+curTime.toString(timeFmt)+"');";

        if(!query.exec(sql_add)){
            QLOG_INFO()<<"Database::addTask Error:"<<query.lastError()<<","<<sql_add;
            msg = "添加任务失败: "+query.lastError().text();
        }else{
            state = true;
             msg = "添加任务成功";
        }
         query.clear();

    }

    dbmtx.unlock();
    return state;
}
bool Database::delTask(const QString &code,QString &msg){
    bool state = false;

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);
    QString sql_del = QString("DELETE FROM '%1' where code='%2' ").arg(TABLE_TASK).arg(code);
    if(!query.exec(sql_del)){
        QLOG_INFO()<<"Database::delTask Error:"<<query.lastError()<<","<<sql_del;
        msg = "删除任务失败: "+query.lastError().text();
    }else{
        state = true;
        msg = "删除成功";
    }

    query.clear();
    dbmtx.unlock();
    return state;
}

bool Database::getTask(const QString &code,QString &msg,MTask *task){
    bool state = false;
    msg = "获取任务失败";

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);


    QString sql_select = QString("select id,code,groupId,name,addressList,program,fieldCount,isBrowserAllowRunningInsecureContent,isBrowserAutoLoadImages,isBrowserAutoLoadIconsForPage,isBrowserPluginsEnabled,defaultUserAgent,addTime,lastUpdateTime from %1 where code='%2' limit 1").arg(TABLE_TASK).arg(code);
    if(!query.exec(sql_select)){
        QLOG_INFO()<<"Database::getTaskWithCode Error:"<<query.lastError()<<","<<sql_select;
    }else{
        while(query.next()){

            task->id=query.value("id").toInt();
            task->code=query.value("code").toString();
            task->groupId=query.value("groupId").toInt();
            task->name=query.value("name").toString();
            task->type=MTask::EDIT;
            task->isHaveName = true;
            task->addressList = query.value("addressList").toString().split(",");
            task->program=query.value("program").toString();
            task->fieldCount =query.value("fieldCount").toInt();
            // 执行时参数
            task->isBrowserAllowRunningInsecureContent =query.value("isBrowserAllowRunningInsecureContent").toInt();
            task->isBrowserAutoLoadImages =query.value("isBrowserAutoLoadImages").toInt();
            task->isBrowserAutoLoadIconsForPage=query.value("isBrowserAutoLoadIconsForPage").toInt();
            task->isBrowserPluginsEnabled=query.value("isBrowserPluginsEnabled").toInt();
            task->defaultUserAgent=query.value("defaultUserAgent").toString();
            // 时间参数
            task->addTime=query.value("addTime").toString();
            task->lastUpdateTime=query.value("lastUpdateTime").toString();
            msg = "获取任务成功";

            state = true;
        }
    }
   query.clear();

   dbmtx.unlock();
   return state;
}
bool Database::getTasks(int groupId,QString &msg,QVector<MTask *> &tasks){
    bool state = false;

    QString sql_select_task = QString("select id,code,name,groupId,addTime,lastUpdateTime from %1 where groupId=%2 order by id desc ").arg(TABLE_TASK).arg(groupId);
    QString sql_select_task_data = QString("select tbName,num,repeatNum,state,startTime,endTime from %1").arg(TABLE_TASK_DATA);
    QHash<QString,MTaskData> taskDatas;

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);


    if(query.exec(sql_select_task_data)){
         while(query.next()){
             MTaskData taskData(query.value(0).toString(),
                                  query.value(1).toInt(),
                                  query.value(2).toInt(),
                                  MTaskData::FINISH,
                                  query.value(4).toString(),
                                  query.value(5).toString());
             taskDatas.insert(taskData.tbName,taskData);

         }
    }else{
        QLOG_INFO()<<"Database::getTasks Error:"<<query.lastError()<<","<<sql_select_task_data;
    }
    query.clear();

    if(query.exec(sql_select_task)){

         while(query.next()){
             MTask *task = new MTask;
             task->id = query.value(0).toInt();
             task->code = query.value(1).toString();
             task->name = query.value(2).toString();
             task->groupId = query.value(3).toInt();
             for (int i = 0; i < taskGroups.length(); ++i) {
                 if(i==task->groupId){
                     task->groupName = taskGroups[i].name;
                     break;
                 }

             }
             task->addTime = query.value(4).toString();
             task->lastUpdateTime = query.value(5).toString();

             // 为每个任务设置数据
             if(taskDatas.contains(task->code)){
                   task->tempIsRun = true;
                   task->tempData = taskDatas[task->code];
             }
             tasks.append(task);
         }
         state = true;

    }else{
        msg = QString("获取任务失败：%1").arg(query.lastError().text());
        QLOG_INFO()<<"Database::getTasks Error:"<<query.lastError()<<","<<sql_select_task;
    }
    query.clear();


    dbmtx.unlock();

    return state;
}

bool Database::createTaskData(const QString &code,QString &msg,const QStringList &fields){

    dbmtx.lock();
    bool success = false;
    QSqlQuery query(*m_sqldb);

    QString sql = "create table "+code+" (id INTEGER PRIMARY KEY AUTOINCREMENT ,period int, isRepeat int, addTime text";

    int end = fields.length()-1;
    for (int i = 0; i <= end; ++i) {
       sql +=" , "+fields[i]+" text ";
    }
    sql+=" ) ";

    QLOG_INFO()<<"Database::createTaskData "<<sql;

    if(!query.exec(sql)){
        msg = QString("创建数据表失败:%1").arg(query.lastError().text());
        QLOG_INFO()<<"Database::createTaskData Error:"<<query.lastError()<<","<<sql;
    }else{
        success = true;
        query.exec(QString("insert into %1 (tbName) values ('%2');").arg(TABLE_TASK_DATA).arg(code));

    }

    query.clear();
    dbmtx.unlock();
    return success;
}
bool Database::clearTaskData(const QString &code,QString &msg){
    dbmtx.lock();
    QSqlQuery query(*m_sqldb);
    bool success = false;

    query.exec(QString("DELETE FROM '%1' ").arg(code));
    query.exec(QString("UPDATE sqlite_sequence SET seq = 0 WHERE name = '%1'; ").arg(code));
    query.exec(QString("DELETE FROM '%1' where tbName='%2' ").arg(TABLE_TASK_DATA).arg(code));

    if(!query.exec()){
       msg = QString("清空数据表失败:%1").arg(query.lastError().text());
       QLOG_INFO()<<"Database::clearTaskData Error:"<<query.lastError();
    }else{
        success = true;
    }
    query.clear();
    dbmtx.unlock();
    return success;

}
void Database::asyncAddTaskData(const QString &add_task_data_sql){
    m_addTaskDataQueue.enqueue(add_task_data_sql);
}
void Database::asyncUpdateTaskData(MTaskData &taskData){
    m_updateTaskDataQueue.enqueue(taskData);
}
bool Database::getTaskData(const QString &code){
    bool isExist = false;
    dbmtx.lock();
    QStringList tables = m_sqldb->tables();
    for (int i = 0; i < tables.length(); ++i) {
        if(tables[i]==code){
            isExist = true;
            break;
        }
    }
    dbmtx.unlock();

    return isExist;
}
bool Database::delTaskData(const QString &code){
    bool isExist = false;
    dbmtx.lock();
    QStringList tables = m_sqldb->tables();
    for (int i = 0; i < tables.length(); ++i) {
        if(tables[i]==code){
            isExist = true;
            break;
        }
    }
    if(isExist){
        QSqlQuery query(*m_sqldb);
        query.exec(QString("DELETE FROM '%1' ").arg(code));
        query.exec(QString("drop table %1").arg(code));
        query.exec(QString("DELETE FROM '%1' where tbName='%2' ").arg(TABLE_TASK_DATA).arg(code));
        query.clear();
    }
    dbmtx.unlock();


    return isExist;
}

QString Database::getRandomUserAgent(){

//    qsrand(time(NULL));
//    int r = qrand() % userAgents.length();
    MTaskUserAgent ua = userAgents.at(0);

    return ua.userAgent;
}
void Database::setFinger(const QString &finger){
    m_finger = finger;
    QLOG_INFO() << "Database::setFinger finger="<< Database::getInstance()->getFinger();

}
QString Database::getFinger() const{
    return m_finger;
}


void Database::run(){

    while (true) {
        // 插入sql
        int size = m_addTaskDataQueue.size();

        if(size>0){
             size = size >200 ? 200:size;
             dbmtx.lock();
             QSqlQuery query(*m_sqldb);
             m_sqldb->transaction();
             for (int i = 0; i < size; ++i) {
                 QString sql = m_addTaskDataQueue.takeFirst();
                 query.exec(sql);
                 sql.clear();
             }
             m_sqldb->commit();
             query.clear();
             dbmtx.unlock();
        }else{
             sleep(1);
        }

        // 更新runResult
        size = m_updateTaskDataQueue.size();

        if(size>0){
            QHash<QString,MTaskData> indexGroups;
            for (int i = 0; i < size; ++i) {
                MTaskData taskData = m_updateTaskDataQueue.takeFirst();
                indexGroups.insert(taskData.tbName,taskData);
            }

             dbmtx.lock();
             QSqlQuery query(*m_sqldb);
             m_sqldb->transaction();

             QHash<QString,MTaskData>::const_iterator it;
             for (it=indexGroups.constBegin();it!=indexGroups.constEnd();++it) {

                 MTaskData taskData = it.value();
                 QString sql = QString("UPDATE %1 SET num=%2,repeatNum=%3,state=%4,startTime='%5',endTime='%6' WHERE tbName = '%7'; ").
                                                arg(TABLE_TASK_DATA).
                                                arg(taskData.num).
                                                arg(taskData.repeatNum).
                                                arg(taskData.state).
                                                arg(taskData.startTime).
                                                arg(taskData.endTime).
                                                arg(taskData.tbName);
                 query.exec(sql);
                 sql.clear();
             }
             m_sqldb->commit();
             query.clear();
             dbmtx.unlock();
        }

    }

}

QVector<QVector<QString>> Database::select(int queryColumnCount,const QString &sql){

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);
    QVector<QVector<QString>> data;
    if(!query.exec(sql)){
        QLOG_INFO()<<"Database::select Error:"<<query.lastError()<<","<<sql;
    }else{
         while(query.next()){
             QVector<QString> columnData;
             for (int i = 0; i < queryColumnCount; ++i) {
                 columnData.append(query.value(i).toString());
             }
             data.append(columnData);
         }
    }
    query.clear();
    dbmtx.unlock();
    return data;
}
int Database::getTableRowCount(const QString &tbName){

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);
    int count = 0;
    QString sql = QString("SELECT count(1) from %1").arg(tbName);
    if(!query.exec(sql)){
        QLOG_INFO()<<"Database::getTableRowCount Error:"<<query.lastError()<<","<<sql;
    }else{
        if(query.next()){
            count = query.value(0).toInt();
        }
    }
    query.clear();
    dbmtx.unlock();
    return count;
}
QStringList Database::getTableFields(const QString &tbName){

    dbmtx.lock();
    QSqlQuery query(*m_sqldb);

    QStringList names;

    //该查询单行的数据
    //QVariant(qlonglong, 0)
    //QVariant(QString, "id")
    //QVariant(QString, "INTEGER")
    //QVariant(qlonglong, 0)
    //QVariant(QString, "")
    //QVariant(qlonglong, 1)
    QString sql = QString("PRAGMA table_info('%1')").arg(tbName);

    if(!query.exec(sql)){
         QLOG_INFO()<<"Database::getTableFields() Error:"<<query.lastError()<<","<<sql;
    }else{
        while(query.next()){
            names.append(query.value(1).toString());
        }

    }
    query.clear();
    dbmtx.unlock();
//    if(names.count()>0){
//       names.removeAt(0);// "id" 不需要
//    }
//    if(names.count()>0){
//        names.removeAt(0);// "isRepeat" 不需要
//    }
    return names;
}
