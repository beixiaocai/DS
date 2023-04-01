#include "StartupDialog.h"
#include "StartupBox.h"
#include "Utils/ComLineWidget.h"
#include "Utils/models.h"
#include "RunDialog.h"
#include "style.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QStackedWidget>
#include <QTimer>

StartupDialog::StartupDialog(MTask *task) : mTask(task),QDialog(nullptr)
{
    qDebug()<<"StartupDialog::StartupDialog()";
    resize(600,400);

    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(QString(".StartupDialog{background-color:%1;}").arg(m_rgb_basic));

    setWindowTitle("启动设置");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,2,0,0);
    mainLayout->setSpacing(0);


    mainLayout->addWidget(new ComLineWidget(this));
    QWidget * boxWidget = new QWidget(this);
    boxHLayout = new QHBoxLayout(boxWidget);

    mainLayout->addWidget(boxWidget);
    boxHLayout->setContentsMargins(0,0,0,0);
    boxHLayout->setSpacing(0);

    initLeftUi();

    boxHLayout->addWidget(new ComLineWidget(this,1));

    rightStackedWidget = new QStackedWidget(this);
    boxHLayout->addWidget(rightStackedWidget);

    initRightDirectStartUi();
    rightStackedWidget->setCurrentWidget(rightDirectStartWidget);

    QTimer::singleShot(50,this,[this](){
          initRightTimedStartUi();
    });

}
StartupDialog::~StartupDialog(){
    qDebug()<<"StartupDialog::~StartupDialog()";
    if(mTask){
        delete mTask ;
        mTask = nullptr;
    }
}
void StartupDialog::initRightTimedStartUi(){
    rightTimedStartWidget = new QWidget(rightStackedWidget);
//    rightDirectStartWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);}");
    QVBoxLayout *vLayout = new QVBoxLayout(rightTimedStartWidget);
    vLayout->setContentsMargins(0,0,0,0);

    // 介绍行
    QWidget * introWidget = new QWidget(rightTimedStartWidget);
    introWidget->setFixedHeight(40);
    QHBoxLayout *introHLayout = new QHBoxLayout(introWidget);
    introHLayout->setContentsMargins(0,0,0,0);

    QLabel *introLabel = new QLabel(introWidget);
    introLabel->setStyleSheet(".QLabel{color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:15px;}");
    introLabel->setText("定时周期性执行");

    introHLayout->addStretch(10);
    introHLayout->addWidget(introLabel);
    introHLayout->addStretch(10);


    StartupBox *startupBox = new StartupBox(rightTimedStartWidget);


    //是否循环采集 start
    rightLoopCb = new QCheckBox(startupBox->gWidget);
    rightLoopCb->setStyleSheet(m_stylesheet_QCheckBox);
    rightLoopCb->setText("循环采集");
    rightLoopCb->setChecked(true);

    startupBox->gLayout->addWidget(rightLoopCb,startupBox->rowStart,0);
    ++startupBox->rowStart;
    //是否循环采集 send


    //循环间隔时间 start
    QLabel *intervalLabel = new QLabel(startupBox->gWidget);
    intervalLabel->setStyleSheet(m_stylesheet_QLabel);
    intervalLabel->setText("循环间隔时间");
    rightLoopIntervalSpin = new QSpinBox(startupBox->gWidget);
    rightLoopIntervalSpin->setMinimum(1);
    rightLoopIntervalSpin->setMaximum(1440);
    rightLoopIntervalSpin->setSuffix("分钟");
    rightLoopIntervalSpin->setMaximumWidth(160);
    rightLoopIntervalSpin->setStyleSheet(m_stylesheet_QSpinBox);
    rightLoopIntervalSpin->setValue(10);

    startupBox->gLayout->addWidget(intervalLabel,startupBox->rowStart,0);
    startupBox->gLayout->addWidget(rightLoopIntervalSpin,startupBox->rowStart,1);
    ++startupBox->rowStart;

    //循环间隔时间 end


    //单次运行时间 start

    QLabel *singleLabel = new QLabel(startupBox->gWidget);
    singleLabel->setStyleSheet(m_stylesheet_QLabel);
    singleLabel->setText("循环单次时长");

    rightLoopSingleSpin = new QSpinBox(startupBox->gWidget);
    rightLoopSingleSpin->setMinimum(0);
    rightLoopSingleSpin->setMaximum(0);
    rightLoopSingleSpin->setSuffix("分钟");
    rightLoopSingleSpin->setMaximumWidth(160);
    rightLoopSingleSpin->setStyleSheet(m_stylesheet_QSpinBox);
    rightLoopSingleSpin->setValue(0);

    QLabel *singleRemarkLabel = new QLabel(startupBox->gWidget);
    singleRemarkLabel->setStyleSheet(m_stylesheet_QLabel);
    singleRemarkLabel->setText("0表示不限制单次时长");

    startupBox->gLayout->addWidget(singleLabel,startupBox->rowStart,0);
    startupBox->gLayout->addWidget(rightLoopSingleSpin,startupBox->rowStart,1);
    startupBox->gLayout->addWidget(singleRemarkLabel,startupBox->rowStart,2);
    ++startupBox->rowStart;
    //循环间隔时间 end


    // 任务提交行
    QWidget * submitWidget = new QWidget(rightDirectStartWidget);
    submitWidget->setFixedHeight(50);
    QHBoxLayout *submitHLayout = new QHBoxLayout(submitWidget);
    submitHLayout->setContentsMargins(0,0,0,0);

    QPushButton *submitBtn = new QPushButton(submitWidget);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setFixedSize(120,50);
    submitBtn->setStyleSheet(m_stylesheet_QPushButton);
    submitBtn->setText("启动");

    submitHLayout->addStretch(10);
    submitHLayout->addWidget(submitBtn);
    submitHLayout->addStretch(10);

    connect(submitBtn,&QPushButton::clicked,this,[this](){
        bool isLoop = false;
        if(rightLoopCb->checkState()==Qt::CheckState::Checked){
            isLoop = true;
        }
        MTaskRunParams runParams;
        runParams.isLoop = isLoop;
        runParams.loopInterval = rightLoopIntervalSpin->value();//循环间隔时长
        runParams.loopSingle = rightLoopSingleSpin->value();//循环单次间隔时长
        mTask->runParams = runParams;

        RunDialog *runDialog = new RunDialog(mTask);// 交给RunDialog界面释放task
        runDialog->setAttribute(Qt::WA_DeleteOnClose);
        runDialog->show();
        mTask = nullptr;

        this->close();
    });

    vLayout->addSpacing(30);
    vLayout->addWidget(introWidget);
    vLayout->addSpacing(10);
    vLayout->addWidget(startupBox);
    vLayout->addStretch(10);
    vLayout->addWidget(submitWidget);
    vLayout->addSpacing(30);

    rightStackedWidget->addWidget(rightTimedStartWidget);

}
void StartupDialog::initRightDirectStartUi(){
    rightDirectStartWidget = new QWidget(rightStackedWidget);
//    rightDirectStartWidget->setStyleSheet(".QWidget{background-color:rgb(255,255,255);}");
    QVBoxLayout *vLayout = new QVBoxLayout(rightDirectStartWidget);
    vLayout->setContentsMargins(0,0,0,0);

    // 介绍行
    QWidget * introWidget = new QWidget(rightDirectStartWidget);
  introWidget->setFixedHeight(40);
    QHBoxLayout *introHLayout = new QHBoxLayout(introWidget);
    introHLayout->setContentsMargins(0,0,0,0);

    QLabel *summaryLabel = new QLabel(introWidget);
    summaryLabel->setStyleSheet(".QLabel{color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:16px;}");
    summaryLabel->setText("一次性执行");

    introHLayout->addStretch(10);
    introHLayout->addWidget(summaryLabel);
    introHLayout->addStretch(10);


    // 任务提交行
    QWidget * submitWidget = new QWidget(rightDirectStartWidget);
    submitWidget->setFixedHeight(50);
    QHBoxLayout *submitHLayout = new QHBoxLayout(submitWidget);
    submitHLayout->setContentsMargins(0,0,0,0);

    QPushButton *submitBtn = new QPushButton(submitWidget);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setFixedSize(120,50);
    submitBtn->setStyleSheet(m_stylesheet_QPushButton);
    submitBtn->setText("启动");

    submitHLayout->addStretch(10);
    submitHLayout->addWidget(submitBtn);
    submitHLayout->addStretch(10);

    connect(submitBtn,&QPushButton::clicked,this,[this](){
        MTaskRunParams runParams;
        runParams.isLoop = false;
        mTask->runParams = runParams;

        RunDialog *runDialog = new RunDialog(mTask);// 交给RunDialog界面释放task
        runDialog->setAttribute(Qt::WA_DeleteOnClose);
        runDialog->show();
        mTask = nullptr;

        this->close();
    });

    vLayout->addSpacing(30);
    vLayout->addWidget(introWidget);
    vLayout->addStretch(10);
    vLayout->addWidget(submitWidget);
    vLayout->addSpacing(30);

    rightStackedWidget->addWidget(rightDirectStartWidget);
}

void StartupDialog::initLeftUi(){
    QWidget *leftWidget = new QWidget(this);
    leftWidget->setFixedWidth(120);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(0);

    leftDirectStartBtn = new QPushButton(leftWidget);
    leftDirectStartBtn->setCursor(Qt::PointingHandCursor);
    leftDirectStartBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
    leftDirectStartBtn->setFixedSize(100,32);
    leftDirectStartBtn->setText("直接启动");

    leftTimedStartBtn = new QPushButton(leftWidget);
    leftTimedStartBtn->setCursor(Qt::PointingHandCursor);
    leftTimedStartBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    leftTimedStartBtn->setFixedSize(100,32);
    leftTimedStartBtn->setText("定时启动");



    connect(leftDirectStartBtn,&QPushButton::clicked,this,[this](){
        leftDirectStartBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
        leftTimedStartBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
        rightStackedWidget->setCurrentWidget(rightDirectStartWidget);

    });

    connect(leftTimedStartBtn,&QPushButton::clicked,this,[this](){
        leftDirectStartBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
        leftTimedStartBtn->setStyleSheet(m_stylesheet_QPushButton_blue);
        if(rightTimedStartWidget){
            rightStackedWidget->setCurrentWidget(rightTimedStartWidget);
        }
    });




    leftLayout->addSpacing(40);
    leftLayout->addWidget(leftDirectStartBtn,0,Qt::AlignHCenter);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(leftTimedStartBtn,0,Qt::AlignHCenter);
    leftLayout->addStretch(40);
    boxHLayout->addWidget(leftWidget);

}

