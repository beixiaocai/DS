#ifndef TASKWEBENGINEPROFILE_H
#define TASKWEBENGINEPROFILE_H

#include <QObject>

QT_BEGIN_NAMESPACE;
class QMutex;
class QWebEngineProfile;
QT_END_NAMESPACE;


class TaskWebEngineProfile : public QObject
{
    Q_OBJECT
public:
    static TaskWebEngineProfile *getInstance();
private:
    explicit TaskWebEngineProfile(QObject *parent);
    ~TaskWebEngineProfile();
public:
    QWebEngineProfile* profile();
    QWebEngineProfile* copyProfile();
    QString cachePath();
    void clearCache();
private:
    static QAtomicPointer<TaskWebEngineProfile> mInstance;
    static QMutex mInstanceMtx;

    QWebEngineProfile *mProfile;
    QString mCacheName;
    QString mCachePath;



};

#endif // TASKWEBENGINEPROFILE_H
