#ifndef M_OPTION_BOX_H
#define M_OPTION_BOX_H

#include <QWidget>
QT_BEGIN_NAMESPACE;
class QVBoxLayout;
class QGridLayout;
QT_END_NAMESPACE;

class MOptionBox : public QWidget
{
    Q_OBJECT
public:
    explicit MOptionBox(QWidget *parent = nullptr);


    QWidget *gWidget;
    QGridLayout *gLayout;
    int rowStart = 0;

private:


signals:

public slots:
};

#endif // M_OPTION_BOX_H
