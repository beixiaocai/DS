#ifndef TASKWEBVIEWBRIDGE_H
#define TASKWEBVIEWBRIDGE_H


#include <QObject>

class TaskWebViewBridge : public QObject{
    Q_OBJECT
public:
    explicit TaskWebViewBridge(QObject *parent);
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

#endif // TASKWEBVIEWBRIDGE_H
