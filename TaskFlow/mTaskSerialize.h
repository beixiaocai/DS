#ifndef MTASKSERIALIZE_H
#define MTASKSERIALIZE_H

#include <QObject>
class PageCustomTaskFlowControl;
class MFlowButtonWidget;
class MFlowLoopWidget;
class MFlowIfWidget;

class MTaskSerialize : public QObject
{
    Q_OBJECT
public:
    explicit MTaskSerialize(PageCustomTaskFlowControl *parent);
private:
    PageCustomTaskFlowControl *m_fc;

    QString serializeLoopWidget(MFlowLoopWidget *loopWidget);
    QString serializeIfWidget(MFlowIfWidget *ifWidget);
    QString serializeButtonWidget(MFlowButtonWidget *btnWidget);


signals:

};

#endif // MTASKSERIALIZE_H
