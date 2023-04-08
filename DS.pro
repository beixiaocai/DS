#-------------------------------------------------
#
# Project created by QtCreator 2023-03-12T15:48:22
#
#-------------------------------------------------

QT += core webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET = DS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_DEBUG_OUTPUT
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#程序获取管理员权限 start
#（注意：获取管理员权限以后，调试起来麻烦一些，建议在开发状态下，不要开启，这里之所以开启管理员权限）
# QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
#程序获取管理员权限 end

CONFIG += c++11
CONFIG += precompile_header
CONFIG += release
PRECOMPILED_HEADER=stable.h

SOURCES += \
        mainwindow.cpp \
        tab.cpp \
        main.cpp

HEADERS += \
         mainwindow.h \
         tab.h \
         style.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    README.md

RC_ICONS = logo.ico # 配置桌面软件和的图标

include(3rdparty/3rdparty.pri)
include(Index/Index.pri)
include(Utils/Utils.pri)
include(Task/Task.pri)
include(TaskFlow/TaskFlow.pri)
include(Run/Run.pri)


