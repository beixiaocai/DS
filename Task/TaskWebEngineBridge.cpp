#include "TaskWebEngineBridge.h"

TaskWebEngineBridge::TaskWebEngineBridge(QObject *parent) : QObject(parent){

}
void TaskWebEngineBridge::clickSignalToQt(const QString &eachp2,const QString &nodeName,const QString &nodeCode,const QString &loopXpath){
    emit this->sendClickSignalToQt(eachp2,nodeName,nodeCode,loopXpath);
}


//CompareXpathResult compareXpath(const QString s1, const QString s2){

//    int s1Length=s1.length();
//    int s2Length=s2.length();

//    // 最小长度
//    int minLength = s1Length;
//    if(s2Length<minLength){
//        minLength = s2Length;
//    }

//    // 最大重复的长度
//    int maxRepeatLength = minLength;
//    for (int i = 0; i < minLength; ++i) {
//        if(s1[i]!=s2[i]){
//            maxRepeatLength = i;
//            break;
//        }
//    }

//    CompareXpathResult res;
//    res.allowXpathObjMerge = false;
//    res.maxRepeatStr = s1.mid(0,maxRepeatLength);//s1和s2重复部分
//    res.s1NotRepeatStr = s1.mid(maxRepeatLength);//s1未重复部分
//    res.s2NotRepeatStr = s2.mid(maxRepeatLength);//s2未重复部分
//    if(s1Length==s2Length && s1Length==maxRepeatLength){
//        //s1与s2完成重复，指向的是同一个元素，两者可以合并

//        res.allowXpathObjMerge = true;

//    }else {
//        QString s_s1Length=QString("%1").arg(s1Length);
//        QString s_s2Length=QString("%1").arg(s2Length);
//        QString s_maxRepeatLength=QString("%1").arg(maxRepeatLength);
//        //s1重复字符占比
//        double s1RepeatRatio =s_maxRepeatLength.toDouble()/s_s1Length.toDouble();
//        //s2重复字符占比
//        double s2RepeatRatio =s_maxRepeatLength.toDouble()/s_s2Length.toDouble();

//        if(s1RepeatRatio>0.8 && s2RepeatRatio>0.8){
//            res.allowXpathObjMerge = true;
//        }

//    }


//    return res;
//}


