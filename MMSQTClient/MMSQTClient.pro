#-------------------------------------------------
#
# Project created by QtCreator 2012-04-09T12:08:02
#
#-------------------------------------------------

QT       += core gui

TARGET = MMSQTClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    readthread.cpp \
    MMSQTHandlers.cpp \
    connectiondialog.cpp \
    graphclass.cpp \
    incrementalplot.cpp \
    getlogs.cpp \
    getgraph.cpp

HEADERS  += mainwindow.h \
    readthread.h \
    MMSQTHandlers.h \
    connectiondialog.h \
    graphclass.h \
    incrementalplot.h \
    getlogs.h \
    getgraph.h \
    basewidget.h

FORMS    += mainwindow.ui \
    connectiondialog.ui \
    getlogs.ui \
    getgraph.ui

CONFIG += qwt

win32:CONFIG(debug, debug|release): CONFIG += console

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -llibFileHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -llibFileHandlerd

INCLUDEPATH += $$PWD/../libFileHandler
DEPENDPATH += $$PWD/../libFileHandler

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libFileHandler.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libFileHandlerd.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -llibMMS
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -llibMMSd

INCLUDEPATH += $$PWD/../libMMS
DEPENDPATH += $$PWD/../libMMS

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libMMS.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libMMSd.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -llibSettings
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -llibSettingsd

INCLUDEPATH += $$PWD/../libSettings
DEPENDPATH += $$PWD/../libSettings

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libSettings.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libSettingsd.lib
