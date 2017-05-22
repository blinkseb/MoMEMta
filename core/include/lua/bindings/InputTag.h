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

#pragma once

struct lua_State;
struct InputTag;

/**
 * \file
 * \brief Lua binding of C++ InputTag class
 */

#define LUA_INPUTTAG_TYPE_NAME "InputTag"

namespace lua {

/**
 * \brief Register InputTag into lua runtime
 */
void inputtag_register(lua_State* L);

/**
 * \brief Create a new instance of InputTag
 **/
int inputtag_new(lua_State* L);

/**
 * \brief Free an instance of InputTag
 */
int inputtag_free(lua_State* L);

/**
 * \brief Retrieve an instance of InputTag from the lua stack
 */
InputTag* inputtag_get(lua_State* L, int index);

}
