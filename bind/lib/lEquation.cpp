#include "stdafx.h"
#include "lEquation.h"


int static cEquation_getValue(lua_State* L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, "key1");
	return 1;
}

int static cEquation_getreg(lua_State* L)
{
	return 0;
}

static const luaL_Reg cReg_Equation[] =
{
	{"getvalue", cEquation_getValue},
	{"getreg", cEquation_getreg},

	{NULL, NULL},
};

int luaopen_Equation(lua_State* L)
{
	luaL_newlib(L, cReg_Equation);
	return 1;
}