#include "Index.h"
#include "Utils/ComLineWidget.h"
#include "Utils/models.h"
#include "Utils/ApiCheckVersion.h"
#include "Report.h"
#include "style.h"
#include "Version.h"
#include "Utils/constant.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QAction>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>

Index::Index(QWidget *parent) : QWidget(parent)
{

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,2,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(new ComLineWidget(this));

    initUI();
    if(IS_reportHeart){
        mReport= new Report;
        mReport->start();

        connect(ApiCheckVersion::getInstance(),&ApiCheckVersion::notifyCheckVersion,this,[this](bool state,QString &msg,MVersion &version){
            if(state){
                float curVersion = QCoreApplication::applicationVersion().toFloat();
                if(version.version > curVersion){
                    Version dlg(version,this);
                    dlg.exec();
                }
            }
        });
    }

    QTimer::singleShot(2000,this,[](){
        ApiCheckVersion::getInstance()->checkVersion();
    });
}
Index::~Index(){
    if(IS_reportHeart){
        if(mReport){
            delete mReport;
            mReport = nullptr;
        }
    }
}
void Index::initUI(){

    QWidget *boxWidget = new QWidget(this);
    QVBoxLayout *boxVLayout = new QVBoxLayout(boxWidget);
    boxVLayout->setContentsMargins(0,0,0,0);

    // 介绍行
    QWidget * introWidget = new QWidget(boxWidget);
    introWidget->setFixedHeight(50);
    QHBoxLayout *introHLayout = new QHBoxLayout(introWidget);
    introHLayout->setContentsMargins(0,0,0,0);

    QLabel *nameLabel = new QLabel(boxWidget);
    nameLabel->setStyleSheet(m_stylesheet_QLabel30);
    nameLabel->setText(QCoreApplication::applicationName());


    QLabel *summaryLabel = new QLabel(boxWidget);
    summaryLabel->setStyleSheet(".QLabel{color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:16px;}");
    summaryLabel->setText("无需编程的数据采集工具");

    introHLayout->addStretch(10);
    introHLayout->addWidget(nameLabel);
    introHLayout->addSpacing(5);
    introHLayout->addWidget(summaryLabel);
    introHLayout->addStretch(10);

   // 图标行
    QWidget * imgWidget = new QWidget(boxWidget);
    imgWidget->setFixedHeight(100);
    QHBoxLayout *imgHLayout = new QHBoxLayout(imgWidget);
    imgHLayout->setContentsMargins(0,0,0,0);

    QLabel *logo = new QLabel(imgWidget);
    logo->setPixmap(QIcon(":/res/images/logo.png").pixmap(100,100));
   imgHLayout->addStretch(10);
      imgHLayout->addWidget(logo);
   imgHLayout->addStretch(10);

    // 任务提交行
    QWidget * taskWidget = new QWidget(boxWidget);
    taskWidget->setFixedHeight(50);
    QHBoxLayout *taskHLayout = new QHBoxLayout(taskWidget);
    taskHLayout->setContentsMargins(0,0,0,0);


    QLineEdit *inputLine = new QLineEdit(taskWidget);
    inputLine->setFixedSize(500,40);
    inputLine->setPlaceholderText("请输入目标网站的网址");
    inputLine->setStyleSheet(m_stylesheet_QLineEdit);
    inputLine->setClearButtonEnabled(true);
    inputLine->setFocusPolicy(Qt::FocusPolicy::ClickFocus);


    QAction *inputAction = new QAction(inputLine);
    inputAction->setIcon(QIcon(":/res/images/icon/searchblack.png"));
    inputLine->addAction(inputAction,QLineEdit::LeadingPosition);


    QPushButton *customTaskBtn = new QPushButton(taskWidget);
    customTaskBtn->setCursor(Qt::PointingHandCursor);
    customTaskBtn->setFixedSize(110,35);
    customTaskBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    customTaskBtn->setText("新建任务");
    customTaskBtn->setIcon(QIcon(":/res/images/icon/mainwindow/add.png"));
    customTaskBtn->setIconSize(QSize(20,20));


    taskHLayout->addStretch(10);
    taskHLayout->addWidget(inputLine);
    taskHLayout->addSpacing(10);
    taskHLayout->addWidget(customTaskBtn);
    taskHLayout->addStretch(10);

    connect(customTaskBtn,&QPushButton::clicked,this,[this,inputLine](){
        QString address = inputLine->text().trimmed();
        emit this->notifyCreateTask(address);

    });

    boxVLayout->addSpacing(20);
    boxVLayout->addWidget(imgWidget);
    boxVLayout->addSpacing(10);
    boxVLayout->addWidget(introWidget);
    boxVLayout->addSpacing(15);
    boxVLayout->addWidget(taskWidget);
    boxVLayout->addSpacing(50);
    boxVLayout->addStretch(10);


    mainLayout->addWidget(boxWidget);

}
