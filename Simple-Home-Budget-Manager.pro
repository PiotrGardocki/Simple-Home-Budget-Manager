#-------------------------------------------------
#
# Project created by QtCreator 2019-05-28T13:25:13
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Simple-Home-Budget-Manager
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp

HEADERS += \
    src/MainWindow.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target