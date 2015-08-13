#include "application.h"
#include <qapplication.h>

class Viewer : public QGLViewer
{
protected :
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;
};


//template<class IT> void debugPrintIt(IT begin, IT end, int columns = 4)
//{
//    int column = 0;

//    for(auto it = begin; it != end; ++it)
//    {
//        if(++column % columns == 0)
//            std::cout<<"\n";
//        std::cout<<*it<<"  ";
//    }
//}

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Instantiate the viewer.
    Application viewer;

    viewer.setWindowTitle("simpleViewer");

    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
