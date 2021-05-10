#include <QApplication>

#include "VideoPlayer.h"
#include "lua.hpp"


int main(int argc, char *argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    QApplication app(argc, argv);
    VideoPlayer window;
    window.show();
    return app.exec();
}
