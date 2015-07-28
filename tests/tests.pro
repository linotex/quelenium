#-------------------------------------------------
#
# Project created by QtCreator
# linotex.m@gmail.com
#
#-------------------------------------------------

QT       += core network testlib

QT       -= gui

TARGET = bin/seleniumtest
CONFIG   += console
CONFIG   -= app_bundle exception

TEMPLATE = app

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

SOURCES += \
    queleniumtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    queleniumtest.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lQuelenium
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lQuelenium
else:unix: LIBS += -L$$PWD/../ -lQuelenium

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libQuelenium.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libQuelenium.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/Quelenium.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/Quelenium.lib
else:unix: PRE_TARGETDEPS += $$PWD/../libQuelenium.a
