#ifndef STYLE_H
#define STYLE_H
#include <QString>
// 字体 https://blog.csdn.net/qq_37370501/article/details/81016910


const QString m_rgb_basic = "rgb(246,247,251)";

const QString m_stylesheet_QLabel = ".QLabel{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:13px;}";
const QString m_stylesheet_QLabel12 = ".QLabel{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:12px;}";
const QString m_stylesheet_QLabel14 = ".QLabel{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:14px;}";
const QString m_stylesheet_QLabel18 = ".QLabel{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:18px;}";
const QString m_stylesheet_QLabel30 = ".QLabel{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:30px;}";

const QString m_stylesheet_QCheckBox = "QCheckBox{color:rgb(0,0,0);font-family:Microsoft YaHei;font-size:13px;height:20px;border-radius:2px;};\
          QCheckBox::indicator{width:16px;height:16px;};";

const QString m_stylesheet_QLineEdit = ".QLineEdit{color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:13px;border:1px solid rgb(217,217,217);border-radius: 3px; padding: 1px;}\
         .QLineEdit:hover {border:1px solid rgb(64,65,66);}";

const QString m_stylesheet_QTextEdit = ".QTextEdit{color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:13px;border:1px solid rgb(217,217,217);border-radius: 3px; padding: 1px;}\
         .QTextEdit:hover {border:1px solid rgb(64,65,66);}";

const QString m_stylesheet_QSpinBox = ".QSpinBox{color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:13px;border:1px solid rgb(217,217,217);border-radius: 3px; padding: 1px;}\
         .QSpinBox:hover {border:1px solid rgb(64,65,66);}";

const QString m_stylesheet_QComboBox =".QComboBox {color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:13px; border:1px solid rgb(217,217,217);border-radius: 3px; padding: 1px;}\
         .QComboBox:hover {border-color:rgb(64,65,66);}\
         .QComboBox::drop-down{width:5px;} \
         .QComboBox QAbstractItemView{border: 1px solid darkgray;border-radius:2px;outline:0px;} \
         .QComboBox QAbstractItemView::item { height: 28px;}\
         .QComboBox QAbstractItemView::item:selected {background-color:rgb(54,98,180);}";

const QString m_stylesheet_QToolButton_hollow = ".QToolButton {color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:12px;background-color:white; border:1px solid rgb(64,65,66); border-radius: 3px;padding: 2px;}\
         .QToolButton:pressed {background-color: rgba(240,240,240,0.8);}\
         .QToolButton:hover {background-color: rgba(240,240,240,0.4); border-color:rgba(64,65,66,0.5);}";

const QString m_stylesheet_QToolButton_empty = "QToolButton{border:0px;}";

// 背景为黑色，有边框
const QString m_stylesheet_QPushButton = ".QPushButton {color:white;font-family:Microsoft YaHei;font-size:14px;background-color:rgb(64,65,66);border:none; border-radius: 3px;padding: 2px;}\
         .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
         .QPushButton:hover {background-color: rgba(64,65,66,0.5);}\
         .QPushButton:focus{outline: none;}";
// 背景为蓝色，有边框
const QString m_stylesheet_QPushButton_blue = ".QPushButton {color:white;font-family:Microsoft YaHei;font-size:14px;background-color:rgb(43,113,237);border:none; border-radius: 3px;padding: 2px;}\
         .QPushButton:pressed {background-color: rgba(43,113,237,0.5);}\
         .QPushButton:hover {background-color: rgba(43,113,237,0.5);}\
         .QPushButton:focus{outline: none;}";

// 背景为白色，边框为黑色
const QString m_stylesheet_QPushButton_hollow = ".QPushButton {color:rgb(64,65,66);font-family:Microsoft YaHei;font-size:14px;background-color:white; border:1px solid rgb(64,65,66); border-radius: 3px;padding: 2px;}\
         .QPushButton:pressed {background-color: rgba(240,240,240,0.8);}\
         .QPushButton:hover {background-color: rgba(240,240,240,0.4); border-color:rgba(64,65,66,0.5);}\
         .QPushButton:focus{outline: none;}";
// 任务流的左侧和右侧滚动框
const QString m_stylesheet_QScrollArea = "QScrollArea{border:none;}\
          QScrollBar::vertical{background:#484848;padding:0px;border-radius:0px;max-width:6px;}\
          QScrollBar::handle:vertical{background:#CCCCCC;}\
          QScrollBar::handle:hover:vertical,QScrollBar::handle:pressed:vertical{background:#A7A7A7;}\
          QScrollBar::sub-page:vertical{background:444444;}\
          QScrollBar::add-page:vertical{background:5B5B5B;}\
          QScrollBar::add-line:vertical{background:none;}\
          QScrollBar::sub-line:vertical{background:none;}";

const QString m_stylesheet_QToolBar = "QToolBar {background-color:rgb(56,75,113);spacing:8px;}\
          QToolBar QToolButton {color: rgb(246,247,251);text-align: left; padding:3px 5px; margin:2px 2px;}\
          QToolBar QToolButton:hover {border:1px solid rgb(43,113,237); }";


#endif // STYLE_H
