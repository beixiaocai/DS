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
public:
    QString tbName;
    int num;
    int repeatNum;
    RUN_STATE state;
    QString startTime;
    QString endTime;

};
struct MTaskRunParams{
public:
    bool isLoop = false;
    bool loopIsContinue = true;//循环执行时，执行状态，默认true 是一直循环，如果手动停止，则修改为false，循环将被终止
    int  loopPeriod = 1;
    int  loopInterval = 0;//循环间隔时长（单位分钟）
    int  loopSingle = 0;//循环单次时长（单位分钟），等于0表示不限制时长
};
struct MTask{
public:
    enum TASKTYPE{
        ADD = 10, //新建
        EDIT,  //编辑
    };
    void toFile(const QString &filename); // 导出任务到指定文件夹
    bool fromFile(const QString &filename);// 导入任务
public:
    int id = 0;//任务添加进数据表以后产生的ID，不参与业务逻辑
    QString code;  //任务编号（导入时或新建时生成）
    int groupId;
    QString groupName;//分组名称，不存储到数据表
    QStringList addressList;
    TASKTYPE type = TASKTYPE::ADD;// 默认类型为新增
    QString name;// 任务名称
    bool isHaveName = false;// 默认不存在名称，交互后产生名称。服务端获取任务时，直接赋予true
    QString program;//任务的程序(已经base64处理)
    int  fieldCount = 0;// 字段数量
    bool isBrowserAllowRunningInsecureContent = true;
    bool isBrowserAutoLoadImages=true;//浏览器是否加载网页图片
    bool isBrowserAutoLoadIconsForPage=true;//浏览器是否加载icon
    bool isBrowserPluginsEnabled=true;//浏览器是否加载插件
    QString defaultUserAgent;  // 存储到数据表的userAgent
    QString addTime;//添加时间
    QString lastUpdateTime;//最近一次修改时间

    // 任务管理时，查询数据辅助参数，不存储 start
    int  tempIndex;
    bool tempIsRun = false;
    MTaskData tempData;
    // 任务管理时，查询数据辅助参数，不存储 end

    MTaskRunParams runParams;

};
struct MVersion{
public:
    float   version = 0.0;// 最新版本号
    QString pubdate;  // 发布时间
    QString updateContent;   // 版本更新内容

    QString url;// 跳转到浏览器直接下载完整新版本的url

};
#endif // MODELS_H
