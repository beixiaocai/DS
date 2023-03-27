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
private:
    explicit ApiCheckVersion(QObject *parent=nullptr);
    ~ApiCheckVersion();
private:
    QNetworkAccessManager * mNetworkManager;
    static QAtomicPointer<ApiCheckVersion> mInstance;
    static QMutex mInstanceMtx;
public:
    static ApiCheckVersion *getInstance();
    void checkVersion();// 检查版本
signals:
    void notifyCheckVersion(bool state,QString &msg,MVersion &version);
private slots:
    void replyFinished(QNetworkReply *reply);

};

#endif // APICHECKVERSION_H
