#ifndef REPORT_H
#define REPORT_H

#include <QThread>

class Report : public QThread
{
    Q_OBJECT
public:
    explicit Report();
    ~Report();
protected:
    void run() override;
signals:

};

#endif // REPORT_H
