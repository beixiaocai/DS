#ifndef MROLLWIDGET_H
#define MROLLWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE;
class QComboBox;
class QSpinBox;
QT_END_NAMESPACE;

class MRollWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MRollWidget(QWidget *parent);
    ~MRollWidget();
    QSpinBox *rollCountSpin;//滚动次数
    QSpinBox *rollIntervalSpin;// 每次间隔
    QComboBox *rollTypeCombo;// 滚动方式

};

#endif // MROLLWIDGET_H
