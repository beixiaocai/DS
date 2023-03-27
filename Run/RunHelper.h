#ifndef RUNHELPER_H
#define RUNHELPER_H

#include <QObject>
#include <QHash>

class MTaskDeserialize;
class MFlowStepParams;
class MFlowStepParamsLoop;
class MFlowStepParamsExtract;
struct MTask;

class RunHelper : public QObject
{
    Q_OBJECT
public:
    explicit RunHelper(MTask *task,QObject *parent);
    ~RunHelper();

    MTask *task;

    QVector<MFlowStepParamsLoop *> loops;      // 所有循环窗体
    QVector<MFlowStepParamsExtract *> extracts;// 所有提取参数窗体
    QStringList fields;           //任务流程中的字段名称数组
    QHash<QString,int> fieldsH;

    MFlowStepParams * getNextStepParams(QString lastStepID);

    QHash<QString,MFlowStepParams*> m_stepsH;
private:
    MTaskDeserialize *m_taskdeser;


signals:

};

#endif // RUNHELPER_H
