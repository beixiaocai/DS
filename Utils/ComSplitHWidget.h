#ifndef COMSPLITHWIDGET_H
#define COMSPLITHWIDGET_H

#include <QWidget>
#include <QMouseEvent>
class ComSplitHWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComSplitHWidget(QWidget *parent);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QPoint pressPoint;
signals:
    void moveDistance(int distance);
public slots:
};

#endif // COMSPLITHWIDGET_H
