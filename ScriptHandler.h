//
// Created by mateu on 5/11/2021.
//

#ifndef VIDEO_PLAYER_SCRIPTHANDLER_H
#define VIDEO_PLAYER_SCRIPTHANDLER_H

#include <string>
#include <iostream>
#include <lua.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>


class ScriptHandler {
public:

    enum Result {
        Ok,
        Error,
    };

    explicit ScriptHandler() = default;

    static Result run_script(const std::string &path);

    static void add_custom_libs();

    static std::string get_last_error();

    static lua_State *init() {
        ScriptHandler::lua_state_ = luaL_newstate();
        luaL_openlibs(ScriptHandler::lua_state_);
        add_custom_libs();
        return lua_state_;
    }

    static void update_current_frame(cv::Mat &frame);

    static cv::Mat get_current_frame();

    ~ScriptHandler();

private:

    static const char *CURRENT_FRAME;

    static cv::Mat *extract_frame(lua_State *L, const std::string &id) {
        void *mat_ptr;
        cv::Mat *mat;

        lua_getglobal(L, CURRENT_FRAME);

        if (!lua_isuserdata(L, -1)) {
            std::clog << "Expected userdata" << std::endl;
            luaL_error(L, "Expected userdata");
            throw "It is not userdata";
        }

        mat_ptr = lua_touserdata(L, -1);

        mat = reinterpret_cast<cv::Mat *>(mat_ptr);

        if (!mat) {
            std::clog << "cv::Mat is null" << std::endl;
            luaL_error(L, "cv::Mat is null");
            throw "cv::Mat is null";
        }

        return mat;
    }

    static int applyGreyscale(lua_State *L) {
        auto mat = extract_frame(L, CURRENT_FRAME);

        cv::cvtColor(*mat, *mat, CV_RGB2GRAY);

        return 1;
    }

    static int setPixelGrayscale(lua_State *L) {
        auto color = luaL_checkinteger(L, 3);
        auto y = luaL_checkinteger(L, 2);
        auto x = luaL_checkinteger(L, 1);

        auto mat = extract_frame(L, CURRENT_FRAME);

        mat->at<uint8_t>(x, y) = color;

        return 1;
    }

    static int setPixelBGR(lua_State *L) {
        auto r = luaL_checkinteger(L, 5);
        auto g = luaL_checkinteger(L, 4);
        auto b = luaL_checkinteger(L, 3);
        auto y = luaL_checkinteger(L, 2);
        auto x = luaL_checkinteger(L, 1);

        auto mat = extract_frame(L, CURRENT_FRAME);

        mat->at<cv::Vec3b>(x, y) = cv::Vec3b(b, g, r);

        return 1;
    }

    static int getSizeX(lua_State *L) {
        auto mat = extract_frame(L, CURRENT_FRAME);

        unsigned size_x = mat->rows;

        lua_pushinteger(L, size_x);
        return 1;
    }

    static int getSizeY(lua_State *L) {
        auto mat = extract_frame(L, CURRENT_FRAME);

        unsigned size_y = mat->cols;

        lua_pushinteger(L, size_y);
        return 1;
    }

    static int applyHSV(lua_State *L) {

        auto mat = extract_frame(L, CURRENT_FRAME);

        cv::cvtColor(*mat, *mat, CV_BGR2HSV);

        return 1;
    }

    static int addText(lua_State *L) {

        auto thickness = luaL_checknumber(L, 8);
        auto r = luaL_checkinteger(L, 7);
        auto g = luaL_checkinteger(L, 6);
        auto b = luaL_checkinteger(L, 5);
        auto scale = luaL_checknumber(L, 4);
        auto y = luaL_checkinteger(L, 3);
        auto x = luaL_checkinteger(L, 2);
        auto string = luaL_checkstring(L, 1);

        lua_pop(L, 6);

        auto mat = extract_frame(L, CURRENT_FRAME);

        cv::putText(*mat,
                    string,
                    cv::Point(x, y),
                    CV_FONT_HERSHEY_SIMPLEX,
                    scale,
                //todo how is the color set?
                    cv::Scalar(b, g, r),
                    thickness
        );

        return 1;
    }

    static int getPixel(lua_State *L) {
        auto mat = extract_frame(L, CURRENT_FRAME);
        auto y = luaL_checkinteger(L, 2);
        auto x = luaL_checkinteger(L, 1);

        auto get_pix = mat->at<uint8_t >(x,y);

        lua_pushinteger(L, get_pix);
        return 1;
    }

    static std::string error_msg_;
    static lua_State *lua_state_;
};


#endif //VIDEO_PLAYER_SCRIPTHANDLER_H
