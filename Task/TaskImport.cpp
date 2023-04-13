#include "TaskImport.h"
#include "style.h"
#include "Utils/models.h"
#include "Utils/database.h"
#include "Utils/ComMessageBox.h"
#include "Utils/ComLineWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QFileDialog>
#include <QsLog.h>

TaskImport::TaskImport(QWidget *parent) : QDialog(parent)
{
    setFixedSize(520,200);

    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet(".TaskImport{background-color:rgb(255,255,255);}");
    setWindowTitle("导入任务");

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(20,10,20,0);


    QWidget *gWidget = new QWidget(this);

    QGridLayout *gLayout = new QGridLayout(gWidget);
    gLayout->setColumnMinimumWidth(0,60);
    gLayout->setColumnMinimumWidth(2,110);

    // 第一行
    QLabel *nameLabel = new QLabel(gWidget);
    nameLabel->setText("任务名称");
    QLineEdit *nameLine = new QLineEdit(gWidget);
    nameLine->setFixedHeight(30);
    nameLine->setStyleSheet(m_stylesheet_QLineEdit);

    QPushButton *selectBtn = new QPushButton(gWidget);
    selectBtn->setText("选择...");
    selectBtn->setCursor(Qt::PointingHandCursor);
    selectBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    selectBtn->setFixedSize(90,28);

    connect(selectBtn,&QPushButton::clicked,this,[this,nameLine](){
       QString filename = QFileDialog::getOpenFileName(this,"选择任务文件","","*.ds");
       if(filename.length()>0){
          nameLine->setText(filename);
       }
    });

    gLayout->addWidget(nameLabel,0,0,Qt::AlignCenter);
    gLayout->addWidget(nameLine,0,1);
    gLayout->addWidget(selectBtn,0,2,Qt::AlignCenter);

    // 第二行
    QLabel *groupNameLabel = new QLabel(gWidget);
    groupNameLabel->setText("任务组");

    QComboBox *groupComboBox = new QComboBox(gWidget);
    groupComboBox->setFixedHeight(30);
    QStyledItemDelegate *groupDelegate = new QStyledItemDelegate(groupComboBox);
    groupComboBox->setItemDelegate(groupDelegate);
    groupComboBox->setStyleSheet(m_stylesheet_QComboBox);

    QVector<MTaskGroup> taskGroups = Database::getInstance()->taskGroups;

    for (int i = 0; i < taskGroups.length(); ++i) {
        if(i==0){
            m_currentGroupId = taskGroups[i].id;
        }
        groupComboBox->addItem(taskGroups[i].name);
    }

    connect(groupComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[this,taskGroups](int index){
         m_currentGroupId = taskGroups[index].id;
    });

    gLayout->addWidget(groupNameLabel,1,0,Qt::AlignCenter);
    gLayout->addWidget(groupComboBox,1,1);
    boxLayout->addWidget(gWidget);

    boxLayout->addWidget(new ComLineWidget(this));
    // 第三行(底部菜单)
    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomHLayout = new QHBoxLayout(bottomWidget);
    bottomHLayout->setContentsMargins(0,0,0,0);
    bottomHLayout->setSpacing(0);
    QPushButton *okBtn = new QPushButton(bottomWidget);
    okBtn->setText("确定");
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setStyleSheet(m_stylesheet_QPushButton);
    okBtn->setFixedSize(80,28);
    connect(okBtn,&QPushButton::clicked,this,[this,nameLine](){
        QString filename = nameLine->text().trimmed();
        if(filename.length()>0){
            MTask *task = new MTask();
            task->groupId = m_currentGroupId;

            if(task->fromFile(filename)){
                task->defaultUserAgent =  Database::getInstance()->getRandomUserAgent();

                m_importSuccessTasks.append(task);// 导出成功的任务放入数组

                QString msg;
                bool state = Database::getInstance()->addTask(task,msg);

                if(state==true){
                    ComMessageBox::success(this,msg);
                    this->close();
                }else{
                    ComMessageBox::error(this,msg);
                }


            }else{
                delete task;
                ComMessageBox::error(this,"您导入的任务可能已损毁！");
            }
        }else{
            ComMessageBox::error(this,"请选择任务！");
        }
    });

    QPushButton *cancelBtn = new QPushButton(bottomWidget);
    cancelBtn->setText("取消");
    cancelBtn->setCursor(Qt::PointingHandCursor);
    cancelBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    cancelBtn->setFixedSize(80,28);
    connect(cancelBtn,&QPushButton::clicked,this,[this](){
        this->close();
    });
    bottomHLayout->addStretch(10);
    bottomHLayout->addWidget(okBtn,0,Qt::AlignRight);
    bottomHLayout->addSpacing(10);
    bottomHLayout->addWidget(cancelBtn,0,Qt::AlignRight);
    bottomHLayout->addSpacing(20);
    boxLayout->addWidget(bottomWidget);
}
TaskImport::~TaskImport(){

    for (int i = 0; i < m_importSuccessTasks.length(); ++i) {
        delete m_importSuccessTasks[i];
    }
}
