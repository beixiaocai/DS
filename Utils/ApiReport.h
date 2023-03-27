#ifndef APIREPORT_H
#define APIREPORT_H

#include <QObject>

QT_BEGIN_NAMESPACE;
class QNetworkAccessManager;
class QMutex;
class QNetworkReply;
QT_END_NAMESPACE;

class ApiReport : public QObject
{
    Q_OBJECT
private:
    explicit ApiReport(QObject *parent=nullptr);
    ~ApiReport();
private:
    QNetworkAccessManager * mNetworkManager;
    static QAtomicPointer<ApiReport> mInstance;
    static QMutex mInstanceMtx;
public:
    static ApiReport *getInstance();

    void reportCrash(QString &crashMsg); // 上报崩溃信息
    void reportHeart(uint32_t reportCount);// 上传心跳

private slots:
    void replyFinished(QNetworkReply *reply);

};

#endif // APIREPORT_H
