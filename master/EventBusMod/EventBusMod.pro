#-------------------------------------------------
#
# Project created by QtCreator 2015-05-22T22:56:41
#
#-------------------------------------------------

QT       -= gui

TARGET = EventBusMod
TEMPLATE = lib

DEFINES += EVENTBUSMOD_LIBRARY

SOURCES += eventbusmod.cpp

HEADERS += eventbusmod.h\
        eventbusmod_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
