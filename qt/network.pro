#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T17:13:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = network
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include($$PWD/gloox.pri)

INCLUDEPATH += $$PWD/../network/Classes \
               $$PWD/../libs/gloox-1.0.11/src

HEADERS +=  $$PWD/../network/Classes/net/client.h \
            $$PWD/../network/Classes/net/datahandler.h \
            $$PWD/network.h \
            $$PWD/nwapplication.h


SOURCES +=  $$PWD/main.cpp \
            $$PWD/../network/Classes/net/client.cpp \
            $$PWD/../network/Classes/net/datahandler.cpp \
            $$PWD/nwapplication.cpp
