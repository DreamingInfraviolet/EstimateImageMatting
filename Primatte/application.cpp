#include <stdexcept>
#include <opencv2/opencv.hpp>
#include "application.h"
#include "matrixd.h"
#include "algorithmprimatte.h"
#include "fittingalgorithms.h"
#include "averagebackgroundcolourlocators.h"
#include "coloursegmenters.h"
#include "alphalocator.h"
#include "io.h"


Application::Application() :
    mInputAssembler(nullptr),
    mAlgorithm(nullptr),
    mFitter(nullptr),
    mSegmenter(nullptr),
    mAlphaLocator(nullptr),
    mBackgroundLocator(nullptr) {}

Application::~Application()
{
    delete mBackgroundLocator;
    delete mAlphaLocator;
    delete mSegmenter;
    delete mFitter;
    delete mAlgorithm;
    delete mInputAssembler;
}

void Application::timerEvent(QTimerEvent*)
{
    update();
}
cv::Mat imageMat;
void Application::init()
{
    try
    {
        using namespace anima::ia;
        using namespace anima::alg::primatte;


        //Restore QGLPreviewer state from last run, such as camera position, etc.
        restoreStateFromFile();

        //A scene radius of 1 has some clipping.
        this->setSceneRadius(1.5);

        Inform("Processing input");

        imageMat = cv::imread("test.jpg");
        cv::Mat backgroundMat = cv::imread("test_background.jpg");
        if(imageMat.data==nullptr || backgroundMat.data == nullptr)
            throw std::runtime_error("Could not load images");


        START_TIMER(WholeProgramTimer);

        ////////////////////////////////////////////////////////////////////////////
        ////////////////////////////// Input Stage /////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////

        /* Locates the dominant background colour from the input background image mat. */
        mBackgroundLocator = new anima::ia::ABCL_BarycentreBased();

        //This descriptor is used to initialise the input assembler.
        InputAssemblerDescriptor iaDesc;

        iaDesc.backgroundSource = &backgroundMat;

        iaDesc.backgroundLocator = mBackgroundLocator;

        //Set the image source to be copied from.
        //The source mat is converted into floating point values.
        //8-bit, 16-bit and floating point formats are supported.
        //Expects a 3-component image.
        iaDesc.foregroundSource = &imageMat;

        //The 3D grid segment count to use for cleaning up duplicate input
        //points. Lower values require less memory and filter more aggressively.
        iaDesc.ipd.gridSize = 400;

        //The target colour space to convert to.
        //Currently can either be RGB, HSV or LAB.
        //HSV is unsuitable for blue due to wrap-around!
        iaDesc.targetColourspace = InputAssemblerDescriptor::ETCS_RGB;

        //Remove % of points randomly after cleanup to speed up computation.
        iaDesc.ipd.randomSimplify = false;
        iaDesc.ipd.randomSimplifyPercentage = 30.0;

        //Create the assembler object, load, and process the input.
        //An exception will be thrown in case of an error, most likely
        //a std::runtime_error.
        mInputAssembler = new InputAssembler(iaDesc);

        ////////////////////////////////////////////////////////////////////////////
        //////////////////////////// Algorithm Stage ///////////////////////////////
        ////////////////////////////////////////////////////////////////////////////
        Inform("Creating primatte algorithm");

        //Choose your algorithms:

        /* The fitting algorithm to use. The input is the number of binary iterations
           to perform, akin to a binary search algorithm iteration.
           2 is a good number, as it's both accurate and doesn't fit TOO closely, which
           can lead to errors due to the sample points being simplified. */
        mFitter = new anima::alg::primatte::StableFitting(2);

        /* The segmenter algorithm to use. It splits the data points in two based on the parameters.
           The distance segmenter splits the input based on whether a point is inside/outside a sphere. */
        mSegmenter = new anima::alg::primatte::DistanceColourSegmenter();

        /* The alpha interpolation algorithm to use. It is used to compute the alpha for each pixel. */
        mAlphaLocator = new anima::alg::primatte::AlphaRayLocator();

        //Fill in the algorithm descriptor.
        AlgorithmPrimatteDesc algDesc;

        //The fitter algorithm to use.
        algDesc.boundingPolyhedronDesc.fitter = mFitter;

        //The number of phi and theta faces for the generated sphere.
        //Optimally, there is a ration of 2:1. 16*8 is a 128-faced sphere.
        algDesc.boundingPolyhedronDesc.phiFaces = 16;
        algDesc.boundingPolyhedronDesc.thetaFaces = 8;

        //The sphere is multiplied by this amount after creation to ensure no
        //accidental intersection with the points due to low mesh resolution.
        algDesc.boundingPolyhedronDesc.scaleMultiplier = 1.2f;

        //The segmenter splits the points into outer and inner points.
        algDesc.segmenter = mSegmenter;

        //The alpha locator is responsible for interpolating and generating the alpha.
        algDesc.alphaLocator = mAlphaLocator;

        //The parameter passed to the colour segmenter.
        //The inner background points returned are wrapped around.
        algDesc.innerShrinkingThreshold = 0.6f;

        //The distance of a shrunken inner vertex from the centre.
        algDesc.innerShrinkingMinDistance = 0.001f;

        //After shrinking the inner polyhedron is multiplied by this amount.
        algDesc.innerPostShrinkingMultiplier = 1.1f;

        //Passed to the segmenter when positioning outer sphere around inner points before expanding.
        //Essentially the start radius of the outer sphere.
        algDesc.outerExpansionStartThreshold = 0.15f;

        //The approximate amount the outer sphere should try to expand.
        algDesc.outerExpandDelta = 0.075f;

        //The amount the outer sphere should be scaled after expansion relative to
        //the inner sphere. It is made so that it never collides with the inner polyhedron,
        //such that a value of 0 = as close as possible to inner. A value of 1 = minimal scaling.
        //Note that it is relative to the final size of the inner polyhedron.
        algDesc.outerScaleParameter = 1.f;

        mAlgorithm = new AlgorithmPrimatte(algDesc);

        Inform("Analysing input");
        //Set the new input to be used.
        mAlgorithm->setInput(mInputAssembler);

        //Analyse the input. This is a performance-intensive operation.
        //In primatte this corresponds to polyhedron deformation.
        mAlgorithm->analyse();


        ////////////////////////////////////////////////////////////////////////////
        ///////////////////////// Get and preview result ///////////////////////////
        ////////////////////////////////////////////////////////////////////////////


        Inform("Applying results");
        //Apply the polyhedrae to the input image, and save the alpha to result.
        cv::Mat result = mAlgorithm->computeAlphas();

        END_TIMER(WholeProgramTimer);

        //Note: This is not part of the algorithm. It's just to preview the result.

        //Display the alpha image
        cv::namedWindow("Alpha", cv::WINDOW_AUTOSIZE);
        cv::imshow("Alpha", result);

        //Apply the alpha to the original image in order to preview it.
        cv::Mat af = imageMat.clone();

        //The background colour to blend with in BGR format.
        cv::Vec3f backgroundBlendColour(0,0,1);
        cv::Vec3f backgroundInImage(mInputAssembler->background().x,
                                    mInputAssembler->background().y,
                                    mInputAssembler->background().z);

        for(int r = 0; r < af.rows; ++r)
            for(int c = 0; c < af.cols; ++c)
            {
                cv::Vec3b& original = af.at<cv::Vec3b>(r,c);
                float alpha = result.at<float>(r,c);
                cv::Vec3f originalf = original;
                originalf *= 1/255.f;

                //get foreground colour
                cv::Vec3f foreground = (originalf - backgroundInImage*(1 - alpha))*(alpha);

                original = (foreground*alpha + backgroundBlendColour*(1.f-alpha))*255;
            }

        cv::namedWindow( "AF", cv::WINDOW_AUTOSIZE );
        cv::imshow( "AF", af );

        //This makes things difficult to see and interferes with the pixel colours.
        glDisable(GL_LIGHTING);

        //Set fps (update every n milliseconds). 16.66... ~ 60fps
        mBasicTimer.start(16.66666666, this);

    }
    catch(std::runtime_error err)
    {
        //If a known error has occured:
        Error(err.what());
        this->hide();
        this->close();
    }
    catch(...)
    {
        //If an unknown exception happened:
        Error("Something happened");
        this->hide();
        this->close();
    }
}

/** This function takes in a normalised colour in linear colourspace and
    calls glColour3f with its gamma-corrected version. */
void glColor3fSRGB(float x, float y, float z)
{
    glColor3f(pow(x,2.2f),pow(y,2.2f),pow(z,2.2f));
}

void Application::draw()
{
    if(this->isHidden())
        return;

  drawBackground();

  //Draw points
  glPointSize(5.0);
  glLineWidth(2);
  glBegin(GL_POINTS);

  //Background points
  for(auto it = mInputAssembler->backgroundPoints().begin(); it!=mInputAssembler->backgroundPoints().end(); ++it)
  {
      glColor3f(1,0,0);
      glVertex3f(it->x, it->y, it->z);
  }

  //Foreground points
  for(auto it = mInputAssembler->points().begin(); it!=mInputAssembler->points().end(); ++it)
  {
      auto c = mInputAssembler->debugGetPointColour(*it);

      //Swap components because the internal image is in BGR
      glColor3fSRGB(c.x,c.y,c.z);

      glVertex3f(it->x, it->y, it->z);
  }
  glEnd();

  //Draw background point
  if(mInputAssembler)
  {
      glColor3fSRGB(1,0,0);
      glPointSize(10.0);
      glBegin(GL_POINTS);
      auto b = mInputAssembler->background();
      glVertex3f(b.x,b.y,b.z);
      glEnd();
  }

  //Draw algorithm
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  if(mAlgorithm)
      mAlgorithm->debugDraw();
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  glPointSize(5.0);
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
