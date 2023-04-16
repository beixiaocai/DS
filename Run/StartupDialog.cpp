#include "StartupDialog.h"
#include "Utils/ComLineWidget.h"
#include "Utils/ComOptionsBox.h"
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
#include <QsLog.h>

StartupDialog::StartupDialog(MTask *task) : mTask(task),QDialog(nullptr)
{
    QLOG_INFO()<<"StartupDialog::StartupDialog()";
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
    QLOG_INFO()<<"StartupDialog::~StartupDialog()";
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


    ComOptionsBox *optionsBox = new ComOptionsBox(rightTimedStartWidget,"高级选项");


    //开启无痕浏览器 start
    QLabel *tracelessBrowserLabel = new QLabel(optionsBox->gWidget);
    tracelessBrowserLabel->setStyleSheet(m_stylesheet_QLabel);
    tracelessBrowserLabel->setText("无痕浏览器");

    rightTimedTracelessBrowserCb = new QCheckBox(optionsBox->gWidget);
    rightTimedTracelessBrowserCb->setStyleSheet(m_stylesheet_QCheckBox);
    rightTimedTracelessBrowserCb->setChecked(false);

    optionsBox->gLayout->addWidget(tracelessBrowserLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(rightTimedTracelessBrowserCb,optionsBox->rowStart,1);

    ++optionsBox->rowStart;
    //开启无痕浏览器 end


    //循环间隔时间 start
    QLabel *intervalLabel = new QLabel(optionsBox->gWidget);
    intervalLabel->setStyleSheet(m_stylesheet_QLabel);
    intervalLabel->setText("循环间隔时间");
    rightTimedLoopIntervalSpin = new QSpinBox(optionsBox->gWidget);
    rightTimedLoopIntervalSpin->setMinimum(1);
    rightTimedLoopIntervalSpin->setMaximum(1440);
    rightTimedLoopIntervalSpin->setSuffix("分钟");
    rightTimedLoopIntervalSpin->setMaximumWidth(160);
    rightTimedLoopIntervalSpin->setStyleSheet(m_stylesheet_QSpinBox);
    rightTimedLoopIntervalSpin->setValue(10);

    optionsBox->gLayout->addWidget(intervalLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(rightTimedLoopIntervalSpin,optionsBox->rowStart,1);
    ++optionsBox->rowStart;

    //循环间隔时间 end


    //单次运行时间 start

    QLabel *singleLabel = new QLabel(optionsBox->gWidget);
    singleLabel->setStyleSheet(m_stylesheet_QLabel);
    singleLabel->setText("循环单次时长");

    rightTimedLoopSingleSpin = new QSpinBox(optionsBox->gWidget);
    rightTimedLoopSingleSpin->setMinimum(0);
    rightTimedLoopSingleSpin->setMaximum(0);
    rightTimedLoopSingleSpin->setSuffix("分钟");
    rightTimedLoopSingleSpin->setMaximumWidth(160);
    rightTimedLoopSingleSpin->setStyleSheet(m_stylesheet_QSpinBox);
    rightTimedLoopSingleSpin->setValue(0);

    QLabel *singleRemarkLabel = new QLabel(optionsBox->gWidget);
    singleRemarkLabel->setStyleSheet(m_stylesheet_QLabel);
    singleRemarkLabel->setText("0表示不限制单次时长");

    optionsBox->gLayout->addWidget(singleLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(rightTimedLoopSingleSpin,optionsBox->rowStart,1);
    optionsBox->gLayout->addWidget(singleRemarkLabel,optionsBox->rowStart,2);
    ++optionsBox->rowStart;
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
        bool isTraceless = false;
        if(rightTimedTracelessBrowserCb->checkState()==Qt::CheckState::Checked){
            isTraceless = true;
        }

        MTaskRunParams runParams;
        runParams.isTraceless = isTraceless;
        runParams.isLoop = true;
        runParams.loopInterval = rightTimedLoopIntervalSpin->value();//循环间隔时长
        runParams.loopSingle = rightTimedLoopSingleSpin->value();//循环单次间隔时长
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
    vLayout->addWidget(optionsBox);
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

    ComOptionsBox *optionsBox = new ComOptionsBox(rightDirectStartWidget,"高级选项");

    //开启无痕浏览器 start
    QLabel *tracelessBrowserLabel = new QLabel(optionsBox->gWidget);
    tracelessBrowserLabel->setStyleSheet(m_stylesheet_QLabel);
    tracelessBrowserLabel->setText("无痕浏览器");

    rightDirectTracelessBrowserCb = new QCheckBox(optionsBox->gWidget);
    rightDirectTracelessBrowserCb->setStyleSheet(m_stylesheet_QCheckBox);
    rightDirectTracelessBrowserCb->setChecked(false);

    optionsBox->gLayout->addWidget(tracelessBrowserLabel,optionsBox->rowStart,0);
    optionsBox->gLayout->addWidget(rightDirectTracelessBrowserCb,optionsBox->rowStart,1);
    ++optionsBox->rowStart;
    //开启无痕浏览器 end

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
        bool isTraceless = false;
        if(rightDirectTracelessBrowserCb->checkState()==Qt::CheckState::Checked){
            isTraceless = true;
        }
        MTaskRunParams runParams;
        runParams.isTraceless = isTraceless;
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
    vLayout->addSpacing(10);
    vLayout->addWidget(optionsBox);
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

