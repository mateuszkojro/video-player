#include <QApplication>

#include "VideoPlayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VideoPlayer window;
    window.show();
    return app.exec();
}
