
#include "stdafx.h"
#include "lclcalib.h"
#include <stdlib.h>
#include <math.h>


#undef PI
#define PI	((lua_Number)(3.1415926535897932384626433832795))
#define RADIANS_PER_DEGREE	((lua_Number)(PI/180.0))


static int math_abs (lua_State *L) {
	lua_pushnumber(L, l_mathop(fabs)(luaL_checknumber(L, 1)));
	return 1;
}

static int math_add (lua_State *L) {
	lua_Number x = luaL_checknumber(L, 1);
	lua_Number y = luaL_checknumber(L, 2);
	lua_pushnumber(L, l_mathop(x + y));
	return 1;
}
static int string_len(lua_State* L)
{
	size_t l;
	const char *s = luaL_checklstring(L, 1, &l);
	if (NULL == s)
		lua_pushnumber(L, l_mathop(l));
	else
		lua_pushnumber(L, l_mathop(strlen(s)));
	return 1;
}

static int math_muladd(lua_State* L)
{
	lua_Number x = luaL_checknumber(L, 1);
	lua_Number y = luaL_checknumber(L, 2);
	x++;
	y++;
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}

static int math_setNum(lua_State* L)
{
	lua_Number x = luaL_checknumber(L, 1);
	//lua_Number y = luaL_checknumber(L, 2);
	lua_pushnumber(L, x);
	//lua_pushnumber(L, y);
	lua_setfield(L, LUA_REGISTRYINDEX, "key1");
	return 0;
}

static const luaL_Reg mathlib[] = {
	{"abs",  math_abs},
	{"add",  math_add},
	{"muladd",  math_muladd},
	{"len",  string_len},
	{"setvalue",  math_setNum},

	{NULL, NULL}
};


int luaopen_clca (lua_State *L) {
	luaL_newlib(L, mathlib);
	lua_pushnumber(L, PI);
	lua_setfield(L, -2, "pi");
	lua_pushnumber(L, HUGE_VAL);
	lua_setfield(L, -2, "huge");
	return 1;
}