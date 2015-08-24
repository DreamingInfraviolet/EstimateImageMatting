#include "application.h"
#include <vector>
#include <cassert>
#include <opencv2/opencv.hpp>
#include <fittingalgorithms.h>
#include "io.h"
#include "coloursegmenters.h"
#include "boundingpolyhedron.h"
#include "cgal.h"
#include "alphalocator.h"
#include "algorithmprimatte.h"
#include "spherepolyhedron.h"

Application::Application() :
    mInputAssembler(nullptr),
    mAlgorithm(nullptr) {}

Application::~Application()
{
    delete mAlgorithm;
    delete mInputAssembler;
}

void Application::timerEvent(QTimerEvent *event)
{
    update();
}


void Application::init()
{
    try
    {
        using namespace anima::ia;

        Inform("Running");

        restoreStateFromFile();

        Inform("Processing input");

        cv::Mat imageMat = InputAssembler::loadRgbMatFromFile("test.bmp");

        anima::alg::Point background = anima::alg::Point(12/255.f,246/255.f,0/255.f);

        InputAssemblerDescriptor iaDesc;
        iaDesc.source = &imageMat;
        iaDesc.ipd.order = InputProcessingDescriptor::ERandomOutliersGrid;
        iaDesc.targetColourspace = InputAssemblerDescriptor::ETCS_HSV;
        iaDesc.ipd.removeOutliers = false;
        iaDesc.ipd.removeOutliersK = 24;
        iaDesc.ipd.gridSimplify = false;
        iaDesc.ipd.gridSimplifyEpsilon = 0.04;
        iaDesc.ipd.randomSimplify = true;
        iaDesc.ipd.randomSimplifyPercentage = 50.0;
        iaDesc.ipd.gridSize = 100;
        iaDesc.backgroundPoint = background;

        mInputAssembler = new InputAssembler(iaDesc);

        Inform("Creating primatte algorithm");
        anima::alg::primatte::NoFitting fitter;
        anima::alg::primatte::DistanceColourSegmenter segmenter;
        anima::alg::primatte::AlphaRayLocator alphaLocator;

        anima::alg::primatte::AlgorithmPrimatteDesc algDesc;
        algDesc.boundingPolyhedronDesc.fitter = &fitter;
        algDesc.boundingPolyhedronDesc.meshPath = "objects/sphere.obj";
        algDesc.boundingPolyhedronDesc.scaleMultiplier = 1.1f;
        algDesc.boundingPolyhedronDesc.centre = mInputAssembler->background();
        algDesc.segmenter = &segmenter;
        algDesc.alphaLocator = &alphaLocator;

        mAlgorithm = new anima::alg::primatte::AlgorithmPrimatte(algDesc);

        Inform("Analysing input");
        mAlgorithm->setInput(mInputAssembler);
        mAlgorithm->analyse();

        Inform("Applying results");
        auto result = mAlgorithm->computeAlphas();

//        cv::namedWindow( "Alpha", cv::WINDOW_AUTOSIZE );// Create a window for display.
//        cv::imshow( "Alpha", result );

//        cv::Mat af = imageMat;

//        for(int r = 0; r < af.rows; ++r)
//            for(int c = 0; c < af.cols; ++c)
//            {
//                cv::Vec3b& v = af.at<cv::Vec3b>(r,c);
//                v *= result.at<float>(r,c);
//            }

//        cv::namedWindow( "AF", cv::WINDOW_AUTOSIZE );// Create a window for display.
//        cv::imshow( "AF", af );

//        //Prepare drawing
        glClearColor(0.9,0.9,0.9,1);
        glDisable(GL_LIGHTING);

        //Set fps (update every n milliseconds)
            mBasicTimer.start(30, this);
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

void glColor3fSRGB(float x, float y, float z)
{
    glColor3f(pow(x,2.2f),pow(y,2.2f),pow(z,2.2f));
}

void Application::draw()
{
  drawBackground();

  //Draw points
  glPointSize(5.0);
  glLineWidth(2);
  glBegin(GL_POINTS);
  for(auto it = mInputAssembler->points().begin(); it!=mInputAssembler->points().end(); ++it)
  {
      auto c = mInputAssembler->debugGetPointColour(*it);
      glColor3fSRGB(c.x,c.y,c.z);
      glVertex3f(it->x(), it->y(), it->z());
  }
  glEnd();

  //Draw background point
  if(mInputAssembler)
  {
      glColor3fSRGB(1,0,0);
      glPointSize(10.0);
      glBegin(GL_POINTS);
      auto b = mInputAssembler->background();
      glVertex3f(b.x(),b.y(),b.z());
      glEnd();
  }

  //Draw algorithm
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  if(mAlgorithm)
      mAlgorithm->debugDraw();
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  //Draw debug sphere
  glPointSize(5.0);

  SpherePolyhedron sp(16,8,math::vec3(0,1,0),0.8);

  static float phi=0,theta=-2;
  phi = phi + 0.002f;
  theta = theta + 0.003f;

  sp.findDistanceToPolyhedron(
              SpherePolyhedron::sphericalToCartesian(math::vec2(theta,phi)));
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
    glColor3fSRGB(c1r,c1g,c1b);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0,1.0);
    //c2
    glColor3fSRGB(c2r,c2g,c2b);
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
