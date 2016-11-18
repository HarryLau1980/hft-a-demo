#-------------------------------------------------
#
# Project created by QtCreator 2015-05-22T20:11:44
#
#-------------------------------------------------

QT       += core widgets
QT       -= gui

TARGET = TestSuite
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DataAccInterf/release/ -lDataAccInterf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DataAccInterf/debug/ -lDataAccInterf
else:unix: LIBS += -L$$OUT_PWD/../DataAccInterf/ -lDataAccInterf

INCLUDEPATH += $$PWD/../DataAccInterf
DEPENDPATH += $$PWD/../DataAccInterf


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PlotMod/release/ -lPlotMod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PlotMod/debug/ -lPlotMod
else:unix: LIBS += -L$$OUT_PWD/../PlotMod/ -lPlotMod

INCLUDEPATH += $$PWD/../PlotMod
DEPENDPATH += $$PWD/../PlotMod


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../StrategyAnalysisMod/release/ -lStrategyAnalysisMod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../StrategyAnalysisMod/debug/ -lStrategyAnalysisMod
else:unix: LIBS += -L$$OUT_PWD/../StrategyAnalysisMod/ -lStrategyAnalysisMod

INCLUDEPATH += $$PWD/../StrategyAnalysisMod
DEPENDPATH += $$PWD/../StrategyAnalysisMod


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../TradeExecMod/release/ -lTradeExecMod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../TradeExecMod/debug/ -lTradeExecMod
else:unix: LIBS += -L$$OUT_PWD/../TradeExecMod/ -lTradeExecMod

INCLUDEPATH += $$PWD/../TradeExecMod
DEPENDPATH += $$PWD/../TradeExecMod
