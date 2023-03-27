
HEADERS += \
    $$PWD/mFcHelper.h \
    $$PWD/mFcModels.h \
    $$PWD/mTaskDeserialize.h \
    $$PWD/mTaskSerialize.h \
    $$PWD/pageCustomtaskFlow.h \
    $$PWD/pageCustomtaskFlowControl.h
 
SOURCES += \
    $$PWD/mFcHelper.cpp \
    $$PWD/mFcModels.cpp \
    $$PWD/mTaskDeserialize.cpp \
    $$PWD/mTaskSerialize.cpp \
    $$PWD/pageCustomtaskFlow.cpp \
    $$PWD/pageCustomtaskFlowControl.cpp
 
INCLUDEPATH += TaskFlow

include(flowleftwidget/flowleftwidget.pri)
include(flowright/flowright.pri)



