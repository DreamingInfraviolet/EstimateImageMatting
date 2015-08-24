#pragma once
#include <QGLViewer/qglviewer.h>
#include <memory>
#include "inputassembler.h"
#include <qtimer.h>
#include "coloursegmenters.h"
#include <fittingalgorithms.h>
#include "alphalocator.h"
namespace anima { namespace alg { class IAlgorithm; } }

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

    //Handles the frame timing
    QBasicTimer mBasicTimer;


    /** Called on every timer tick: updates the frame. */
    void timerEvent(QTimerEvent *event);

    anima::alg::primatte::Test2Fitting fitter;
    anima::alg::primatte::DistanceColourSegmenter segmenter;
    anima::alg::primatte::AlphaRayLocator alphaLocator;
};
