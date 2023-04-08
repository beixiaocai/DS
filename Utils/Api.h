#ifndef API_H
#define API_H

#include <QObject>

class Api : public QObject
{
    Q_OBJECT
public:
    explicit Api(QObject *parent);
    ~Api();
public:
    static QByteArray download(const QString &url,bool &state,QString &msg); // 下载链接
    static void reportHeart(uint64_t reportCount);// 上传心跳
    static void reportCrash(QString &crashMsg); // 上报崩溃信息
private slots:

};

#endif // API_H
