#ifndef RUNWEBENGINEPAGE_H
#define RUNWEBENGINEPAGE_H

#include <QWebEnginePage>

class RunWebEnginePage : public QWebEnginePage
{
    Q_OBJECT
public:
    RunWebEnginePage(QWebEngineProfile *profile, QObject *parent=nullptr);
protected:
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    bool certificateError(const QWebEngineCertificateError &error) override;
#endif

signals:
private slots:
    void handleCertificateError(const QWebEngineCertificateError &error);

};

#endif // RUNWEBENGINEPAGE_H
