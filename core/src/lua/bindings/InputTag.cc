/*
 *  MoMEMta: a modular implementation of the Matrix Element Method
 *  Copyright (C) 2017  Universite catholique de Louvain (UCL), Belgium
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <lua/bindings/InputTag.h>

#include <momemta/InputTag.h>

#include <lua/bindings/Types.h>

#include <lua.hpp>

namespace lua {

int inputtag_tostring(lua_State* L) {
    InputTag* inputTag = inputtag_get(L, -1);

    lua_pushstring(L, inputTag->toString().c_str());
    return 1;
}

int inputtag_getmodule(lua_State* L) {
    InputTag* inputTag = inputtag_get(L, -1);

    lua_pushstring(L, inputTag->module.c_str());
    return 1;
}

int inputtag_getparam(lua_State* L) {
    InputTag* inputTag = inputtag_get(L, -1);

    lua_pushstring(L, inputTag->parameter.c_str());
    return 1;
}

int inputtag_getindex(lua_State* L) {
    InputTag* inputTag = inputtag_get(L, -1);

    if (! inputTag->isIndexed()) {
        luaL_error(L, "non indexed InputTag");
        return 0;
    }

    lua_pushinteger(L, inputTag->index + 1);
    return 1;
}

int inputtag_isindexed(lua_State* L) {
    InputTag* inputTag = inputtag_get(L, -1);

    lua_pushboolean(L, inputTag->isIndexed());
    return 1;
}

int luaopen_InputTag(lua_State* L) {

    // Create `InputTag` table acting as our InputTag lib
    static const luaL_Reg InputTag_methods[] = {
            { "new", inputtag_new },
            { NULL, NULL }
    };

    luaL_newlib(L, InputTag_methods);

    // Create metatable for type InputTag with all necessary functions
    push_type_metatable(L, LUA_INPUTTAG_TYPE_NAME);

    // Register methods
    static const luaL_Reg functions[] = {
            // Destructor
            {"__gc", inputtag_free},
            // String representation
            {"__tostring", inputtag_tostring},

            // Public methods
            {"module", inputtag_getmodule},
            {"parameter", inputtag_getparam},
            {"index", inputtag_getindex},
            {"isindexed", inputtag_isindexed},
            {nullptr, nullptr}
    };
    luaL_setfuncs(L, functions, 0);

    // Register the `__index` method of the metatable to points to itself
    // First copy the metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // Pop metatable
    lua_pop(L, 1);

    // Return InputTag table
    return 1;
}

}

void lua::inputtag_register(lua_State* L) {
    luaL_requiref(L, LUA_INPUTTAG_TYPE_NAME, luaopen_InputTag, 1);
}

int lua::inputtag_new(lua_State* L) {

    int n = lua_gettop(L);
    if (n < 2) {
        luaL_error(L, "invalid number of arguments: at least 2 expected, got %d", n);
    }

    if (n > 3) {
        luaL_error(L, "invalid number of arguments: at more 3 expected, got %d", n);
    }

    // Grab arguments
    std::string module_name = luaL_checkstring(L, 1);
    std::string module_output = luaL_checkstring(L, 2);

    bool indexed = false;
    LUA_INTEGER index = 0;
    if (n == 3) {
        indexed = true;
        auto local_index = luaL_checkinteger(L, 3);
        if (local_index <= 0)
            luaL_error(L, "invalid index %d: must be greater than 0", local_index);

        index = local_index - 1;
    }

    InputTag** pInputTag = static_cast<InputTag**>(lua_newuserdata(L, sizeof(InputTag*)));
    if (indexed)
        *pInputTag = new InputTag(module_name, module_output, (size_t) index);
    else
        *pInputTag = new InputTag(module_name, module_output);

    // Associate InputTag metatable to the new userdata
    luaL_getmetatable(L, LUA_INPUTTAG_TYPE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

int lua::inputtag_free(lua_State* L) {
    delete *static_cast<InputTag**>(luaL_checkudata(L, 1, LUA_INPUTTAG_TYPE_NAME));

    return 0;
}

InputTag* lua::inputtag_get(lua_State* L, int index) {
    luaL_checktype(L, index, LUA_TUSERDATA);
    InputTag** inputTag = static_cast<InputTag**>(luaL_checkudata(L, index, LUA_INPUTTAG_TYPE_NAME));
    if (!inputTag) {
        const char *msg = lua_pushfstring(L, "%s expected, got %s",
                LUA_INPUTTAG_TYPE_NAME, luaL_typename(L, index));
        luaL_argerror(L, index, msg);
    }

    return *inputTag;
}
