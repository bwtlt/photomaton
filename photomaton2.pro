######################################################################
# Automatically generated by qmake (3.1) Sun Aug 2 15:47:48 2020
######################################################################

TEMPLATE = app
TARGET = photomaton2
INCLUDEPATH += .
QT += widgets
LIBS += -lraspicam -lopencv_core -lopencv_imgproc -lwiringPi

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/gpio.h \
           src/image.h \
           src/imgprocessing.h \
           src/mainwindow.h \
           src/videostream.h \
           src/slideshow.h
FORMS += src/mainwindow.ui
SOURCES += src/gpio.cpp \
           src/imgprocessing.cpp \
           src/image.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/videostream.cpp \
           src/slideshow.cpp
