#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QStringList>


struct MTaskGroup{
public:
    MTaskGroup();
    MTaskGroup(int id,const QString name);
    int id;
    QString name;//任务组名称
};
struct MTaskUserAgent{
public:
    MTaskUserAgent();
    int id;
    QString name;
    QString userAgent;
    int terminal;
};

struct MTaskData{
public:
    enum RUN_STATE{
        FINISH=10,    // 已完成
        STOP       // 异常终止
    };
    MTaskData();
    MTaskData(const QString &tbName,int num,int repeatNum,RUN_STATE state,const QString &startTime,const QString &endTime);

    QString tbName;
    int num;
    int repeatNum;
    RUN_STATE state;
    QString startTime;
    QString endTime;

};

struct MTask{
public:
    MTask();
    ~MTask();
//    MTask(const MTask &task);

    enum TASKTYPE{
        ADD = 10, //本地新建
        EDIT,  //来自云服务
    };
    void toFile(const QString &filename); // 导出任务到指定文件夹
    bool fromFile(const QString &filename);// 导入任务
public:

    int id = 0;//如果等于0，则表示新增任务，不等于0，则表示修改任务
    QString code;  // 任务编号
    // 新建产生
    int groupId;
    QStringList addressList;

    // 交互产生
    TASKTYPE type = TASKTYPE::ADD;// 默认类型为新增
    QString name;// 任务名称
    bool isHaveName = false;// 默认不存在名称，交互后产生名称。服务端获取任务时，直接赋予true
    QString program;//任务的程序(已经base64处理)
    int  fieldCount = 0;// 字段数量
    bool isBrowserAllowRunningInsecureContent = true;
    bool isBrowserAutoLoadImages=true;//浏览器是否加载网页图片
    bool isBrowserAutoLoadIconsForPage=true;//浏览器是否加载icon
    bool isBrowserPluginsEnabled=true;//浏览器是否加载插件
    QString  defaultUserAgent;  // 默认的userAgent

    //////////////// 2023.3.13
    QString groupName;//所属分组名称
    QString addTime;//添加时间
    QString lastUpdateTime;//最近一次修改时间

    // 任务管理行标记
    int showNumRow;
    bool isRun = false;

    MTaskData data;

};



struct MVersion{
public:
    float version = 0.0;// 最新版本号
    QString pubdate;  // 发布时间
    QString updateContent;   // 版本更新内容

    QString url;// 跳转到浏览器直接下载完整新版本的url

};
#endif // MODELS_H
