TEMPLATE = app
CONFIG += console
QT += opengl openglextensions xml glviewer

#QMAKE_CC = gcc-4.9
#QMAKE_CXX = g++-4.9

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv

QMAKE_CXXFLAGS += -std=c++0x -frounding-math -Wall
INCLUDEPATH += /usr/include
LIBS += -L/usr/lib
LIBS +=  -lCGAL -lgmp -lboost_system -lboost_thread -lqglviewer-qt4 -lGLU

DEFINES += DCGAL_DISABLE_ROUNDING_MATH_CHECK=1


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Part2
TEMPLATE = app


SOURCES += main.cpp \
    cgal.cpp \
    io.cpp \
    application.cpp \
    matrixd.cpp \
    drawhelpers.cpp \
    polyhedronalphagenerator.cpp \
    boundingpolyhedron.cpp \
    ialphagenerator.cpp \
    polyhedronfittingalhorithms.cpp

HEADERS  += \
    iboundingobject.h \
    cgal.h \
    polyhedronloader.h \
    io.h \
    pixel.h \
    colourhistogram.h \
    application.h \
    matrixd.h \
    drawhelpers.h \
    ialphagenerator.h \
    polyhedronalphagenerator.h \
    boundingpolyhedron.h \
    imageref.h \
    icoloursegmenter.h \
    ifittingalgorithm.h \
    polyhedronfittingalgorithms.h


