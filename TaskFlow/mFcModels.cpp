#include "mFcModels.h"
#include <QDebug>




MControl::MControl(QString menuType,
                             QString menuTip,
                             QString menuIcon,
                             QString menuFlowWidgetName): menuType(menuType),menuTip(menuTip),menuIcon(menuIcon),menuFlowWidgetName(menuFlowWidgetName){

}
MControl:: ~MControl(){}
MFlowStepParams::MFlowStepParams(){}
MFlowStepParams::~MFlowStepParams(){}
MFlowStepParamsOpenWeb::MFlowStepParamsOpenWeb(){}
MFlowStepParamsOpenWeb::MFlowStepParamsOpenWeb(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 18){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));
        overtime = QString(QByteArray::fromBase64(params[4].toUtf8().data())).toInt();
        url = QString(QByteArray::fromBase64(params[5].toUtf8().data()));
        waitSleep = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[7].toUtf8().data()));

        isAutoTry = QString(QByteArray::fromBase64(params[8].toUtf8().data())).toInt();
        isCustomCookie = QString(QByteArray::fromBase64(params[9].toUtf8().data())).toInt();
        cookie = QString(QByteArray::fromBase64(params[10].toUtf8().data()));
        isLoop = QString(QByteArray::fromBase64(params[11].toUtf8().data())).toInt();
        isPreventWindow = QString(QByteArray::fromBase64(params[12].toUtf8().data())).toInt();
        isClearCacheBeforeOpenWeb = QString(QByteArray::fromBase64(params[13].toUtf8().data())).toInt();
        isRoll = QString(QByteArray::fromBase64(params[14].toUtf8().data())).toInt();
        rollCount = QString(QByteArray::fromBase64(params[15].toUtf8().data())).toInt();
        rollInterval = QString(QByteArray::fromBase64(params[16].toUtf8().data())).toInt();
        rollTypeIndex = QString(QByteArray::fromBase64(params[17].toUtf8().data())).toInt();

    }else {
        qDebug()<<"MFlowStepParamsOpenWeb(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsOpenWeb::getProgram(){
    QStringList params;

    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<QString::number(overtime).toUtf8().toBase64()
          <<url.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isAutoTry).toUtf8().toBase64()
          <<QString::number(isCustomCookie).toUtf8().toBase64()
          <<cookie.toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64()
          <<QString::number(isPreventWindow).toUtf8().toBase64()
          <<QString::number(isClearCacheBeforeOpenWeb).toUtf8().toBase64()
          <<QString::number(isRoll).toUtf8().toBase64()
          <<QString::number(rollCount).toUtf8().toBase64()
          <<QString::number(rollInterval).toUtf8().toBase64()
          <<QString::number(rollTypeIndex).toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}
MFlowStepParamsClickEle::MFlowStepParamsClickEle(){}
MFlowStepParamsClickEle::MFlowStepParamsClickEle(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 21){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));
        eleText = QString(QByteArray::fromBase64(params[4].toUtf8().data()));
        eleXpath = QString(QByteArray::fromBase64(params[5].toUtf8().data()));
        waitSleep = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[7].toUtf8().data()));
        isAutoTry = QString(QByteArray::fromBase64(params[8].toUtf8().data())).toInt();
        isLoop = QString(QByteArray::fromBase64(params[9].toUtf8().data())).toInt();

        isNewTab = QString(QByteArray::fromBase64(params[10].toUtf8().data())).toInt();
        isAjax = QString(QByteArray::fromBase64(params[11].toUtf8().data())).toInt();
        ajaxOvertime = QString(QByteArray::fromBase64(params[12].toUtf8().data())).toInt();
        isPageSpeed = QString(QByteArray::fromBase64(params[13].toUtf8().data())).toInt();
        isAnchor = QString(QByteArray::fromBase64(params[14].toUtf8().data())).toInt();
        anchorId = QString(QByteArray::fromBase64(params[15].toUtf8().data()));

        isRoll = QString(QByteArray::fromBase64(params[16].toUtf8().data())).toInt();
        rollCount = QString(QByteArray::fromBase64(params[17].toUtf8().data())).toInt();
        rollInterval = QString(QByteArray::fromBase64(params[18].toUtf8().data())).toInt();
        rollTypeIndex = QString(QByteArray::fromBase64(params[19].toUtf8().data())).toInt();

        isTurnPage = QString(QByteArray::fromBase64(params[20].toUtf8().data())).toInt();

    }else {
        qDebug()<<"MFlowStepParamsClickEle(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsClickEle::getProgram(){
    QStringList params;

    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<eleText.toUtf8().toBase64()
          <<eleXpath.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isAutoTry).toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64()
          <<QString::number(isNewTab).toUtf8().toBase64()
          <<QString::number(isAjax).toUtf8().toBase64()
          <<QString::number(ajaxOvertime).toUtf8().toBase64()
          <<QString::number(isPageSpeed).toUtf8().toBase64()
          <<QString::number(isAnchor).toUtf8().toBase64()
          <<anchorId.toUtf8().toBase64()
          <<QString::number(isRoll).toUtf8().toBase64()
          <<QString::number(rollCount).toUtf8().toBase64()
          <<QString::number(rollInterval).toUtf8().toBase64()
          <<QString::number(rollTypeIndex).toUtf8().toBase64()
          <<QString::number(isTurnPage).toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MParamsField::MParamsField(){}
MParamsField::MParamsField(const QString &program){

    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 7){
        fieldXpath = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        fieldName = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        fieldTag = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        fieldVal = QString(QByteArray::fromBase64(params[3].toUtf8().data()));
        fieldActionName = QString(QByteArray::fromBase64(params[4].toUtf8().data()));
        fieldActionMethod = QString(QByteArray::fromBase64(params[5].toUtf8().data()));
        currentRow = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
    }else {
        qDebug()<<"MParamsField(const QString &program) 反序列化失败";
    }
}
MParamsField::~MParamsField(){
    qDebug()<<"MParamsField::~MParamsField()";
}
const QString MParamsField::getProgram(){

    QStringList params;
    params<<fieldXpath.toUtf8().toBase64()
          <<fieldName.toUtf8().toBase64()
          <<fieldTag.toUtf8().toBase64()
          <<fieldVal.toUtf8().toBase64()
          <<fieldActionName.toUtf8().toBase64()
          <<fieldActionMethod.toUtf8().toBase64()
          <<QString::number(currentRow).toUtf8().toBase64();
    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}


MFlowStepParamsExtract::MFlowStepParamsExtract(){
}
MFlowStepParamsExtract::~MFlowStepParamsExtract(){
    for (int i = 0; i < pfs.count(); ++i) {
        delete pfs[i];
    }
}
MFlowStepParamsExtract::MFlowStepParamsExtract(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 8){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));
        waitSleep = QString(QByteArray::fromBase64(params[4].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[5].toUtf8().data()));
        isLoop = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
        QString fieldListStr = QString(QByteArray::fromBase64(params[7].toUtf8().data()));

        if(fieldListStr!=""){
            // 字段列表的属性不为空
            QStringList fieldList = fieldListStr.split(SEPARATOR_2);
            for (int i = 0; i < fieldList.count(); ++i) {
                QString fieldProgram = QString(QByteArray::fromBase64(fieldList[i].toUtf8().data()));
                pfs.append(new MParamsField(fieldProgram));
            }
        }

    }else {
        qDebug()<<"MFlowStepParamsExtract(const QString &program) 反序列化失败";
    }

}
const QString MFlowStepParamsExtract::getProgram(){
    QStringList params;

    //提取字段数组
    QStringList pfList;
    for (int i = 0; i < pfs.count(); ++i) {
        pfList.append(pfs.at(i)->getProgram());
    }
    QString pfListStr = pfList.join(SEPARATOR_2);

    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64()
          <<pfListStr.toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsInput::MFlowStepParamsInput(){}
MFlowStepParamsInput::MFlowStepParamsInput(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 10){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));

        eleText = QString(QByteArray::fromBase64(params[4].toUtf8().data()));
        eleXpath = QString(QByteArray::fromBase64(params[5].toUtf8().data()));

        waitSleep = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[7].toUtf8().data()));
        isLoop = QString(QByteArray::fromBase64(params[8].toUtf8().data())).toInt();
        inputText = QString(QByteArray::fromBase64(params[9].toUtf8().data()));

    }else {
        qDebug()<<"MFlowStepParamsInput(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsInput::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<eleText.toUtf8().toBase64()
          <<eleXpath.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64()
          <<inputText.toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}
MFlowStepParamsCaptcha::MFlowStepParamsCaptcha(){}
const QString MFlowStepParamsCaptcha::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64();
    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsSwitch::MFlowStepParamsSwitch(){}
MFlowStepParamsSwitch::MFlowStepParamsSwitch(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 9){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));

        eleText = QString(QByteArray::fromBase64(params[4].toUtf8().data()));
        eleXpath = QString(QByteArray::fromBase64(params[5].toUtf8().data()));

        waitSleep = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[7].toUtf8().data()));
        isLoop = QString(QByteArray::fromBase64(params[8].toUtf8().data())).toInt();
    }else {
        qDebug()<<"MFlowStepParamsSwitch(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsSwitch::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<eleText.toUtf8().toBase64()
          <<eleXpath.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64();


    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsIf::MFlowStepParamsIf(){}
MFlowStepParamsIf::MFlowStepParamsIf(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 7){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));
        waitSleep = QString(QByteArray::fromBase64(params[4].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[5].toUtf8().data()));
        isLoop = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
    }else {
        qDebug()<<"MFlowStepParamsMouse(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsIf::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsLoop::MFlowStepParamsLoop(){}
MFlowStepParamsLoop::MFlowStepParamsLoop(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 11){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));

        waitSleep = QString(QByteArray::fromBase64(params[4].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[5].toUtf8().data()));

        loopTypeName = QString(QByteArray::fromBase64(params[6].toUtf8().data()));
        loopTypeValue = QString(QByteArray::fromBase64(params[7].toUtf8().data()));
        loopTextList = QString(QByteArray::fromBase64(params[8].toUtf8().data())).split(SEPARATOR_2);
        isEndLoopWithTimes = QString(QByteArray::fromBase64(params[9].toUtf8().data())).toInt();
        endLoopTimes = QString(QByteArray::fromBase64(params[10].toUtf8().data())).toInt();

    }else {
        qDebug()<<"MFlowStepParamsLoop(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsLoop::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<loopTypeName.toUtf8().toBase64()
          <<loopTypeValue.toUtf8().toBase64()
          <<loopTextList.join(SEPARATOR_2).toUtf8().toBase64()
         <<QString::number(isEndLoopWithTimes).toUtf8().toBase64()
         <<QString::number(endLoopTimes).toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsMouse::MFlowStepParamsMouse(){}
MFlowStepParamsMouse::MFlowStepParamsMouse(const QString &program){
    QStringList params = program.split(SEPARATOR_1);
    if(params.length() == 9){
        name = QString(QByteArray::fromBase64(params[0].toUtf8().data()));
        stepID = QString(QByteArray::fromBase64(params[1].toUtf8().data()));
        stepURL = QString(QByteArray::fromBase64(params[2].toUtf8().data()));
        menuType = QString(QByteArray::fromBase64(params[3].toUtf8().data()));

        eleText = QString(QByteArray::fromBase64(params[4].toUtf8().data()));
        eleXpath = QString(QByteArray::fromBase64(params[5].toUtf8().data()));

        waitSleep = QString(QByteArray::fromBase64(params[6].toUtf8().data())).toInt();
        waitEmergintEle = QString(QByteArray::fromBase64(params[7].toUtf8().data()));
        isLoop = QString(QByteArray::fromBase64(params[8].toUtf8().data())).toInt();
    }else {
        qDebug()<<"MFlowStepParamsMouse(const QString &program) 反序列化失败";
    }
}
const QString MFlowStepParamsMouse::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64()
          <<eleText.toUtf8().toBase64()
          <<eleXpath.toUtf8().toBase64()
          <<QString::number(waitSleep).toUtf8().toBase64()
          <<waitEmergintEle.toUtf8().toBase64()
          <<QString::number(isLoop).toUtf8().toBase64();

    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsEndLoop::MFlowStepParamsEndLoop(){}
const QString MFlowStepParamsEndLoop::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64();
    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}

MFlowStepParamsEnd::MFlowStepParamsEnd(){}
const QString MFlowStepParamsEnd::getProgram(){
    QStringList params;
    params<<name.toUtf8().toBase64()
          <<stepURL.toUtf8().toBase64()
          <<stepID.toUtf8().toBase64()
          <<menuType.toUtf8().toBase64();
    return QString(params.join(SEPARATOR_1).toUtf8().toBase64().data());
}


QString getRandomString(int len=12)
{
    const char chrs[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int chrs_size = sizeof(chrs);

    char* ch = new char[len + 1];
    memset(ch, 0, len + 1);
    int randomx = 0;
    for (int i = 0; i < len; ++i){
        randomx= rand() % (chrs_size - 1);
        ch[i] = chrs[randomx];
    }

    QString ret(ch);
    delete[] ch;
    return ret;
}

//! 步骤窗体的参数初始化

MFlowStepData::MFlowStepData(MControl *control,MFlowStepParams *params){
    this->control = control;

    if(params==nullptr){

        if(MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn == control->menuType){
            stepparams= new MFlowStepParamsOpenWeb();
        }else if(MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn == control->menuType){
            stepparams = new MFlowStepParamsClickEle();
        }else if(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn == control->menuType){
            stepparams= new MFlowStepParamsExtract();
        }else if(MCONSTANT_FLOW_MENU_TYPE_InputBtn == control->menuType){
            stepparams= new MFlowStepParamsInput();
        }else if (MCONSTANT_FLOW_MENU_TYPE_CaptchaBtn == control->menuType) {
            stepparams = new MFlowStepParamsCaptcha();
        }else if (MCONSTANT_FLOW_MENU_TYPE_SwitchBtn == control->menuType) {
            stepparams = new MFlowStepParamsSwitch();
        }else if (MCONSTANT_FLOW_MENU_TYPE_JudgeBtn == control->menuType) {
            stepparams = new MFlowStepParamsIf();
        }else if (MCONSTANT_FLOW_MENU_TYPE_LoopBtn == control->menuType) {
            MFlowStepParamsLoop *paramsloop = new MFlowStepParamsLoop();
            paramsloop->loopTypeName = MCONSTANT_RADIO_SITEADDRESS;
            stepparams = paramsloop;
        }else if (MCONSTANT_FLOW_MENU_TYPE_MouseBtn == control->menuType) {
            stepparams = new MFlowStepParamsMouse();
        }else if (MCONSTANT_FLOW_MENU_TYPE_EndLoopBtn == control->menuType) {
            stepparams = new MFlowStepParamsEndLoop();
        }else if (MCONSTANT_FLOW_MENU_TYPE_EndBtn == control->menuType) {
            stepparams = new MFlowStepParamsEnd();
        }else {
            stepparams = new MFlowStepParams();
        }

        stepparams->menuType = control->menuType;
        stepparams->stepID = getRandomString();
        stepparams->name = control->menuTip;

    }else {
        stepparams = params;
    }
}
MFlowStepData::~MFlowStepData(){
    delete stepparams;
}
