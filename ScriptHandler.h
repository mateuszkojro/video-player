//
// Created by mateu on 5/11/2021.
//

#ifndef VIDEO_PLAYER_SCRIPTHANDLER_H
#define VIDEO_PLAYER_SCRIPTHANDLER_H

#include <string>
#include <lua.hpp>




class ScriptHandler {
public:

    enum Result{
        Ok,
        Error,
    };


    explicit ScriptHandler(const std::string&);

    ScriptHandler::Result run_script();

    void add_custom_libs();

    ~ScriptHandler();

private:

    static int add(lua_State* L){
        auto x = lua_tointeger(L, -1);
        auto y = lua_tointeger(L, -2);

        printf("a = %lld , b = %lld\n", x, y);
        lua_pushinteger(L,x+y);
        return 1;
    }

    std::string error_msg_;
    lua_State *lua_state_;

};


#endif //VIDEO_PLAYER_SCRIPTHANDLER_H
