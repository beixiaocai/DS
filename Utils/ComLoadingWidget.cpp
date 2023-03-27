#include "ComLoadingWidget.h"
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>

ComLoadingWidget::ComLoadingWidget(QWidget *parent) :QWidget(parent)
{

//    setAttribute(Qt::WA_StyledBackground,true);
//    setStyleSheet("background-color: rgba(0,0,0,0.2)");

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,100,0,0);

    QLabel *loadingLabel = new QLabel(this);


    boxLayout->addWidget(loadingLabel,0,Qt::AlignCenter);

    QMovie *movie = new QMovie(this);
    movie->setFileName(":/res/images/loading.gif");
    loadingLabel->setFixedSize(30,30);
    loadingLabel->setScaledContents(true);
    loadingLabel->setMovie(movie);
    movie->start();

}

