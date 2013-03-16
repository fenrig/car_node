#-------------------------------------------------
#
# Project created by QtCreator 2013-02-25T23:43:39
#
#-------------------------------------------------

QT       += core
QT += network
QT       -= gui
QT += testlib

TARGET = PI-Client-V1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcp_client.cpp \
    spi.cpp \
    binder.cpp \
    FindLine.cpp \
    linefollowingthread.cpp

HEADERS += \
    tcp_client.h \
    spi.h \
    binder.h \
    FindLine.h \
    linefollowingthread.h

#Laptop Dries
#INCLUDEPATH += /home/dries/OpenCV/OpenCV-2.4.3/build/include
#LIBS    += -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ts -lopencv_video

#Raspberry PI Dries
INCLUDEPATH += ~/usr/include
LIBS    += -L/usr/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ts -lopencv_video
