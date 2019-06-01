#-------------------------------------------------
#
# Project created by QtCreator 2019-05-28T13:25:13
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = Simple-Home-Budget-Manager
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/TagsTab.cpp \
    src/TableEditBox.cpp \
    src/DialogForRegularTransfers.cpp

HEADERS += \
    src/MainWindow.hpp \
    src/TagsTab.hpp \
    src/TableEditBox.hpp \
    src/DialogForRegularTransfers.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
