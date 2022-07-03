QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cJSON.c \
    edge.cpp \
    fsm.cpp \
    ifconditionoptdialog.cpp \
    inputBoard.cpp \
    inputBoardproperty.cpp \
    main.cpp \
    mainwindow.cpp \
    mqtt.cpp \
    msgQueue.cpp \
    mysquare.cpp \
    node.cpp \
    outputBoard.cpp \
    outputBoardproperty.cpp \
    signalThread.cpp

HEADERS += \
    cJSON.h \
    diagram.h \
    edge.h \
    fsm.h \
    ifconditionoptdialog.h \
    inputBoard.h \
    inputBoardproperty.h \
    mainwindow.h \
    mqtt.h \
    msgQueue.hpp \
    mysquare.h \
    node.h \
    outputBoard.h \
    outputBoardproperty.h \
    signalThread.h

FORMS += \
    ifconditionoptdialog.ui \
    ioBoardProperty.ui \
    mainwindow.ui \
    test.ui

QMAKE_CXXFLAGS += -std=c++0x
LIBS += -lmosquitto

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
