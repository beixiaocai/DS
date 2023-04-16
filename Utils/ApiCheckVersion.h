#ifndef APICHECKVERSION_H
#define APICHECKVERSION_H

#include <QObject>

QT_BEGIN_NAMESPACE;
class QNetworkAccessManager;
class QMutex;
class QNetworkReply;
QT_END_NAMESPACE;
struct MVersion;

class ApiCheckVersion : public QObject
{
    Q_OBJECT
public:
    static ApiCheckVersion *getInstance();//检查版本的接口因为启动时就会被初始化，所以设置了单例模式的接口
private:
    explicit ApiCheckVersion(QObject *parent);
    ~ApiCheckVersion();
private:
    QNetworkAccessManager * mNetworkManager;
    static QAtomicPointer<ApiCheckVersion> mInstance;
    static QMutex mInstanceMtx;
public:
    void checkVersion();// 检查版本
signals:
    void notifyCheckVersion(bool state,QString &msg,MVersion &version);
private slots:
    void onRequestFinished(QNetworkReply *reply);

};

#endif // APICHECKVERSION_H
