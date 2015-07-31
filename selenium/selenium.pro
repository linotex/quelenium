QT       += network

QT       -= gui


TARGET = ../Quelenium
TEMPLATE = lib
CONFIG += staticlib

OBJECTS_DIR = ../build
MOC_DIR = ../build
UI_DIR = ../build
RCC_DIR = ../build

DEFINES += QTSELENIUM_DEBUG


SOURCES += \
    desiredcapabilities.cpp \
    webdriver.cpp \
    seleniumserverhub.cpp \
    webdriverhub.cpp \
    navigation.cpp \
    options.cpp \
    timeouts.cpp \
    window.cpp \
    cookie.cpp \
    imehandler.cpp \
    logs.cpp \
    logentry.cpp \
    webelement.cpp \
    by.cpp \
    targetlocator.cpp \
    alert.cpp \
    webdriverexception.cpp \
    keys.cpp \
    proxy.cpp \
    actions.cpp \
    mouse.cpp \
    touch.cpp \
    select.cpp \
    server.cpp

HEADERS += \
    desiredcapabilities.h \
    browsertype.h \
    platform.h \
    unexpectedalertbehaviour.h \
    webdriver.h \
    seleniumserverhub.h \
    webdriverhub.h \
    proxy.h \
    responsestatuscodes.h \
    navigation.h \
    options.h \
    timeouts.h \
    window.h \
    cookie.h \
    imehandler.h \
    logs.h \
    logentry.h \
    webelement.h \
    by.h \
    targetlocator.h \
    alert.h \
    webdriverexception.h \
    queleniumexception.h \
    keys.h \
    actions.h \
    mouse.h \
    touch.h \
    select.h \
    server.h
#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
