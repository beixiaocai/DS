#ifndef RUNTHREADPIPLINE_H
#define RUNTHREADPIPLINE_H

#include <QThread>
#include <QQueue>
#include <QSet>
#include <QDateTime>

class RunHelper;

struct MExtractSingleItem{
    MExtractSingleItem();
    MExtractSingleItem(const QString &name,const QString &value);
    public:
        QString name;
        QString value;
};

struct MExtractSingleData{
    MExtractSingleData(const QString &stepID);
    public:
        QString stepID;
        QVector<MExtractSingleItem> items;


};
struct MExtractParamsGroupLocation{
    MExtractParamsGroupLocation();
    MExtractParamsGroupLocation(int group,int index);
    public:
        int group;//外层序号
        int index;//内层序号
};

class RunThreadPipline : public QThread
{
    Q_OBJECT
public:
    explicit RunThreadPipline(QObject *parent);
    ~RunThreadPipline();

    void sendStartCommand(int jobPeriod);
    void sendStopCommand();

    bool jobCommandState = false;  // 当前任务命令的状态，默认false
    bool jobFinished = true; // 当前任务完成的状态，默认true

private:
    RunHelper *mRunHelper; // 任务反序列化对象
    QQueue<MExtractSingleData> mDataQueue; //数据队列
    QSet<QString> mDataFingerSet;// 指纹集合

    QVector<QVector<MExtractSingleData*>> m_temps; // 合并临时存储点（合并过程最重要的点）
    QHash<QString,MExtractParamsGroupLocation*> m_locations; // <stepID,MExtractParamsGroupLocation>


    // 属性
    int mJobPeriod = 1;//采集周期默认1（采集周期等于1的时候，才会重新创建数据表）
    int mJobCount = 0;// 当前周期采集总条数
    int mJobRepeatCount = 0;// 当前周期总重复条数
    int mTotalJobCount = 0;//所有周期采集总条数
    int mTotalJobRepeatCount = 0;//所有周期总重复条数
    QDateTime mJobStartDate;//本次开始执行的日期时间
    int       mJobStartStamp;//开始的时间戳（秒）

    QString calcuDataFinger(const QStringList& values);
protected:
    void run() override;
signals:
    void notifyUpdateData(int num,QStringList names,QStringList values);
    void notifyAlert(int type,QString info); // type（1：通知修改统计信息 10：通知采集成功-结束 11：通知采集失败-结束）
    void notifyJobFinished();

public slots:
    void onPushData(QString stepID,QString res);

};

#endif // RUNTHREADPIPLINE_H
