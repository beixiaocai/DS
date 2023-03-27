#ifndef MTASKDESERIALIZE_H
#define MTASKDESERIALIZE_H

#include <QHash>
#include "mFcModels.h"

class MTaskDeserialize : public QObject
{
    Q_OBJECT
public:
    explicit MTaskDeserialize(const QString &program,QObject *parent);
    QVector<MFlowStepParams*> steps; // 序列化产生的步骤参数堆内存，由使用者自行清理内存，本class不负责统一清理
private:
    MFlowStepParams::LocationType getStepLocationType(int start,int end,int index);
    void appendParams(int deep,const QString &program,MFlowStepParams *parentParams = nullptr);
    void appendLoopParams(int deep,MFlowStepParams::LocationType stepLocation,const QString &programBase64,MFlowStepParams *parentParams = nullptr);
signals:

};

#endif // MTASKDESERIALIZE_H
