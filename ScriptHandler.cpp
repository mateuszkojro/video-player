//
// Created by mateu on 5/11/2021.
//

#include <iostream>
#include "ScriptHandler.h"


lua_State *ScriptHandler::lua_state_ = ScriptHandler::init();
const char *ScriptHandler::CURRENT_FRAME = "__current_frame";
std::string ScriptHandler::error_msg_;

ScriptHandler::Result ScriptHandler::run_script(const std::string &path) {
    if (luaL_dofile(lua_state_, path.c_str()) == LUA_OK) {
        lua_pop(lua_state_, lua_gettop(lua_state_));
        return Result::Ok;
    }
    error_msg_ = lua_tostring(lua_state_, -1);

    return Result::Error;
}

void ScriptHandler::add_custom_libs() {

    const struct luaL_Reg Control[] = {
            {"applyGreyscale",    applyGreyscale},
            {"applyHSV",          applyHSV},
            {"addText",           addText},
            {"setPixelGrayscale", setPixelGrayscale},
            {"setPixelBGR",       setPixelBGR},
            {"getSizeX",          getSizeX},
            {"getSizeY",          getSizeY},
            {"getPixel",          getPixel},
            {"setPixel",          setPixel}
    };

    lua_newtable(lua_state_);
    luaL_setfuncs(lua_state_, Control, 0);
    lua_setglobal(lua_state_, "CurrentFrame");

}

ScriptHandler::~ScriptHandler() {
    lua_close(lua_state_);
}

std::string ScriptHandler::get_last_error() {
    return error_msg_;
}

void ScriptHandler::update_current_frame(cv::Mat &frame) {
    auto &L = ScriptHandler::lua_state_;
    lua_pushlightuserdata(L, reinterpret_cast<void *>(&frame));
    lua_setglobal(L, CURRENT_FRAME);
}

cv::Mat ScriptHandler::get_current_frame() {
    auto &L = ScriptHandler::lua_state_;
    void *ud;
    lua_getglobal(L, CURRENT_FRAME);
    if (lua_isuserdata(L, -1)) {
        ud = lua_touserdata(L, -1);
        return *reinterpret_cast<cv::Mat *>(ud);
    }
    lua_pop(L, 1);
    std::cout << "It was not user data" << std::endl;
    throw "Could not get user data";
}
