#ifndef M_DOWN_ARROW_H
#define M_DOWN_ARROW_H

#include <QWidget>


class MDownArrow : public QWidget
{
    Q_OBJECT
public:
    explicit MDownArrow(QWidget *parent);
    void setIconNormal();
    void setIconPress();
private:
    int iconType=1; //1：向下的蓝色箭头  2：灰色的圆圈+号

protected:
    void paintEvent(QPaintEvent * event) override;

signals:

public slots:
};

#endif // M_DOWN_ARROW_H
