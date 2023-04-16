#ifndef VERSIONDIALOG_H
#define VERSIONDIALOG_H

#include <QDialog>
struct MVersion;

class VersionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VersionDialog(QWidget *parent,MVersion &version);
signals:

};

#endif // VERSIONDIALOG_H
