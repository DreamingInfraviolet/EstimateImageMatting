#pragma once
#include <QGLViewer/qglviewer.h>
#include <qtimer.h>
#include "inputassembler.h"
#include "coloursegmenters.h"
#include "fittingalgorithms.h"
#include "alphalocator.h"
#include "averagebackgroundcolourlocators.h"

namespace anima { namespace alg { class IAlgorithm; } }

/** The main application class. It is to be replaced in the future by a proper
  * driver. Currently it reads the input from a file called "test.bmp" and
  * supplies it to the algorithm, which in itself is a general system.
  */
class Application: public QGLViewer
{
public:
    /** Initialises variables to null. */
    Application();

    /** Deletes internal pointers. */
    ~Application();

    /** Draws a preview of the scene. */
    virtual void draw();

    /** Draws the background plane. Used by draw(). */
    void drawBackground();

    /** Gathers input and runs the algorithm. */
    virtual void init();

    /** Used by qglviewer to provide a help string during preview. */
    virtual QString helpString() const;

    /* The input data structure. */
    anima::ia::InputAssembler* mInputAssembler;

    /* The currently used algorithm (only primatte available). */
    anima::alg::IAlgorithm* mAlgorithm;

    /* Handles debug preview fps. */
    QBasicTimer mBasicTimer;

    /** Called on every timer tick: updates the frame. */
    void timerEvent(QTimerEvent*);

    /* The fitting algorithm to use. */
    anima::alg::primatte::StableFitting fitter;

    /* The segmenter algorithm to use. */
    anima::alg::primatte::DistanceColourSegmenter segmenter;

    /* The alpha interpolation algorithm to use. */
    anima::alg::primatte::AlphaRayLocator alphaLocator;

    anima::ia::ABCL_BarycentreBased backgroundLocator;
};
