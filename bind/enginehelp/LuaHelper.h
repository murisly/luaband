#pragma once

#include "llimits.h"
#include "interface/ILuaApi.h"

namespace binding{namespace lua{

#ifdef LUA_CPLUSPLUS
#define LUA_C_EXTERN
#else
#define LUA_C_EXTERN extern "C"
#endif

LUA_C_EXTERN void __cdecl lua_cppthrow(int _Code);

inline void lua_push(lua_State* L, bool v)  { lua_pushboolean(L, v); }

inline void lua_push(lua_State* L, char v)  { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, short v) { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, int v)   { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, long v)  { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, long long v)  { lua_pushnumber(L, cast_num(v)); }

inline void lua_push(lua_State* L, unsigned char v)  { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, unsigned short v) { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, unsigned int v)   { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, unsigned long v)  { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, unsigned long long v)  { lua_pushnumber(L, cast_num(v)); }

inline void lua_push(lua_State* L, float v)  { lua_pushnumber(L, cast_num(v)); }
inline void lua_push(lua_State* L, double v)  { lua_pushnumber(L, cast_num(v)); }

extern void lua_push(lua_State* L, const std::wstring& str);
extern void lua_push(lua_State* L, const CLuaVariant& var);
extern void lua_push(lua_State* L, ILuaElementCollection* pColl);
extern void lua_push(lua_State* L, ILuaApi* pApi);
extern int lua_push(lua_State* L, const LuaVarArray& varArray);
extern void lua_push(lua_State* L, const std::vector<char>& vaBinary);

extern void lua_settable(lua_State* L, ILuaApi* pApi);

extern int lua_panic_none(lua_State *L);

extern void lua_tovariant(lua_State* L, int idx, CLuaVariant& var);
extern void lua_totable(lua_State* L, int idx, CLuaVariant& var);

extern int lua_pcall2(lua_State* L, int nargs, int nresults, lua_CFunction errfunc);

extern int ApiInvoke(lua_State* L,ILuaApi* pApi,DISP_ID dispid);
extern int lua_callback_entry(lua_State* L);

extern void stackDump (lua_State *L);
extern void Register(lua_State *L,const char* pszGlobalName,ILuaApi* p);

#if LUA_SAVE_STATE

void SetCurLuaState(lua_State* L);
lua_State* GetCurLuaState(void);

struct CAutoSetCurLuaState
{
	CAutoSetCurLuaState(lua_State* _L)
	{
		SetCurLuaState(_L);
	}
	~CAutoSetCurLuaState(void)
	{
		SetCurLuaState(NULL);
	}
};

#define LuaAutoSetCurState(L)		CAutoSetCurLuaState __auto_set_cur_lua_state(L)

#else // LUA_SAVE_STATE

inline void SetCurLuaState(lua_State* L) {}
inline lua_State* GetCurLuaState(void) { return NULL; }
#define LuaAutoSetCurState(L)

#endif // LUA_SAVE_STATE

}}