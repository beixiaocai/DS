#include "TaskWebEnginePage.h"
#include "ComMessageBox.h"
#include <QAuthenticator>
#include <QMessageBox>
#include <QStyle>
#include <QWebEngineCertificateError>
#include <QsLog.h>
#include <QTimer>

TaskWebEnginePage::TaskWebEnginePage(QObject *parent,QWebEngineProfile *profile)
    : QWebEnginePage(profile, parent)
{
    QLOG_INFO() <<"TaskWebEnginePage::TaskWebEnginePage()";
    connect(this, &QWebEnginePage::authenticationRequired, this, &TaskWebEnginePage::handleAuthenticationRequired);
    connect(this, &QWebEnginePage::featurePermissionRequested, this, &TaskWebEnginePage::handleFeaturePermissionRequested);
    connect(this, &QWebEnginePage::proxyAuthenticationRequired, this, &TaskWebEnginePage::handleProxyAuthenticationRequired);
    connect(this, &QWebEnginePage::registerProtocolHandlerRequested, this, &TaskWebEnginePage::handleRegisterProtocolHandlerRequested);
#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    connect(this, &QWebEnginePage::selectClientCertificate, this, &TaskWebEnginePage::handleSelectClientCertificate);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    connect(this, &QWebEnginePage::certificateError, this, &TaskWebEnginePage::handleCertificateError);
#endif
}

TaskWebEnginePage::~TaskWebEnginePage(){
    QLOG_INFO() <<"TaskWebEnginePage::~TaskWebEnginePage()";

}

void TaskWebEnginePage::handleCertificateError(const QWebEngineCertificateError &error){
     qDebug()<<"TaskWebEnginePage::handleCertificateError";

}

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
bool TaskWebEnginePage::certificateError(const QWebEngineCertificateError &error)
{
    QLOG_INFO() <<"TaskWebEnginePage::certificateError()";
    if (error.isOverridable()) {
        ComMessageBox::error(view(),error.errorDescription());
    }

    return false;
}
#endif

void TaskWebEnginePage::handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth)
{
    QLOG_INFO() <<"TaskWebEnginePage::handleAuthenticationRequired()";

}

inline QString questionForFeature(QWebEnginePage::Feature feature)
{

    switch (feature) {
    case QWebEnginePage::Geolocation:
        return TaskWebEnginePage::tr("Allow %1 to access your location information?");
    case QWebEnginePage::MediaAudioCapture:
        return TaskWebEnginePage::tr("Allow %1 to access your microphone?");
    case QWebEnginePage::MediaVideoCapture:
        return TaskWebEnginePage::tr("Allow %1 to access your webcam?");
    case QWebEnginePage::MediaAudioVideoCapture:
        return TaskWebEnginePage::tr("Allow %1 to access your microphone and webcam?");
    case QWebEnginePage::MouseLock:
        return TaskWebEnginePage::tr("Allow %1 to lock your mouse cursor?");
    case QWebEnginePage::DesktopVideoCapture:
        return TaskWebEnginePage::tr("Allow %1 to capture video of your desktop?");
    case QWebEnginePage::DesktopAudioVideoCapture:
        return TaskWebEnginePage::tr("Allow %1 to capture audio and video of your desktop?");
    case QWebEnginePage::Notifications:
        return TaskWebEnginePage::tr("Allow %1 to show notification on your desktop?");
    }
    return QString();
}

void TaskWebEnginePage::handleFeaturePermissionRequested(const QUrl &securityOrigin, Feature feature)
{
    QLOG_INFO() <<"TaskWebEnginePage::handleFeaturePermissionRequested()";
    QString title = tr("Permission Request");
    QString question = questionForFeature(feature).arg(securityOrigin.host());

    auto answer = QMessageBox::question(nullptr/*sunqt6 view()->window()*/, title, question);

    if (!question.isEmpty() && answer == QMessageBox::Yes){
        setFeaturePermission(securityOrigin, feature, PermissionGrantedByUser);
    }else{
        setFeaturePermission(securityOrigin, feature, PermissionDeniedByUser);
    }
}

void TaskWebEnginePage::handleProxyAuthenticationRequired(const QUrl &, QAuthenticator *auth, const QString &proxyHost)
{

}

//! [registerProtocolHandlerRequested]
void TaskWebEnginePage::handleRegisterProtocolHandlerRequested(QWebEngineRegisterProtocolHandlerRequest request)
{
    auto answer = QMessageBox::question(
                nullptr,
//        view()->window(),sunqt6
        tr("Permission Request"),
        tr("Allow %1 to open all %2 links?")
        .arg(request.origin().host())
        .arg(request.scheme()));

    if (answer == QMessageBox::Yes){
        request.accept();
    }else{
        request.reject();
    }
}
//! [registerProtocolHandlerRequested]

#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void TaskWebEnginePage::handleSelectClientCertificate(QWebEngineClientCertificateSelection selection)
{
    // Just select one.
    selection.select(selection.certificates().at(0));
}
#endif
