//
// Created by mateu on 5/11/2021.
//

#ifndef VIDEO_PLAYER_SCRIPTHANDLER_H
#define VIDEO_PLAYER_SCRIPTHANDLER_H

#include <string>
#include <lua.hpp>


class ScriptHandler {
public:

    enum Result {
        Ok,
        Error,
    };

    explicit ScriptHandler();

    Result run_script(const std::string &path);

    void add_custom_libs();

    template<class T>
    void add_context(T *obj, const char *uid) {
        void *ud = lua_newuserdata(lua_state_, sizeof(obj));
        ud = (void *) obj;
        lua_setglobal(lua_state_, uid);
    }

    std::string get_last_error();

    ~ScriptHandler();

private:

    static int add(lua_State *L) {
        auto x = lua_tointeger(L, -1);
        auto y = lua_tointeger(L, -2);

        printf("a = %lld , b = %lld\n", x, y);
        lua_pushinteger(L, x + y);
        return 1;
    }

    static int user_data(lua_State *L) {
        struct Pair {
            int a;
            int b;
        };

        lua_getglobal(L, "__glwidget");
        Pair* user_data = *reinterpret_cast<Pair **>( lua_touserdata(L, 1));

        printf("User data: a=%d, b=%d\n", user_data->a, user_data->b);
        return 1;
    }

    std::string error_msg_;
    lua_State *lua_state_;
};


#endif //VIDEO_PLAYER_SCRIPTHANDLER_H
