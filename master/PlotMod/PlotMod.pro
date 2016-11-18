#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T02:23:46
#
#-------------------------------------------------

QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = PlotMod
TEMPLATE = lib

DEFINES += PLOTMOD_LIBRARY

SOURCES += \
    plotdialog.cpp \
    qcustomplot.cpp \
    graphcurve.cpp \
    ../utils/diagnosis.cpp \
    ../utils/indexsequenceset.cpp \
    ledctrl.cpp

HEADERS +=\
        plotmod_global.h \
    plotdialog.h \
    qcustomplot.h \
    graphcurve.h \
    ../utils/diagnosis.h \
    ../utils/indexsequenceset.h \
    ../utils/utils.h \
    ledctrl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    plotdialog.ui \
    ledctrl.ui
