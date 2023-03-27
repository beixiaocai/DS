#ifndef RUNWEBPAGE_H
#define RUNWEBPAGE_H

#include <QWebEnginePage>

class RunWebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    RunWebPage(QWebEngineProfile *profile, QObject *parent=nullptr);
//    virtual bool certificateError(const QWebEngineCertificateError &error);
private:
    void handleCertificateError(const QWebEngineCertificateError &error);

signals:
private slots:

};

#endif // RUNWEBPAGE_H
