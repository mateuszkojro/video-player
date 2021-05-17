//
// Created by mateu on 5/11/2021.
//

#include "ScriptHandler.h"


ScriptHandler::ScriptHandler() {
    lua_state_ = luaL_newstate();
    luaL_openlibs(lua_state_);
}

ScriptHandler::Result ScriptHandler::run_script(const std::string &path) {
    if (luaL_dofile(lua_state_, path.c_str()) == LUA_OK) {
        lua_pop(lua_state_, lua_gettop(lua_state_));
        return Result::Ok;
    }
    error_msg_ = lua_tostring(lua_state_, -1);

    return Result::Error;
}

void ScriptHandler::add_custom_libs() {

//    const struct luaL_Reg LowLevel[] = {
//            {"set_pixel",  NULL},
//            {"get_pixel",  NULL},
//            {"get_size_x", NULL},
//            {"get_size_y", NULL},
//    };


//    lua_newtable(lua_state_);
//    luaL_setfuncs(lua_state_, LowLevel, 0);
//    lua_setglobal(lua_state_, "LowLevel");


//    const struct luaL_Reg Functional[] = {
//            {"apply_grayscale", NULL},
//    };

//    lua_newtable(lua_state_);
//    luaL_setfuncs(lua_state_, Functional, 0);
//    lua_setglobal(lua_state_, "Functional");

    const struct luaL_Reg Control[] = {
            {"add",       add},
            {"user_data", user_data}
    };

    lua_newtable(lua_state_);
    luaL_setfuncs(lua_state_, Control, 0);
    lua_setglobal(lua_state_, "Control");

}

ScriptHandler::~ScriptHandler() {
    lua_close(lua_state_);

}

//void ScriptHandler::add_video_context() {
//    struct Pair {
//        int a;
//        int b;
//    };
//
//    Pair thing{.a =12, .b =15};
//    void *ud = lua_newuserdata(lua_state_, sizeof(Pair));
//    *reinterpret_cast<Pair *>(ud) = thing;
//    lua_setglobal(lua_state_, "__glwidget");
//
//}

std::string ScriptHandler::get_last_error() {
    return error_msg_;
}
