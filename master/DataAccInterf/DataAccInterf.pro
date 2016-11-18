#-------------------------------------------------
#
# Project created by QtCreator 2015-05-22T20:10:18
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = DataAccInterf
TEMPLATE = lib

DEFINES += DATAACCINTERF_LIBRARY

SOURCES += \
    dataaccif.cpp \
    financedatasource.cpp

HEADERS +=\
    dataaccessinterf_global.h \
    dataaccif.h \
    financedatasource.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
