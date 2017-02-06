#pragma once

#include "interface/ILuaApi.h"

typedef struct lua_State lua_State;

namespace binding{namespace lua{

int ShowStack(lua_State* L);
int ErrorHandle(lua_State* L);

void DumpArgsInfo(std::wstring& str,const DISP_PARAMS& rParams);

void DebugInvokError(lua_State* L,HRESULT hr,const std::wstring& strScriptName,ILuaApi* pApi,DISP_ID dispid,const DISP_PARAMS& rParams);

#if (defined _DEBUG || defined _RELEASE_TEST)
//////////////////////////////////////////////////////////////////////////
// CLuaStackDebugCheck
class CLuaStackDebugCheck
{
public:
	lua_State* m_L;
	const char* m_pszFunc;
	const char* m_pszFile;
	int m_iLine;
	int m_top;

	CLuaStackDebugCheck(
		lua_State* L, 
		const char* func,
		const char* file,
		int line);
	~CLuaStackDebugCheck(void);
};

#define LuaStackCheck(L)		CLuaStackDebugCheck __lua_stack_debug_check__(L, __FUNCTION__, __FILE__, __LINE__)

#else // (defined _DEBUG || defined _RELEASE_TEST)

#define LuaStackCheck(L)

#endif // (defined _DEBUG || defined _RELEASE_TEST)

}}