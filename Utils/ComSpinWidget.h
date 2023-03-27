#ifndef COMSPINWIDGET_H
#define COMSPINWIDGET_H

#include <QWidget>

class ComSpinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComSpinWidget(const QString &name,int min,int max,int value,int left,int right,QWidget *parent);

    int value = 0;

signals:

};

#endif // COMSPINWIDGET_H
