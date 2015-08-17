#include "application.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Instantiate the viewer.
    Application viewer;

    viewer.setWindowTitle("Preview");

    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
