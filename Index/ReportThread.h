#ifndef REPORTTHREAD_H
#define REPORTTHREAD_H
#include <QThread>
class ReportThread : public QThread
{
    Q_OBJECT
public:
    explicit ReportThread(QObject *parent);
    ~ReportThread();
protected:
    void run() override;
signals:

private slots:

};

#endif // REPORTTHREAD_H
