#include "application.h"
#include <vector>
#include <cassert>
#include <opencv2/opencv.hpp>
#include <fittingalgorithmspolyhedron.h>
#include "io.h"
#include "coloursegmenters.h"
#include "boundingpolyhedron.h"
#include "cgal.h"
#include "alphalocator.h"

Application::Application() :
    mInputAssembler(nullptr),
    mAlgorithm(nullptr) {}

Application::~Application()
{
    delete mAlgorithm;
    delete mInputAssembler;
}

void Application::init()
{
    try
    {
        using namespace anima::ia;
        using namespace anima::af;
        using namespace anima::oa;

        Inform("Running");
        restoreStateFromFile();

        Inform("Processing input");

        cv::Mat imageMat = InputAssembler::loadRgbMatFromFile("test.bmp");

        InputAssemblerDescriptor iaDesc;
        iaDesc.inputSource = InputAssemblerDescriptor::EMEMORY;
        iaDesc.inputSourceMemory.data = imageMat.data;
        iaDesc.inputSourceMemory.dataSize = imageMat.rows*imageMat.cols*imageMat.channels();
        iaDesc.inputSourceMemory.step = imageMat.channels();
        iaDesc.inputSourceMemory.type = InputAssemblerDescriptor::EPIXEL_RGB8;
        iaDesc.ipd.comparisonBitsToIgnore = 2;
        iaDesc.ipd.order = InputProcessingDescriptor::ERandomOutliersGrid;
        iaDesc.ipd.removeOutliers = false;
        iaDesc.ipd.removeOutliersK = 3;
        iaDesc.ipd.gridSimplify = false;
        iaDesc.ipd.gridSimplifyEpsilon = 0.04;
        iaDesc.ipd.randomSimplify = true;
        iaDesc.ipd.randomSimplifyPercentage = 80.0;

        mInputAssembler = new InputAssembler(iaDesc);

        Inform("Creating primatte algorithm");
        anima::alg::primatte::TestVertexFitting fitter;
        anima::alg::primatte::DistanceColourSegmenter segmenter;
        anima::alg::primatte::AlphaDistanceLocator alphaLocator;

        AlgorithmFactoryDescriptor afDesc;
        afDesc.type = AlgorithmFactoryDescriptor::EPrimatte;


        afDesc.algPrimatteDesc.boundingPolyhedronDesc.fitter = &fitter;
        afDesc.algPrimatteDesc.boundingPolyhedronDesc.meshPath = "objects/sphere.obj";
        afDesc.algPrimatteDesc.boundingPolyhedronDesc.scaleMultiplier = 1.1f;
        afDesc.algPrimatteDesc.segmenter = &segmenter;
        afDesc.algPrimatteDesc.backgroundPoint = anima::alg::Point(0,0,1);
        afDesc.algPrimatteDesc.input = mInputAssembler;
        afDesc.algPrimatteDesc.alphaLocator = &alphaLocator;

        AlgorithmFactory af;
        mAlgorithm = af.produce(afDesc);
        if(!mAlgorithm)
            throw std::runtime_error("Could not create algorithm");

        Inform("Gathering results");
        //Set not to save result for now as it's not implemented.
        OutputAssemblerDescriptor oaDesc;
        oaDesc.algorithm = mAlgorithm;
        oaDesc.imageSource = &imageMat;
        oaDesc.generateFile = false;
        oaDesc.showPreview = false;
        oaDesc.memoryOptions.destinationImage = nullptr;
        oaDesc.memoryOptions.destinationRaw = nullptr;
        oaDesc.memoryOptions.destinationRawSize = 0;

        OutputAssembler oa;
        oa.assemble(oaDesc);

        //Prepare drawing
        glClearColor(0.9,0.9,0.9,1);
        glDisable(GL_LIGHTING);
    }
    catch(std::runtime_error err)
    {
        Error(err.what());
        this->close();
    }
    catch(...)
    {
        Error("Something happened");
        this->close();
    }
}

void Application::draw()
{
  drawBackground();

  //Draw points
  glPointSize(5.0);
  glLineWidth(3);
  glBegin(GL_POINTS);
  for(auto it = mInputAssembler->points().begin(); it!=mInputAssembler->points().end(); ++it)
  {
      glColor3f(it->x(), it->y(), it->z());
      glVertex3f(it->x(), it->y(), it->z());
  }
  glColor4f(0,0,0,1);
  glEnd();

  //Draw algorithm
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  mAlgorithm->debugDraw();
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Application::drawBackground()
{
    float c1r = 0.95, c1g = 0.95, c1b = 0.95;
    float c2r = 0.4, c2g = 0.6, c2b = 0.4;

    //Push
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //Draw
    glBegin(GL_QUADS);
    //c1
    glColor3f(c1r,c1g,c1b);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0,1.0);
    //c2
    glColor3f(c2r,c2g,c2b);
    glVertex2f(-1.0,-1.0);
    glVertex2f(1.0, -1.0);
    glEnd();

    //Pop
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glClear(GL_DEPTH_BUFFER_BIT);
}

QString Application::helpString() const
{
  QString text("<h2>S i m p l e V i e w e r</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the Application.";
  return text;
}
