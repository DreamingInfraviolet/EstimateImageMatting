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
    util.cpp \
    coloursegmenters.cpp \
    inputassembler.cpp \
    algorithmprimatte.cpp \
    alphalocator.cpp \
    inputprocessing.cpp \
    fittingalgorithms.cpp \
    spherepolyhedron.cpp

HEADERS  += \
    cgal.h \
    polyhedronloader.h \
    io.h \
    application.h \
    matrixd.h \
    drawhelpers.h \
    boundingpolyhedron.h \
    icoloursegmenter.h \
    ifittingalgorithm.h \
    util.h \
    coloursegmenters.h \
    inputassembler.h \
    IAlgorithm.h \
    algorithmprimatte.h \
    ialphalocator.h \
    alphalocator.h \
    inputprocessing.h \
    fittingalgorithms.h \
    geomhelper.h \
    spherepolyhedron.h


