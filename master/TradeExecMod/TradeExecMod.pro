#-------------------------------------------------
#
# Project created by QtCreator 2015-05-24T23:18:10
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = TradeExecMod
TEMPLATE = lib

DEFINES += TRADEEXECMOD_LIBRARY

SOURCES += tradeexecmod.cpp

HEADERS += tradeexecmod.h\
        tradeexecmod_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
