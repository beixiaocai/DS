#include "models.h"
#include <QFile>

MTaskGroup::MTaskGroup(){}
MTaskGroup::MTaskGroup(int id,const QString name){
    this->id = id;
    this->name = name;
}
MTaskUserAgent::MTaskUserAgent(){

}
MTaskData::MTaskData(){}
MTaskData::MTaskData(const QString &tbName,int num,int repeatNum,RUN_STATE state,const QString &startTime,const QString &endTime){
    this->tbName = tbName;
    this->num = num;
    this->repeatNum = repeatNum;
    this->state = state;
    this->startTime = startTime;
    this->endTime = endTime;
}

MTask::MTask(){}
MTask::~MTask(){}
bool MTask::fromFile(const QString &filename){

    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QString content = f.readAll();
    f.close();
    content = QByteArray::fromBase64(content.toUtf8()).data();
    QStringList attrs = content.split(",");

    if(attrs.length()==8){

        this->addressList = QString(QByteArray::fromBase64(attrs[0].toUtf8()).data()).
                split(",");
        this->name = QByteArray::fromBase64(attrs[1].toUtf8()).data();
        this->program = attrs[2];
        this->fieldCount = QString(QByteArray::fromBase64(attrs[3].toUtf8().data())).toInt();

        this->isBrowserAllowRunningInsecureContent = QString(QByteArray::fromBase64(attrs[4].toUtf8().data())).toInt();
        this->isBrowserAutoLoadImages = QString(QByteArray::fromBase64(attrs[5].toUtf8().data())).toInt();
        this->isBrowserAutoLoadIconsForPage = QString(QByteArray::fromBase64(attrs[6].toUtf8().data())).toInt();
        this->isBrowserPluginsEnabled= QString(QByteArray::fromBase64(attrs[7].toUtf8().data())).toInt();

        return true;
    }else{
        return false;
    }
}
void MTask::toFile(const QString &filename){

   QStringList attrs;

   attrs.append(addressList.join(",").toUtf8().toBase64());
   attrs.append(name.toUtf8().toBase64());
   attrs.append(program);
   attrs.append(QString::number(fieldCount).toUtf8().toBase64());

   attrs.append(QString::number(isBrowserAllowRunningInsecureContent).toUtf8().toBase64());
   attrs.append(QString::number(isBrowserAutoLoadImages).toUtf8().toBase64());
   attrs.append(QString::number(isBrowserAutoLoadIconsForPage).toUtf8().toBase64());
   attrs.append(QString::number(isBrowserPluginsEnabled).toUtf8().toBase64());


   QString content = attrs.join(",").toUtf8().toBase64();

   QFile file(filename);
   file.open(QIODevice::WriteOnly | QIODevice::Text);
   file.write(content.toUtf8());
   file.close();

}



