TEMPLATE = app
CONFIG += console

#QMAKE_CC = gcc-4.9
#QMAKE_CXX = g++-4.9

QMAKE_CXXFLAGS += -std=c++0x -frounding-math -Wall
INCLUDEPATH += /usr/include
LIBS += -L/usr/include
LIBS +=  -lCGAL -lgmp -lboost_system -lboost_thread

DEFINES += DCGAL_DISABLE_ROUNDING_MATH_CHECK=1


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Part2
TEMPLATE = app


SOURCES += main.cpp \
    cgal.cpp \
    boundingsphere.cpp \
    io.cpp \
    pixel.cpp

HEADERS  += \
    iboundingobject.h \
    cgal.h \
    boundingsphere.h \
    polyhedronloader.h \
    io.h \
    image.h \
    pixel.h


