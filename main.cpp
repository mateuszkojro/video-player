#define WITH_LUA false

#include <QApplication>
#include "VideoPlayer.h"
#include "lua.hpp"

#include "ScriptHandler.h"
#include <iostream>

struct Pair {
    int a;
    int b;
};

int main(int argc, char *argv[]) {

#if WITH_LUA

    ScriptHandler sh;
    sh.add_custom_libs();
    Pair *pair = new Pair;

    pair->a = 12;
    pair->b = 110;

    sh.add_context(pair, "__gl_widget");

    if (sh.run_script("./test.lua") == ScriptHandler::Error) {
        std::cout << "There was an error" << std::endl;
        std::cout << sh.get_last_error() << std::endl;
    }

#else

    QApplication::setStyle("fusion");
    QApplication app(argc, argv);
    VideoPlayer window;
    window.show();
    return app.exec();


#endif

}
