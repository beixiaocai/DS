#include "Task.h"
#include "TaskDo.h"
#include "TaskSettings.h"
#include "style.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "Utils/ComMessageBox.h"
#include "Utils/ComLineWidget.h"
#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>
#include <QTimer>

Task::Task(MTask *task,QWidget *parent) : QWidget(parent),m_task(task)
{

     setAttribute(Qt::WA_StyledBackground,true);
     setStyleSheet(QString(".Task{background-color:%1;}").arg(m_rgb_basic));

     QVBoxLayout *boxLayout = new QVBoxLayout(this);
     boxLayout->setContentsMargins(0,2,0,0);
     boxLayout->setSpacing(0);

     boxLayout->addWidget(new ComLineWidget(this));


     stackedWidget = new QStackedWidget(this);
     boxLayout->addWidget(stackedWidget);


    if(m_task->type == MTask::TASKTYPE::ADD){
        // 生成任务
        initAddView();
        QTimer::singleShot(50,this,[this](){
            initAddViewDo();
            initSettings();
         });
        stackedWidget->setCurrentWidget(addView);
    }else{
        // 编辑任务
        initAddViewDo();
        stackedWidget->setCurrentWidget(mTaskDo);

        QTimer::singleShot(50,this,[this](){
            initAddView();
            initSettings();
            mTaskDo->startLoad();

         });
    }
}
Task::~Task(){
    delete m_task;
    m_task = nullptr;

}


void Task::initSettings(){
    mSettings = new TaskSettings(m_task,stackedWidget);
    connect(mSettings,&TaskSettings::notifyBack,this,[this](){
        stackedWidget->setCurrentWidget(mTaskDo);

    });
    stackedWidget->addWidget(mSettings);
}
void Task::initAddViewDo(){

    mTaskDo = new TaskDo(m_task,stackedWidget);
    connect(mTaskDo, &TaskDo::notifyChangeTabName,this,&Task::notifyChangeTabName);
    connect(mTaskDo, &TaskDo::notifyChangeTabIcon,this,&Task::notifyChangeTabIcon);

    connect(mTaskDo,&TaskDo::notifySettings,this,[this](){
        stackedWidget->setCurrentWidget(mSettings);
    });

    stackedWidget->addWidget(mTaskDo);

}

void Task::initAddView(){
    addView = new QWidget(stackedWidget);

    // 任务组
    QLabel *groupLabel = new QLabel(addView);
    groupLabel->setStyleSheet(m_stylesheet_QLabel14);
    groupLabel->setText("任务组");
    groupComboBox = new QComboBox(addView);
    groupComboBox->setMaximumWidth(300);
    groupComboBox->setMinimumHeight(35);
    groupComboBox->setItemDelegate(new QStyledItemDelegate(groupComboBox));
    groupComboBox->setStyleSheet(m_stylesheet_QComboBox);

    m_taskGroups = Database::getInstance()->taskGroups;

    for (int i = 0; i < m_taskGroups.length(); ++i) {
        groupComboBox->addItem(m_taskGroups[i].name);
        if(i==0){
            m_currentGroupId = m_taskGroups[i].id;
        }
    }
    connect(groupComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
         m_currentGroupId = m_taskGroups[index].id;

    });

    // 内容
    QLabel *addressLabel = new QLabel(addView);
    addressLabel->setStyleSheet(m_stylesheet_QLabel14);
    addressLabel->setText("网址");

    QTextEdit *addressText = new QTextEdit(addView);
    addressText->setMaximumHeight(400);
    addressText->setPlaceholderText("例如：http://www.any12345.com");
    addressText->setStyleSheet(m_stylesheet_QTextEdit);
    addressText->setFontFamily("Microsoft YaHei");
    addressText->setText(m_task->addressList.join("\n")); // 设置默认值（可能存在携带过来的参数）


    QPushButton *okBtn = new QPushButton(addView);
    okBtn->setStyleSheet(m_stylesheet_QPushButton);
    okBtn->setFixedSize(200,32);
    okBtn->setCursor(Qt::PointingHandCursor);

    okBtn->setText("保存");
    connect(okBtn,&QPushButton::clicked,this,[this,addressText](){
        if(mTaskDo!=nullptr){
            QString value = addressText->toPlainText().trimmed();
            if(value.length()>0){
                value = value.replace(",","\n");// 替换掉英文逗号，后面会有使用

                QStringList values = value.split("\n");
                QStringList addressList;
                for (int i = 0; i < values.length(); ++i) {
                    if(values[i].startsWith("http")){
                        addressList.append(values[i].trimmed());
                    }
                }
                if(addressList.length()>0){
                    m_task->groupId = m_currentGroupId;
                    m_task->addressList = addressList;

                    stackedWidget->setCurrentWidget(mTaskDo);
                    mTaskDo->startLoad();
                }else {
                    ComMessageBox::error(this,"请输入至少一条有效网址");
                }
            }else{
                  ComMessageBox::error(this,"请输入网址");
            }

        }else {
             ComMessageBox::error(this,"页面初始化中，请重试！");
        }
    });

    // tip
    QLabel *tipLabel = new QLabel(addView);
    tipLabel->setText("注意：换行表示输入不同的网址");
    tipLabel->setStyleSheet("QLabel{color:rgb(153,153,153);font-family:Microsoft YaHei;font-size:12px;}");


    QGridLayout *gridLayout = new QGridLayout(addView);
    gridLayout->setContentsMargins(20,20,20,0);
    gridLayout->setSpacing(20);
//    gridLayout->setColumnStretch(2,10);
    gridLayout->addWidget(groupLabel,0,0);
    gridLayout->addWidget(groupComboBox,0,1);


    gridLayout->addWidget(addressLabel, 1, 0);
    gridLayout->addWidget(addressText, 1, 1);

    gridLayout->addWidget(tipLabel,2,1);
    gridLayout->addWidget(okBtn,3,1);



    gridLayout->setRowStretch(4,100);

    stackedWidget->addWidget(addView);
}
