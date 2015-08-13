#include "application.h"
#include <vector>
#include <cassert>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "polyhedronalphagenerator.h"
#include <polyhedronfittingalgorithms.h>
#include "io.h"

Application::Application() : mHistogram(mImageRef) {}

bool Application::initialiseImage()
{
    //Load image
    Inform("Loading image");
    cv::Mat cimagein = cv::imread("test.bmp",  CV_LOAD_IMAGE_COLOR);
    if(!cimagein.data)
    {
        Error("Could not load image");
        return false;
    }

    //Convert image to rgb
    cimagein.convertTo(mImage, CV_8UC3);

    mImageRef = ImageRef<PixelRGB8>(mImage.data, mImage.cols, mImage.rows,
                           mImage.channels(), 1, 1*mImage.channels());

    Inform("Loaded image with dimensions (" +
           ToString(mImageRef.width()) + ", " +
           ToString(mImageRef.height()) + ")");

    //Get histogram from image (hist is a reference)
    auto hist = mHistogram.histogram();

    //Prepare 3D floating point points
    mPoints.reserve(hist.size());
    for(auto it = hist.begin(); it != hist.end(); ++it)
        mPoints.push_back(Point(it->rf(), it->gf(), it->bf()));
    return true;
}

void Application::init()
{
  Inform("Running");

  //Restore previous Application state.
  restoreStateFromFile();

  if(!initialiseImage())
      return;

  TestVertexFitting fitter;

  PolyhedronAlphaGeneratorData generatorData;
  generatorData.boundingPolyhedronData.meshPath = "objects/sphere.obj";
  generatorData.boundingPolyhedronData.fitter = &fitter;
  generatorData.boundingPolyhedronData.scaleMultiplier = 1.1f;
  generatorData.points = &mPoints;

  mGenerator = IAlphaGenerator::create(&generatorData);
  if(!mGenerator
   ||!mGenerator->analyse())
      return;


  cv::Mat alpha = findAlpha();

  //Prepare drawing
  glClearColor(0.9,0.9,0.9,1);
  glDisable(GL_LIGHTING);
}

cv::Mat Application::findAlpha()
{
    std::vector<float> alphas = mGenerator->findAlphas(mPoints);
    if(alphas.size()==0)
        return cv::Mat();

    assert(mImageRef.height()*mImageRef.width() == alphas.size());

    cv::Mat alphaMat;
    alphaMat.create(mImageRef.height(), mImageRef.width(), CV_32FC1);
    memcpy(alphaMat.data, &alphas[0],sizeof(float)*alphas.size());
    return alphaMat;
}

void showMatImage(const cv::Mat& mat)
{
    using namespace cv;
    namedWindow( "Image", WINDOW_AUTOSIZE );
    imshow( "Image", mat );
}

// Draws the points and sphere
void Application::draw()
{

  drawBackground();

  //Draw points
  glPointSize(5.0);
  glLineWidth(3);
  glBegin(GL_POINTS);
  for(auto it = mPoints.begin(); it!=mPoints.end(); ++it)
  {
      glColor3f(it->x(), it->y(), it->z());
      glVertex3f(it->x(), it->y(), it->z());
  }
  glColor4f(0,0,0,1);

  glEnd();

  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  mGenerator->debugDraw();
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
