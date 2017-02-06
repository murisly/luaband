#pragma once

#include <list>
#include <atlmem.h>
#include "iLuaScriptEngineImpl.h"

typedef struct lua_State lua_State;

namespace binding{namespace lua{

class ILuaScriptEngineSite;
class CLuaScriptEngineImpl;

class CLuaEngine
{
protected:
	lua_State* m_pLuaState;
	IScriptLibStorage* m_pStorage;
	std::wstring m_strScriptName;
	std::list<ILuaApi*> m_ApiList;
	std::list<HMODULE> m_LibList;
	CWin32Heap m_Heap;
	ILuaScriptEngineSite* m_pLuaScriptEngineSite;
	CLuaScriptEngineImpl* m_pLuaScriptEngine;
protected:
	CLuaEngine(void);
	virtual ~CLuaEngine(void);
public:
	void SetStorage(IScriptLibStorage* pStorage) throw()
	{
		m_pStorage = pStorage;
		if(m_pStorage)
			m_pStorage->AddRef();
	}
	void SetScriptSite(ILuaScriptEngineSite* pScriptSite)
	{
		m_pLuaScriptEngineSite = pScriptSite;
	}
	void SetEngine(CLuaScriptEngineImpl* pEngine)
	{
		m_pLuaScriptEngine = pEngine;
	}
	std::wstring GetScriptName()
	{
		return m_strScriptName;
	}
public:
	long OpenState(void) throw();
	void CloseState(void) throw();
	long Register(ILuaApi* pApi,const std::string& strGlobalName) throw();
	long Parse(const std::wstring& strScriptName) throw();
	long Invoke(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult) throw();
	long Invoke(const std::wstring& strFunc,const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult) throw();
protected:
	long InternalInvoke(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult) throw();
public:
	static int ApiInvoke(lua_State* L);
	static int LuaApi_loadfile(lua_State *L);
	static int LuaApi_loadlib(lua_State *L);
	static int LuaApi_call(lua_State *L);
	static int LuaApi_glocked_call(lua_State *L);
	static int LuaApi_typeof(lua_State *L);
	static void* lua_realloc(void* ud,void *ptr,size_t osize,size_t nsize)
	{
		return ((CLuaEngine*)ud)->lua_realloc(ptr, osize, nsize);
	}

	void* lua_realloc(void *ptr,size_t osize,size_t nsize);
	int LuaApi_loadlib(void);
protected:
	void OnScriptException(int iErrCode,bool bTerminate = false);
};
}}