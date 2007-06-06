//    <oraload - loads and gets lobs (Large Objects) into and out of an Oracle Database> 
//    Copyright (C) <2004-2007> <Ralf Duenkelmann>
//
//    This library is free software; you can redistribute it and/or modify it under the terms of the 
//    GNU Lesser General Public License as published by the Free Software Foundation; 
//    either version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful, 
//    but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License along with this library; 
//    if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include "Lua.hpp"


void Lua::init (void )
{
	/* initialize Lua */
	L = luaL_newstate();
	luaL_openlibs(L);

	/* load Lua base libraries */
    //luaopen_io(L); 
    //luaopen_base(L);
    //luaopen_table(L);
    //luaopen_string(L);
    //luaopen_math(L);
}


void Lua::do_file(const char* plua_file)
{
	//load the file
	int s = luaL_loadfile(L, plua_file);

    if ( s==0 ) {
      // execute Lua program
      s = lua_pcall(L, 0, LUA_MULTRET, 0);
    }
}

void Lua::close()
{
    /* cleanup Lua */
	lua_close(L);
}
