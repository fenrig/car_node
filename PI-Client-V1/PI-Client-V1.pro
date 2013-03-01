#-------------------------------------------------
#
# Project created by QtCreator 2013-02-25T23:43:39
#
#-------------------------------------------------

QT       += core
QT += network
QT       -= gui

TARGET = PI-Client-V1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcp_client.cpp \
    spi.cpp

HEADERS += \
    tcp_client.h \
    spi.h
