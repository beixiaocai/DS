#ifndef RUNPIPLINE_H
#define RUNPIPLINE_H

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

class RunPipline : public QThread
{
    Q_OBJECT
public:
    explicit RunPipline(RunHelper *helper,QObject *parent);
    ~RunPipline();

    void sendStartCommand();
    void sendStopCommand();

    bool jobCommandState= false;  // 当前任务命令的状态，默认false
    bool jobFinished = true; // 当前任务完成的状态，默认true

private:
    RunHelper *m_helper; // 任务反序列化对象
    QQueue<MExtractSingleData> m_dataQueue; //数据队列
    QSet<QString> m_fpSet;// 指纹集合

    QVector<QVector<MExtractSingleData*>> m_temps; // 合并临时存储点（合并过程最重要的点）
    QHash<QString,MExtractParamsGroupLocation*> m_locations; // <stepID,MExtractParamsGroupLocation>


    // 记数属性
    int m_num = 0;// 采集总条数
    int m_repeatNum = 0;// 重复总条数
    QDateTime m_startDate;//本次开始执行的日期时间
    int       m_startStamp;//开始的时间戳（秒）

    QString getfp(const QStringList& values);
protected:
    void run() override;
signals:
    void notifyPipline(int num,QStringList names,QStringList values);
    void notifyInfo(int type,QString info); // type（1：通知修改统计信息 10：通知采集成功-结束 11：通知采集失败-结束）

public slots:
    void onPushData(QString stepID,QString res);

};

#endif // RUNPIPLINE_H
