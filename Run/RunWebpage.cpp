#include "RunWebpage.h"
#include <QsLog.h>

RunWebPage::RunWebPage(QWebEngineProfile *profile, QObject *parent) : QWebEnginePage(profile,parent)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    connect(this, &QWebEnginePage::certificateError, this, &RunWebPage::handleCertificateError);
#endif

}
void RunWebPage::handleCertificateError(const QWebEngineCertificateError &error){
    QLOG_INFO()<<"RunWebPage::handleCertificateError";

//    error.isOverridable();

}
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
bool RunWebPage::certificateError(const QWebEngineCertificateError &error)
{

//    if (error.isOverridable()) {
//        ComMessageBox::error(view(),error.errorDescription());
//    }

//      return error.isOverridable();
    return true;
}
#endif
