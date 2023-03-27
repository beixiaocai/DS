#ifndef MRIGHTSAVE_H
#define MRIGHTSAVE_H

#include <QWidget>

class MRightSave : public QWidget
{
    Q_OBJECT
public:
    explicit MRightSave(QWidget *parent);

signals:
    void notifySave(bool clicked);

};

#endif // MRIGHTSAVE_H
