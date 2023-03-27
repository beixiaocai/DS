#include "mCustomtaskSmartalertXpath.h"
#include "style.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QtCore5Compat>
#include <QDebug>
MCustomTaskSmartAlertXPath::MCustomTaskSmartAlertXPath(QWidget *parent) : QWidget(parent)
{

        boxLayout = new QHBoxLayout(this);
        boxLayout->setContentsMargins(6,0,0,6);
        xpathLabel = new QLabel(this);
        xpathLabel->setCursor(Qt::PointingHandCursor);
        xpathLabel->setStyleSheet(m_stylesheet_QLabel14);

        boxLayout->addWidget(xpathLabel);
        setLayout(boxLayout);


}
void MCustomTaskSmartAlertXPath::setXpath(const QString &xpath){
    if(""==xpath){
        xpathLabel->setText(xpath);
    }else{
        QString x = xpath;
//        x.replace(QRegExp("\\[\\d+\\]"),"");sunqt6
        x.replace(QRegularExpression("\\[\\d+\\]"),"");
        x.replace("/html/body/","");

        QStringList xs = x.split("/");
        QStringList xsReal;

        int end = xs.length();
        if (end >6){
            int start = end - 6;
            for (int i = start; i < end; ++i) {
                xsReal.append(xs[i]);
            }
        }else{
            xsReal = xs;
        }
        xpathLabel->setText(">>"+xsReal.join("/"));
    }

}
