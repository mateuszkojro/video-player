#include <QApplication>

#include "VideoPlayer.h"
#include "lua.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VideoPlayer window;
    window.show();
    return app.exec();
}
