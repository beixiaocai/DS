#include "mFlowStepMenuDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "style.h"


MFlowStepMenuDialog::MFlowStepMenuDialog(QWidget *parent) : QDialog(parent)
{

    //设置模态
    setWindowModality(Qt::ApplicationModal);
    // 隐藏？号
    setWindowFlags(Qt::Dialog| Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setWindowTitle("操作选项");
    setStyleSheet(".MFlowStepMenuDialog{background-color:rgb(246,247,251);}");

    setFixedSize(120,60);

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0,0,0,0);

    deleteBtn = new QPushButton(this);
    deleteBtn->setStyleSheet(m_stylesheet_QPushButton_hollow);
    deleteBtn->setFixedSize(80,28);
    deleteBtn->setText("删除");

    boxLayout->addWidget(deleteBtn,0,Qt::AlignHCenter);


}
