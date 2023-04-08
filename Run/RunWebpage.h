#ifndef RUNWEBPAGE_H
#define RUNWEBPAGE_H

#include <QWebEnginePage>

class RunWebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    RunWebPage(QWebEngineProfile *profile, QObject *parent=nullptr);
protected:
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    bool certificateError(const QWebEngineCertificateError &error) override;
#endif

signals:
private slots:
    void handleCertificateError(const QWebEngineCertificateError &error);

};

#endif // RUNWEBPAGE_H
