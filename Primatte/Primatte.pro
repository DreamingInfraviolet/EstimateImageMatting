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
    boundingpolyhedron.cpp \
    pixel.cpp \
    util.cpp \
    coloursegmenters.cpp \
    algorithmfactory.cpp \
    inputassembler.cpp \
    algorithmprimatte.cpp \
    outputassembler.cpp \
    alphalocator.cpp \
    inputprocessing.cpp

HEADERS  += \
    cgal.h \
    polyhedronloader.h \
    io.h \
    pixel.h \
    application.h \
    matrixd.h \
    drawhelpers.h \
    boundingpolyhedron.h \
    icoloursegmenter.h \
    fittingalgorithmspolyhedron.h \
    ifittingalgorithm.h \
    util.h \
    coloursegmenters.h \
    algorithmfactory.h \
    inputassembler.h \
    IAlgorithm.h \
    algorithmprimatte.h \
    outputassembler.h \
    ialphalocator.h \
    alphalocator.h \
    inputprocessing.h


