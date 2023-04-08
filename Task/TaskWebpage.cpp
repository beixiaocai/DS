#include "TaskWebPage.h"
#include <QAuthenticator>
#include <QMessageBox>
#include <QStyle>
#include <QWebEngineCertificateError>
#include <QsLog.h>

TaskWebPage::TaskWebPage(QObject *parent,QWebEngineProfile *profile)
    : QWebEnginePage(profile, parent)
{
    QLOG_INFO() <<"TaskWebPage::TaskWebPage()";
    connect(this, &QWebEnginePage::authenticationRequired, this, &TaskWebPage::handleAuthenticationRequired);
    connect(this, &QWebEnginePage::featurePermissionRequested, this, &TaskWebPage::handleFeaturePermissionRequested);
    connect(this, &QWebEnginePage::proxyAuthenticationRequired, this, &TaskWebPage::handleProxyAuthenticationRequired);
    connect(this, &QWebEnginePage::registerProtocolHandlerRequested, this, &TaskWebPage::handleRegisterProtocolHandlerRequested);
#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    connect(this, &QWebEnginePage::selectClientCertificate, this, &TaskWebPage::handleSelectClientCertificate);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    connect(this, &QWebEnginePage::certificateError, this, &TaskWebPage::handleCertificateError);
#endif
}

TaskWebPage::~TaskWebPage(){
    QLOG_INFO() <<"TaskWebPage::~TaskWebPage()";

}

void TaskWebPage::handleCertificateError(const QWebEngineCertificateError &error){
     qDebug()<<"TaskWebPage::handleCertificateError";

}

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
bool TaskWebPage::certificateError(const QWebEngineCertificateError &error)
{

    if (error.isOverridable()) {
        ComMessageBox::error(view(),error.errorDescription());
    }

    return false;
}
#endif

void TaskWebPage::handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth)
{


}

inline QString questionForFeature(QWebEnginePage::Feature feature)
{
    switch (feature) {
    case QWebEnginePage::Geolocation:
        return TaskWebPage::tr("Allow %1 to access your location information?");
    case QWebEnginePage::MediaAudioCapture:
        return TaskWebPage::tr("Allow %1 to access your microphone?");
    case QWebEnginePage::MediaVideoCapture:
        return TaskWebPage::tr("Allow %1 to access your webcam?");
    case QWebEnginePage::MediaAudioVideoCapture:
        return TaskWebPage::tr("Allow %1 to access your microphone and webcam?");
    case QWebEnginePage::MouseLock:
        return TaskWebPage::tr("Allow %1 to lock your mouse cursor?");
    case QWebEnginePage::DesktopVideoCapture:
        return TaskWebPage::tr("Allow %1 to capture video of your desktop?");
    case QWebEnginePage::DesktopAudioVideoCapture:
        return TaskWebPage::tr("Allow %1 to capture audio and video of your desktop?");
    case QWebEnginePage::Notifications:
        return TaskWebPage::tr("Allow %1 to show notification on your desktop?");
    }
    return QString();
}

void TaskWebPage::handleFeaturePermissionRequested(const QUrl &securityOrigin, Feature feature)
{
    QString title = tr("Permission Request");
    QString question = questionForFeature(feature).arg(securityOrigin.host());
    if (!question.isEmpty() && QMessageBox::question(nullptr, title, question) == QMessageBox::Yes)
    //if (!question.isEmpty() && QMessageBox::question( sunqt6 view()->window(), title, question) == QMessageBox::Yes)

        setFeaturePermission(securityOrigin, feature, PermissionGrantedByUser);
    else
        setFeaturePermission(securityOrigin, feature, PermissionDeniedByUser);
}

void TaskWebPage::handleProxyAuthenticationRequired(const QUrl &, QAuthenticator *auth, const QString &proxyHost)
{

}

//! [registerProtocolHandlerRequested]
void TaskWebPage::handleRegisterProtocolHandlerRequested(QWebEngineRegisterProtocolHandlerRequest request)
{
    auto answer = QMessageBox::question(
                nullptr,
//        view()->window(),sunqt6
        tr("Permission Request"),
        tr("Allow %1 to open all %2 links?")
        .arg(request.origin().host())
        .arg(request.scheme()));
    if (answer == QMessageBox::Yes)
        request.accept();
    else
        request.reject();
}
//! [registerProtocolHandlerRequested]

#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void TaskWebPage::handleSelectClientCertificate(QWebEngineClientCertificateSelection selection)
{
    // Just select one.
    selection.select(selection.certificates().at(0));
}
#endif
