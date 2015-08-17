#pragma once
#include <QGLViewer/qglviewer.h>
#include <memory>
#include "inputassembler.h"
#include "algorithmfactory.h"
#include "outputassembler.h"

/** The main application class. It is to be replaced in the future by a proper
  * driver. */
class Application: public QGLViewer
{
public:
    /** Initialises variables to null. */
    Application();

    /** Deletes internal pointers. */
    ~Application();

    /** Draws a preview of the scene. */
    virtual void draw();

    /** Draws the background plane. */
    void drawBackground();

    /** Initialises the system. */
    virtual void init();

    /** Used by qglviewer to provide a help string during preview. */
    virtual QString helpString() const;

    /* The input. */
    anima::ia::InputAssembler* mInputAssembler;

    /* The currently used algorithm. */
    anima::alg::IAlgorithm* mAlgorithm;
};
