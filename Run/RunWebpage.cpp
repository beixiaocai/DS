#include "RunWebpage.h"


RunWebPage::RunWebPage(QWebEngineProfile *profile, QObject *parent) : QWebEnginePage(profile,parent)
{

    connect(this, &QWebEnginePage::certificateError, this, &RunWebPage::handleCertificateError);
}
void RunWebPage::handleCertificateError(const QWebEngineCertificateError &error){
    qDebug()<<"RunWebPage::handleCertificateError";
//    error.isOverridable();

}

//bool RunWebPage::certificateError(const QWebEngineCertificateError &error){

////    return error.isOverridable();
//    return true;
//}
