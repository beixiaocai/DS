#ifndef COMSPLITVWIDGET_H
#define COMSPLITVWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class ComSplitVWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComSplitVWidget(QWidget *parent);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QPoint pressPoint;
signals:
    void moveDistance(int distance);

public slots:
};

#endif // COMSPLITVWIDGET_H
