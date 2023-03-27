#include "ComLoadingLabel.h"
#include <QMovie>

ComLoadingLabel::ComLoadingLabel(QWidget *parent) : QLabel(parent)
{
    QMovie *movie = new QMovie(this);
    movie->setFileName(":/res/images/loading.gif");
    setFixedSize(20,20);
    setScaledContents(true);
    setMovie(movie);
    movie->start();
    hide();
}
