#ifndef M_FLOW_RIGHT_TIP_WIDGET_H
#define M_FLOW_RIGHT_TIP_WIDGET_H

#include <QWidget>

class MFlowRightTipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MFlowRightTipWidget(const QString name,const QString tip, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // M_FLOW_RIGHT_TIP_WIDGET_H
