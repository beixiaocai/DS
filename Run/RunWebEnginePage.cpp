#include "RunWebEnginePage.h"
#include <QsLog.h>

RunWebEnginePage::RunWebEnginePage(QWebEngineProfile *profile, QObject *parent) : QWebEnginePage(profile,parent)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    connect(this, &QWebEnginePage::certificateError, this, &RunWebEnginePage::handleCertificateError);
#endif

}
void RunWebEnginePage::handleCertificateError(const QWebEngineCertificateError &error){
    QLOG_INFO()<<"RunWebEnginePage::handleCertificateError";

//    error.isOverridable();

}
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
bool RunWebEnginePage::certificateError(const QWebEngineCertificateError &error)
{

//    if (error.isOverridable()) {
//        ComMessageBox::error(view(),error.errorDescription());
//    }

//      return error.isOverridable();
    return true;
}
#endif
