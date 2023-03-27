#ifndef MVERSIONDIALOG_H
#define MVERSIONDIALOG_H

#include <QDialog>
struct MVersion;

class Version : public QDialog
{
    Q_OBJECT
public:

    explicit Version(MVersion &version,QWidget *parent);

signals:

};

#endif // MVERSIONDIALOG_H
