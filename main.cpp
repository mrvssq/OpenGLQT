// main.cpp

#include <QApplication>
#include <QDesktopWidget>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;    
    window.show();
    //window.resize(window.sizeHint());


    return app.exec();
}
