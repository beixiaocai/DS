#ifndef TASKWEBENGINEPAGE_H
#define TASKWEBENGINEPAGE_H

#include <QWebEnginePage>
#include <QWebEngineRegisterProtocolHandlerRequest>

class TaskWebEnginePage : public QWebEnginePage
{
    Q_OBJECT

public:
    TaskWebEnginePage(QObject *parent,QWebEngineProfile *profile);
    ~TaskWebEnginePage();

protected:
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    bool certificateError(const QWebEngineCertificateError &error) override;
#endif

private slots:
    void handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth);
    void handleFeaturePermissionRequested(const QUrl &securityOrigin, Feature feature);
    void handleProxyAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth, const QString &proxyHost);
    void handleRegisterProtocolHandlerRequested(QWebEngineRegisterProtocolHandlerRequest request);
#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    void handleSelectClientCertificate(QWebEngineClientCertificateSelection clientCertSelection);
#endif
    void handleCertificateError(const QWebEngineCertificateError &error);

};

#endif // TASKWEBENGINEPAGE_H
