//
// Created by mateu on 5/11/2021.
//

#include "ScriptHandler.h"


ScriptHandler::ScriptHandler(const std::string &path) {
    lua_state_ = luaL_newstate();
    luaL_openlibs(lua_state_);
}

ScriptHandler::Result ScriptHandler::run_script() {
    if (luaL_dofile(lua_state_, "../test.lua") == LUA_OK) {
        printf("Lua ok\n");
        lua_pop(lua_state_, lua_gettop(lua_state_));
    } else {
        error_msg_ = lua_tostring(lua_state_, -1);
        return Result::Error;
    }
}

void ScriptHandler::add_custom_libs() {
    auto L = lua_state_;

    const struct luaL_Reg LowLevel[] = {
            {"set_pixel",  NULL},
            {"get_pixel",  NULL},
            {"get_size_x", NULL},
            {"get_size_y", NULL},
    };

    lua_newtable(L);
    luaL_setfuncs(L, LowLevel, 0);
    lua_setglobal(L, "LowLevel");


    const struct luaL_Reg Functional[] = {
            {"apply_grayscale", NULL},
    };

    lua_newtable(L);
    luaL_setfuncs(L, Functional, 0);
    lua_setglobal(L, "Functional");

    const struct luaL_Reg Control[] = {
            {"add", add},
    };

    lua_newtable(L);
    luaL_setfuncs(L, Control, 0);
    lua_setglobal(L, "Control");
}

ScriptHandler::~ScriptHandler() {
    lua_close(lua_state_);
}
