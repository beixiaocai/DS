#ifndef TASKWEBENGINEBRIDGE_H
#define TASKWEBENGINEBRIDGE_H


#include <QObject>

class TaskWebEngineBridge : public QObject{
    Q_OBJECT
public:
    explicit TaskWebEngineBridge(QObject *parent);
signals:
    void sendClickSignalToQt(const QString &eachp2,const QString &nodeName,const QString &nodeCode,const QString &loopXpath);
public slots:
    void clickSignalToQt(const QString &eachp2,const QString &nodeName,const QString &nodeCode,const QString &loopXpath);

};



//struct CompareXpathResult{
//    QString maxRepeatStr;
//    QString s1NotRepeatStr;
//    QString s2NotRepeatStr;

//    bool allowXpathObjMerge;
//};

#endif // TASKWEBENGINEBRIDGE_H
