#include "TaskSmartAlertDialog.h"
#include "mCustomtaskSmartalertClick.h"
#include "mCustomtaskSmartalertSelectall.h"
#include "mCustomtaskSmartalertSuccess.h"
#include "mCustomtaskSmartalertXpath.h"
#include "TaskWebEngineView.h"
#include "TaskWebEngineBridge.h"
#include "Utils/models.h"
#include "style.h"
#include "TaskFlow/pageCustomtaskFlowControl.h"
#include "TaskFlow/mFcModels.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QTimer>
#include <QsLog.h>

TaskSmartAlertDialog::MEachP2::MEachP2(const QString eachp2){
    QStringList p2Attrs = eachp2.split(SEPARATOR_WEBDATA_ATTR);
    this->ele_fullXpath = p2Attrs[0].trimmed();
    this->ele_xpath = p2Attrs[1].trimmed().replace("'","\"");
    this->ele_text = p2Attrs[2].trimmed();
    this->ele_custom = p2Attrs[3].trimmed();
    this->ele_innerHtml = p2Attrs[4].trimmed();
    this->ele_outerHtml = p2Attrs[5].trimmed();
    this->sameTypeCount = p2Attrs[6].trimmed().toInt();
};
TaskSmartAlertDialog::MEachP2::MEachP2(){};

TaskSmartAlertDialog::TaskSmartAlertDialog(QWidget *parent,TaskWebEngineView *webView,PageCustomTaskFlowControl *fc) :QDialog(parent),
    m_webView(webView),m_fc(fc)
{

    setWindowTitle(tr("智能提示"));
    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint);

    this->setFixedWidth(300);
    this->setMaximumHeight(600);
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(QString(".TaskSmartAlertDialog{background-color:%1;}").arg(m_rgb_basic));

    connect(m_webView->bridge,&TaskWebEngineBridge::sendClickSignalToQt,this,&TaskSmartAlertDialog::onClickSignalToQt);
    connect(m_fc,&PageCustomTaskFlowControl::notificationToSmartAlertWidget,this,&TaskSmartAlertDialog::onNotificationFromFlowControl);

    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);
    // 默认布局
    tipLabel = new QLabel(this);
    tipLabel->adjustSize();
    tipLabel->setWordWrap(true);
    tipLabel->setAlignment(Qt::AlignTop);
    tipLabel->setMinimumHeight(32);
    tipLabel->setContentsMargins(10,10,10,0);
    boxLayout->addWidget(tipLabel);
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setContentsMargins(0,0,0,0);
    boxLayout->addWidget(stackedWidget);
    xpathWidget = new MCustomTaskSmartAlertXPath(this);
    boxLayout->addWidget(xpathWidget);


    tipLabel->setText(tr("请选择页面元素。"));
    stackedWidget->hide();

    initSuccessWidget();
    initAWidget();
    initAWidgetSelectAll();
    initImgWidget();
    initImgWidgetSelectAll();
    initInputWidget();
    initInputInputWidget();
    initCommonWidget();
    initCommonWidgetSelectAll();

}


void TaskSmartAlertDialog::smartAlertSelectAll(MCustomTaskSmartAlertSelectAll *selectAll){
    QLOG_INFO()<<"TaskSmartAlertDialog::smartAlertSelectAll()";
    m_webView->page()->runJavaScript(QString("smartAlertSelectAll('%1')").arg(m_i.clickNodeCode), [this,selectAll](const QVariant &v) {
        QString feedback = v.toString();
        QLOG_INFO()<<"选中全部返回：p2";

        stackedWidget->setCurrentWidget(selectAll);
        stackedWidget->show();
        m_i.i_p2V.clear();

        QStringList p2Arrays = feedback.split(SEPARATOR_WEBDATA_ARRAY);
        QStringList nameList;
        for (int i = 0; i < p2Arrays.length(); ++i) {
            QString _p2 = p2Arrays[i];
            if(_p2!=""){
                MEachP2 eachP2(_p2);
                m_i.i_p2V.append(eachP2);
                nameList.append(eachP2.ele_text);
            }
        }
        selectAll->setFieldNameList(nameList);

    });
}
void TaskSmartAlertDialog::smartAlertClickEle(){
    QLOG_INFO()<<"TaskSmartAlertDialog::smartAlertClickEle()";

    m_webView->page()->runJavaScript(QString("smartAlertClickEle('%1','%2')").arg(m_i.clickNodeCode,m_i.i_p2.ele_fullXpath),[this](const QVariant &v) {
        QTimer::singleShot(1000,this,[this](){
            m_fc->addFlowClickStep(m_webView->url().url(),
                                            m_i.i_p2.ele_fullXpath,
                                            m_i.i_p2.ele_text,
                                            NULL,false,false,false);
        });

        this->block();
    });
}
void TaskSmartAlertDialog::smartAlertClickEleTurnPage(){
    QLOG_INFO()<<"TaskSmartAlertDialog::smartAlertClickEleTurnPage()";

    m_webView->page()->runJavaScript(QString("smartAlertClickEle('%1','%2')").arg(m_i.clickNodeCode,m_i.i_p2.ele_fullXpath),[this](const QVariant &v) {

        QTimer::singleShot(1000,this,[this](){

            m_fc->addFlowLoopAndClickStep(
                       m_webView->url().url(),
                       m_i.i_p2.ele_xpath,
                       m_i.i_p2.ele_text,
                       MCONSTANT_RADIO_SINGLE,//loopTypeName
                       m_i.i_p2.ele_xpath,    //loopTypeValue
                       QStringList()<<m_i.i_p2.ele_text,
                       "点击翻页",false,true);

        });
       this->block();
    });

}
void TaskSmartAlertDialog::smartAlertInput(const QString &text){
    QLOG_INFO()<<"TaskSmartAlertDialog::smartAlertInput"<<text;

    m_webView->page()->runJavaScript(QString("smartAlertInput('%1','%2')").arg(m_i.clickNodeCode,text), [this,text](const QVariant &v) {
        Q_UNUSED(v);
        QTimer::singleShot(1000,this,[this,text](){
            m_fc->addFlowInputStep(m_i.i_p2.ele_fullXpath,
                                            m_i.i_p2.ele_text,
                                            text);

        });
       this->block();
    });
}

void TaskSmartAlertDialog::smartAlertSelectAllClickEle(){
    QLOG_INFO()<<"TaskSmartAlertDialog::smartAlertSelectAllClickEle()";

    m_webView->page()->runJavaScript(QString("smartAlertClickEle('%1','%2')").arg(m_i.clickNodeCode,m_i.i_p2.ele_fullXpath), [this](const QVariant &v) {
        Q_UNUSED(v);
        QTimer::singleShot(1000,this,[this](){
            QString nodeRuleUrl = this->m_webView->url().url();
            QStringList nameList;
            for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                nameList.append(m_i.i_p2V[i].ele_text);
            }
            m_fc->addFlowLoopAndClickStep(nodeRuleUrl,
                               m_i.i_p2.ele_fullXpath,
                               m_i.i_p2.ele_text,
                               MCONSTANT_RADIO_FIXED,// 固定元素循环（loop+click 组合）
                               m_i.loopXpath,
                               nameList,
                               NULL,true,false);
        });

       this->block();

    });
}
void TaskSmartAlertDialog::smartAlertMouse(){

    QLOG_INFO()<<"TaskSmartAlertDialog::smartAlertMouse";

    m_webView->page()->runJavaScript(QString("smartAlertMouse('%1')").arg(m_i.clickNodeCode), [this](const QVariant &v) {
        QTimer::singleShot(1000,this,[this](){
            m_fc->addFlowMouseStep(m_i.i_p2.ele_fullXpath,
                                            m_i.i_p2.ele_text);

        });
       this->block();
    });
}
void TaskSmartAlertDialog::smartAlertExtract(const QString &name){
    MParamsField * field = new MParamsField();
    if("A"==m_i.clickNodeName){
        if ("采集该链接的文本"==name) {
             field->fieldActionMethod = "text";
             field->fieldVal = m_i.i_p2.ele_text;
        }else if ("采集该链接地址"==name) {
             field->fieldActionMethod = "href";
             field->fieldVal = m_i.i_p2.ele_custom;

         }else if ("采集该元素的Inner Html"==name) {
             field->fieldActionMethod = "innerHTML";
             field->fieldVal = m_i.i_p2.ele_innerHtml;
         }else if ("采集该元素的Outer Html"==name) {
             field->fieldActionMethod = "outerHTML";
             field->fieldVal = m_i.i_p2.ele_outerHtml;
         }
    }else if ("IMG"==m_i.clickNodeName) {
        if("采集该图片地址" == name){
               field->fieldActionMethod = "src";
               field->fieldVal = m_i.i_p2.ele_custom;
        }else if ("采集该元素的Outer Html"==name) {
            field->fieldActionMethod = "outerHTML";
            field->fieldVal = m_i.i_p2.ele_outerHtml;
        }
    }else if ("INPUT"==m_i.clickNodeName) {
        if("采集该文本框的值" == name){
               field->fieldActionMethod = "value";
               field->fieldVal = m_i.i_p2.ele_custom;
        }else if ("采集该元素的Outer Html"==name) {
            field->fieldActionMethod = "outerHTML";
            field->fieldVal = m_i.i_p2.ele_outerHtml;
        }
    }else {//其他标签
        if ("采集该元素的文本"==name){
             field->fieldActionMethod = "text";
             field->fieldVal = m_i.i_p2.ele_text;
        }else if ("采集该元素的Inner Html"==name) {
             field->fieldActionMethod = "innerHTML";
             field->fieldVal = m_i.i_p2.ele_innerHtml;
        }else if ("采集该元素的Outer Html"==name) {
             field->fieldActionMethod = "outerHTML";
             field->fieldVal = m_i.i_p2.ele_outerHtml;
        }

    }

    m_fc->task->fieldCount+=1;
    field->fieldXpath = m_i.i_p2.ele_fullXpath;
    field->fieldName =QString("字段_%1").arg(m_fc->task->fieldCount);
    field->fieldActionName = name;
    field->fieldTag = m_i.clickNodeName;

    m_webView->page()->runJavaScript(QString("smartAlertExtract('%1')").arg(m_i.clickNodeCode), [this,field](const QVariant &v) {
        m_fc->addFlowExtractStep(field);
        stackedWidget->setCurrentWidget(successWidget);
        successWidget->showMsg("您可继续选择元素",true);
        successWidget->showList(QStringList()<<field->fieldName<<field->fieldVal);

   });

}
void TaskSmartAlertDialog::smartAlertSelectAllExtract(const QString &name){

    m_fc->task->fieldCount+=1;

    MParamsField * field = new MParamsField();
    field->fieldXpath = m_i.i_p2.ele_fullXpath;
    field->fieldName =QString("字段_%1").arg(m_fc->task->fieldCount);
    field->fieldActionName = name;
    field->fieldTag = m_i.clickNodeName;

    QStringList nameList;
    QStringList textList;

    for (int i = 0; i < m_i.i_p2V.length(); ++i) {
        textList.append(m_i.i_p2V[i].ele_text);
    }

    nameList<<field->fieldName;

    if("A"==m_i.clickNodeName){
        if ("采集以下链接文本"==name) {
             field->fieldActionMethod = "text";
             field->fieldVal = m_i.i_p2.ele_text;
             for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                 nameList.append(m_i.i_p2V[i].ele_text);
             }
        }else if ("采集以下链接地址"==name) {
             field->fieldActionMethod = "href";
             field->fieldVal = m_i.i_p2.ele_custom;
             for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                 nameList.append(m_i.i_p2V[i].ele_custom);
             }
         }else if ("采集该元素的Inner Html"==name) {
             field->fieldActionMethod = "innerHTML";
             field->fieldVal = m_i.i_p2.ele_innerHtml;
             for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                 nameList.append(m_i.i_p2V[i].ele_innerHtml);
             }
         }else if ("采集该元素的Outer Html"==name) {
             field->fieldActionMethod = "outerHTML";
             field->fieldVal = m_i.i_p2.ele_outerHtml;
             for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                 nameList.append(m_i.i_p2V[i].ele_outerHtml);
             }
         }

    }else if ("IMG"==m_i.clickNodeName) {
        if("采集以下图片地址" == name){
                field->fieldActionMethod = "src";
                field->fieldVal = m_i.i_p2.ele_custom;
                for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                    nameList.append(m_i.i_p2V[i].ele_custom);
                }
        }

    }else {//其他标签
        if ("采集以下元素文本"==name) {
             field->fieldActionMethod = "text";
             field->fieldVal = m_i.i_p2.ele_text;

             for (int i = 0; i < m_i.i_p2V.length(); ++i) {
                 nameList.append(m_i.i_p2V[i].ele_text);
             }
        }

    }


    m_webView->page()->runJavaScript(QString("smartAlertExtract('%1')").arg(m_i.clickNodeCode), [this,field,textList,nameList](const QVariant &v) {
        m_fc->addFlowLoopAndExtractStep(field,
                                                 MCONSTANT_RADIO_UNFIXED,
                                                 m_i.loopXpath,
                                                 textList);

        stackedWidget->setCurrentWidget(successWidget);
        successWidget->showMsg("您可继续选择元素",true);
        successWidget->showList(nameList);
    });

}

void TaskSmartAlertDialog::onSmartAlertCancel(bool checked){
    Q_UNUSED(checked);
    m_webView->page()->runJavaScript("smartAlertCancel()");
    this->block();
}
void TaskSmartAlertDialog::block(){
    tipLabel->setText("您可继续选取元素");
    xpathWidget->setXpath("");
    stackedWidget->hide();
    this->hide();
    // 如果智能提示未选中，这里会显示，需要处理
    this->show();
}

void TaskSmartAlertDialog::initSuccessWidget(){

    successWidget = new MCustomTaskSmartAlertSuccess(stackedWidget);
    stackedWidget->addWidget(successWidget);

}
void TaskSmartAlertDialog::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    emit this->notifySmartAlertChecked(false);

    m_webView->page()->runJavaScript("smartAlertCancel()");
    this->block();
}

void TaskSmartAlertDialog::initAWidget(){

    aWidget = new MCustomTaskSmartAlertClick(stackedWidget,
         QStringList()<<"选中全部"<<"采集该链接的文本"<<"采集该链接地址"<<"采集该元素的Inner Html"<<"采集该元素的Outer Html"
                      <<"点击该链接"<<"循环点击单个链接"<<"鼠标移动到该链接上");

    connect(aWidget,&MCustomTaskSmartAlertClick::clickedItem,[this](const QString &name){
      if("选中全部"==name){
          smartAlertSelectAll(aWidgetSelectAll);
      }else if ("点击该链接"==name) {
          smartAlertClickEle();
      }else if("循环点击单个链接"==name){
          smartAlertClickEleTurnPage();
      }else if ("鼠标移动到该链接上"==name) {
          smartAlertMouse();
      } else{
          smartAlertExtract(name);
      }
    });

    connect(aWidget->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(aWidget);
}

void TaskSmartAlertDialog::initAWidgetSelectAll(){
    aWidgetSelectAll = new MCustomTaskSmartAlertSelectAll(stackedWidget,
          QStringList()<<"采集以下链接文本"<<"采集以下链接地址"<<"采集该元素的Inner Html"
                       <<"采集该元素的Outer Html"<<"循环点击每个链接");

    connect(aWidgetSelectAll,&MCustomTaskSmartAlertSelectAll::clickedItem,[this](const QString &name){
          if ("循环点击每个链接"==name) {
               smartAlertSelectAllClickEle();
          }else{
               smartAlertSelectAllExtract(name);
        }
    });

    connect(aWidgetSelectAll->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(aWidgetSelectAll);
}

void TaskSmartAlertDialog::initImgWidget(){
    imgWidget = new MCustomTaskSmartAlertClick(stackedWidget,
         QStringList()<<"选中全部"<<"采集该图片地址"<<"点击该图片"<<"采集该元素的Outer Html"
                      <<"循环点击单个图片"<<"鼠标移动到该图片上");
    connect(imgWidget,&MCustomTaskSmartAlertClick::clickedItem,[this](const QString &name){
      if("选中全部"==name){
          smartAlertSelectAll(imgWidgetSelectAll);
      }else if ("点击该图片"==name) {
          smartAlertClickEle();
      }else if("循环点击单个图片"==name){
          smartAlertClickEleTurnPage();
      }else if ("鼠标移动到该图片上"==name) {
          smartAlertMouse();
      }else{
          smartAlertExtract(name);
      }
    });

    connect(imgWidget->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(imgWidget);

}
void TaskSmartAlertDialog::initImgWidgetSelectAll(){
    imgWidgetSelectAll = new MCustomTaskSmartAlertSelectAll(stackedWidget,
          QStringList()<<"采集以下图片地址"<<"循环点击每个图片");

    connect(imgWidgetSelectAll,&MCustomTaskSmartAlertSelectAll::clickedItem,[this](const QString &name){
          if ("循环点击每个图片"==name) {
               smartAlertSelectAllClickEle();
          }else{
               smartAlertSelectAllExtract(name);
        }
    });

    connect(imgWidgetSelectAll->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(imgWidgetSelectAll);
}
void TaskSmartAlertDialog::initInputWidget(){
    inputWidget = new MCustomTaskSmartAlertClick(stackedWidget,QStringList()<<"输入文字"<<"点击该元素"<<"采集该文本框的值"
                                                 <<"采集该元素的Outer Html"<<"鼠标移动到该文本框上");
    connect(inputWidget,&MCustomTaskSmartAlertClick::clickedItem,[this](const QString &name){
      if ("输入文字"==name) {
         stackedWidget->setCurrentWidget(inputinputWidget);
         stackedWidget->show();
      }else if ("点击该元素"==name) {
          smartAlertClickEle();
      }else if ("鼠标移动到该文本框上"==name) {
          smartAlertMouse();
      }else{
         smartAlertExtract(name);
      }
    });

    connect(inputWidget->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(inputWidget);

}
void TaskSmartAlertDialog::initInputInputWidget(){
    inputinputWidget = new QWidget(stackedWidget);
    inputinputWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);}");
    inputinputWidget->setFixedHeight(120);
    QVBoxLayout *iiLayout = new QVBoxLayout(inputinputWidget);
    iiLayout->setContentsMargins(10,0,10,0);
    // 第一行
    QLineEdit *inputLine = new QLineEdit(inputinputWidget);
    inputLine->setStyleSheet(m_stylesheet_QLineEdit);
    inputLine->setFixedHeight(30);
    // 第二行
    QWidget *iibWidget = new QWidget(inputinputWidget);
    iibWidget->setFixedHeight(40);
    QHBoxLayout *iibLayout = new QHBoxLayout(iibWidget);
    iibLayout->setContentsMargins(0,0,0,0);

    QPushButton * okBtn = new QPushButton(iibWidget);
    okBtn->setStyleSheet(m_stylesheet_QPushButton);
    okBtn->setText(tr("确定"));
    okBtn->setFixedSize(70,26);
    okBtn->setCursor(Qt::PointingHandCursor);

    QPushButton * cancelBtn = new QPushButton(iibWidget);
    cancelBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    cancelBtn->setText(tr("取消"));
    cancelBtn->setFixedSize(70,26);
    cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(okBtn,&QPushButton::clicked,[this,inputLine](){
        smartAlertInput(inputLine->text().trimmed());
        inputLine->clear();

    });
    connect(cancelBtn,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);

    iibLayout->addStretch(5);
    iibLayout->addWidget(okBtn);
    iibLayout->addSpacing(10);
    iibLayout->addWidget(cancelBtn);
    iibLayout->addStretch(5);

    iiLayout->addSpacing(20);
    iiLayout->addWidget(inputLine);
    iiLayout->addSpacing(10);
    iiLayout->addWidget(iibWidget);
    iiLayout->addSpacing(20);
    stackedWidget->addWidget(inputinputWidget);

}

void TaskSmartAlertDialog::initCommonWidget(){

    commonWidget = new MCustomTaskSmartAlertClick(stackedWidget,
         QStringList()<<"选中全部"<<"采集该元素的文本"<<"点击该元素"<<"采集该元素的Inner Html"<<"采集该元素的Outer Html"
                      <<"循环点击单个元素"<<"鼠标移动到该元素上");

    connect(commonWidget,&MCustomTaskSmartAlertClick::clickedItem,[this](const QString &name){
      if("选中全部"==name){
          smartAlertSelectAll(commonWidgetSelectAll);
      }else if ("点击该元素"==name) {
          smartAlertClickEle();
      }else if("循环点击单个元素"==name){
          smartAlertClickEleTurnPage();
      }else if ("鼠标移动到该元素上"==name) {
          smartAlertMouse();
      } else{
          smartAlertExtract(name);
      }
    });

    connect(commonWidget->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(commonWidget);

}

void TaskSmartAlertDialog::initCommonWidgetSelectAll(){
    commonWidgetSelectAll = new MCustomTaskSmartAlertSelectAll(stackedWidget,
          QStringList()<<"采集以下元素文本"<<"循环点击每个元素");

    connect(commonWidgetSelectAll,&MCustomTaskSmartAlertSelectAll::clickedItem,[this](const QString &name){
          if ("循环点击每个元素"==name) {
               smartAlertSelectAllClickEle();
          }else{
               smartAlertSelectAllExtract(name);
          }
    });

    connect(commonWidgetSelectAll->cancelButton,&QPushButton::clicked,this,&TaskSmartAlertDialog::onSmartAlertCancel);
    stackedWidget->addWidget(commonWidgetSelectAll);

}

void TaskSmartAlertDialog::onNotificationFromFlowControl(MFlowStepData *data){
    QLOG_INFO()<<"TaskSmartAlertDialog::onNotificationFromFlowControl stepURL = "<<data->stepparams->stepURL;

    if(data->control->menuType==MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn){
        MFlowStepParamsOpenWeb *stepparams =static_cast<MFlowStepParamsOpenWeb *>(data->stepparams);
        m_webView->page()->runJavaScript(QString("smartAlertOpenWeb('%1')").arg(stepparams->url));

    }else if (data->control->menuType==MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn) {
        MFlowStepParamsClickEle *stepparams =static_cast<MFlowStepParamsClickEle *>(data->stepparams);

        m_webView->page()->runJavaScript(QString("smartAlertClickEle('%1','%2')").arg(NULL).arg(stepparams->eleXpath));

    }else if (data->control->menuType==MCONSTANT_FLOW_MENU_TYPE_ExtractBtn) {

        MFlowStepParamsExtract *stepparams =static_cast<MFlowStepParamsExtract *>(data->stepparams);
        QStringList eleXpathArray;
        for (int i = 0; i < stepparams->pfs.length(); ++i) {
            eleXpathArray.append(stepparams->pfs[i]->fieldXpath);
        }
        m_webView->page()->runJavaScript(QString("smartAlertRenderExtract('%1')").arg(eleXpathArray.join(",")));


    }else if (data->control->menuType==MCONSTANT_FLOW_MENU_TYPE_LoopBtn) {

        MFlowStepParamsLoop *stepparams =static_cast<MFlowStepParamsLoop *>(data->stepparams);
        m_webView->page()->runJavaScript(QString("smartAlertRenderLoop('%1')").arg(stepparams->loopTypeValue));


    }
}

void TaskSmartAlertDialog::onClickSignalToQt(const QString &eachp2,const QString &nodeName,const QString &nodeCode,const QString &loopXpath){
    QLOG_INFO()<<"TaskSmartAlertDialog::onClickSignalToQt nodeName="<<nodeName<<"，nodeCode="<<nodeCode<<"，loopXpath="<<loopXpath<<",eachp2="<<eachp2;

    emit this->notifySmartAlertChecked(true);

    m_i.clickNodeName = nodeName;
    m_i.clickNodeCode = nodeCode;
    m_i.eachp2 = eachp2;
    m_i.loopXpath = loopXpath;// 循环xpath
    MEachP2 eachP2(eachp2);
    m_i.i_p2 = eachP2;
    xpathWidget->setXpath(eachP2.ele_fullXpath);

    if("A"==nodeName){
        tipLabel->setText(QString("已选中一个链接，同时发现%1个同类链接，您可以：").arg(m_i.i_p2.sameTypeCount));
        stackedWidget->setCurrentWidget(aWidget);
    }else if ("IMG"==nodeName) {
        tipLabel->setText(QString("已选中一个图片，同时发现%1个同类图片，您可以：").arg(m_i.i_p2.sameTypeCount));
        stackedWidget->setCurrentWidget(imgWidget);
    }else if ("INPUT"==nodeName) {
        tipLabel->setText("已选文本框，您可以：");
        stackedWidget->setCurrentWidget(inputWidget);
    }else {
        tipLabel->setText(QString("已选中一个元素，同时发现%1个同类元素，您可以：").arg(m_i.i_p2.sameTypeCount));
        stackedWidget->setCurrentWidget(commonWidget);
    }
    stackedWidget->show();
}

