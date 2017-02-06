#include "stdafx.h"
#include "LuaEngine.h"
#include "LuaHelper.h"
#include "LuaDebug.h"
#include "LuaVarint.h"
#include "lstate.h"

namespace binding{namespace lua{

typedef struct READ_PARAM
{
	IScriptLibStream* pstm;
	bool bEndOfFile;
	char Buffer[1024];
} *PREAD_PARAM;

static const char* LuaStreamReader(lua_State* L, void* ud, size_t* sz)
{
	PREAD_PARAM rp = (PREAD_PARAM)ud;
	if(rp->bEndOfFile)
	{
		return NULL;
	}
	ULONG nBytesRead = 0;
	rp->pstm->Read(rp->Buffer, sizeof(rp->Buffer), &nBytesRead);
	if(nBytesRead == 0)
	{
		rp->bEndOfFile = true;
		*sz = 0;
		return NULL;
	}
	*sz = nBytesRead;
	return nBytesRead > 0 ? rp->Buffer : NULL;
}

static inline CLuaEngine* FromLuaState(lua_State* L)
{
	return (CLuaEngine*)( G(L)->ud );
}

std::wstring GetScriptName(lua_State* L)
{
	CLuaEngine* p = FromLuaState(L);
	return p->GetScriptName();
}

//////////////////////////////////////////////////////////////////////////
// CLuaEngine
CLuaEngine::CLuaEngine(void): m_pLuaState(NULL)
{
}

CLuaEngine::~CLuaEngine(void)
{
	CloseState();

	CWin32Heap Heap;
	bool bOwnHeap = m_Heap.m_bOwnHeap;
	Heap.Attach(m_Heap.Detach(), bOwnHeap);

	m_strScriptName.clear();
	if(m_pStorage)
	{
		m_pStorage->Release();
		m_pStorage = NULL;
	}
	for(auto idis : m_ApiList)
	{
		idis->Release();
	}
	m_ApiList.clear();

	for(auto lib : m_LibList)
	{
		ATLASSERT(lib);
		FreeLibrary(lib);
	}

	m_LibList.clear();
}

extern CWin32Heap g_luaHeap;

#ifndef _roundup
#define _roundup(s, a) \
	( (((s) + (a) - 1) & ~((a) - 1)) )
#define _rounddown(s, a) \
	( ((s) & ~((a) - 1)) )
#endif

void* CLuaEngine::lua_realloc(void *ptr,size_t osize,size_t nsize)
{
	CWin32Heap& heap = m_Heap.m_hHeap == NULL ? g_luaHeap : m_Heap;
	if(nsize == 0)
	{
		heap.Free(ptr);
		return NULL;
	}
	return heap.Reallocate(ptr, _roundup(nsize, 4));
}

HRESULT CLuaEngine::OpenState(void)
{
	if(m_pLuaState != NULL)
	{
		return S_OK;
	}

	m_Heap.Attach(::HeapCreate(0, 4*1024, 0), true);

	lua_State* L = lua_newstate(lua_realloc, this);
	if(L == NULL)
	{
		return E_OUTOFMEMORY;
	}
	m_pLuaState = L;

	LuaStackCheck(L);
	LuaAutoSetCurState(L);

// 	static const luaL_Reg lualibs[] =
// 	{
// 		{"", luaopen_base},
// 		{LUA_LOADLIBNAME, luaopen_package},
// 		{LUA_TABLIBNAME, luaopen_table},
// 		{LUA_IOLIBNAME, luaopen_io},
// 		{LUA_OSLIBNAME, luaopen_os},
// 		{LUA_STRLIBNAME, luaopen_string},
// 		{LUA_MATHLIBNAME, luaopen_math},
// 		{LUA_DBLIBNAME, luaopen_debug},
// 		{NULL, NULL}
// 	};

	lua_atpanic(L, lua_panic_none);
// 	for (const luaL_Reg *lib = lualibs; lib->func; ++lib)
// 	{
// 		lua_pushcfunction(L, lib->func);
// 		lua_pushstring(L, lib->name);
// 		lua_call(L, 1, 0);
// 	}

	luaL_openlibs(L);
	//luaL_requiref(L,"ffi",luaopen_ffi,1);
	//lua_pop(L, 1);  /* remove lib */

	//luaL_requiref(L,"cjson",luaopen_cjson,1);
	//lua_pop(L, 1);  /* remove lib */

	//luaL_requiref(L,"cjson_safe",luaopen_cjson_safe,1);
	//lua_pop(L, 1);  /* remove lib */

	//lsqlite3
	//luaL_requiref(L,"lsqlite3",luaopen_lsqlite3,1);
	//lua_pop(L, 1);  /* remove lib */

	//luaxml
 	//luaL_requiref(L,"xml",luaopen_LuaXML_lib,1);
 	//lua_pop(L, 1);  /* remove lib */

	static const luaL_Reg ext_base_funcs[] =
	{
		{"call", LuaApi_call},
		{"glockedcall", LuaApi_glocked_call},
		{"loadfile", LuaApi_loadfile},
		{"loadlib", LuaApi_loadlib},
		{"typeof", LuaApi_typeof},
		{NULL, NULL}
	};
	luaL_register(L, "_G", ext_base_funcs);
	lua_pop(L, 1); // 把栈顶的_G table移除
	return S_OK;
}


void CLuaEngine::CloseState(void)
{
	lua_State* L = m_pLuaState;
	m_pLuaState = NULL;
	if(L != NULL)
	{
		LuaAutoSetCurState(L);
		lua_close(L);
	}
}

void CLuaEngine::OnScriptException(int iErrCode,bool bTerminate /*= false*/)
{
	if(m_pLuaScriptEngineSite != NULL)
	{
		lua_State* L = m_pLuaState;
		const char* pszError = lua_tostring(L, -1);
		CComBSTR bstrDesc(pszError), bstrSource(m_strScriptName.c_str());
		EXCEPINFO ei = {0};
		ei.bstrDescription = bstrDesc;
		ei.bstrSource = bstrSource;
		ei.wCode = (WORD)iErrCode;
		if(bTerminate)
		{
			CLuaVariant var;
			var.setValue<int>(iErrCode);
			m_pLuaScriptEngineSite->OnScriptTerminate(var,&ei);
		}
		else
		{
			m_pLuaScriptEngineSite->OnScriptError(&ei);
		}
	}
}

HRESULT CLuaEngine::Register(ILuaApi* pApi,const std::string& strGlobalName)
{
	lua_State* L = m_pLuaState;
	if(L == NULL || pApi == NULL)
	{
		return E_POINTER;
	}

	LuaStackCheck(L);
	LuaAutoSetCurState(L);

	try
	{
		pApi->AddRef();
		m_ApiList.push_back(pApi);
	}
	catch(...)
	{
		return E_OUTOFMEMORY;
	}

	binding::lua::Register(L,strGlobalName.c_str(),pApi);

	return S_OK;
}

HRESULT CLuaEngine::Parse(const std::wstring& strScriptName)
{
	if(m_pStorage == NULL)
	{
		return E_POINTER;
	}
	m_strScriptName = strScriptName;
	lua_State* L = m_pLuaState;
	if (L == NULL)
	{
		return E_POINTER;
	}

	LuaStackCheck(L);
	LuaAutoSetCurState(L);

	IScriptLibStream* pstm(NULL);
	HRESULT hr = m_pStorage->OpenStream(m_strScriptName,&pstm);
	if(FAILED(hr))
	{
		return hr;
	}
	if(pstm == NULL)
	{
		return E_POINTER;
	}

	READ_PARAM rp;
	rp.bEndOfFile = false;
	rp.pstm = pstm;

	int res = lua_load(L, LuaStreamReader, &rp, "","bt");
	if(res != 0)
	{
		OnScriptException(res);
		pstm->Release();
		return HRESULT_FROM_WIN32(res);
	}

#if (defined _DEBUG || defined RELEASE_TEST)
	res = lua_pcall2(L, 0, 0, ErrorHandle);
#else
	res = lua_pcall(L, 0, 0, 0);
#endif
	if(res != 0)
	{
		OnScriptException(res);
		pstm->Release();
		return HRESULT_FROM_WIN32(res);
	}
	pstm->Release();
	return S_OK;
}

HRESULT CLuaEngine::Invoke(const std::wstring& strFunc,const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	lua_State* L = m_pLuaState;
	if(L == NULL)
	{
		return E_POINTER;
	}

	LuaAutoSetCurState(L);

	lua_getglobal(L, CW2A(strFunc.c_str()));
	return Invoke(rDispParams,rVarResult);
}

HRESULT CLuaEngine::Invoke(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	LuaAutoSetCurState(L);
	lua_State* L = m_pLuaState;

	ATLASSERT(lua_gettop(L) >= 1);
	int base = lua_gettop(L) - 1;
	HRESULT hr = InternalInvoke(rDispParams,rVarResult);
	int top = lua_gettop(L);
	ATLASSERT(top >= base);
	if(top > base)
	{
		lua_pop(L, top - base);
		ATLASSERT(lua_gettop(L) == base);
	}
	return hr;
}


HRESULT CLuaEngine::InternalInvoke(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	lua_State* L = m_pLuaState;

	ATLASSERT(lua_gettop(L) >= 1);
	int iStackBase = lua_gettop(L) - 1;

	int iArgc = rDispParams.size();
	for(auto var : rDispParams)
	{
		lua_push(L,var);
	}

#if (defined _DEBUG || defined RELEASE_TEST)
	int res = lua_pcall2(L, iArgc, LUA_MULTRET, ErrorHandle);
#else
	int res = lua_pcall(L, iArgc, LUA_MULTRET, 0);
#endif

	if(res != 0)
	{
		if(m_pLuaScriptEngineSite != NULL)
		{
			const char* pszError = lua_tostring(L, -1);
			CComBSTR bstrDesc(pszError), bstrSource(m_strScriptName.c_str());

			EXCEPINFO ei = {0};
			ei.bstrDescription = bstrDesc;
			ei.bstrSource = bstrSource;
			ei.wCode = (WORD)res;
			m_pLuaScriptEngineSite->OnScriptError(&ei);
		}
		//lua_pop(L, 1); //函数返回会lua_settop(L, 0)
		return HRESULT_FROM_WIN32(res);
	}

	int iNumRet = lua_gettop(L) - iStackBase;
	if(iNumRet > 0)
	{
		if(iNumRet > 1)
		{
			LuaVarArray array;
			for(int i = 0; i < iNumRet; ++i)
			{
				CLuaVariant var;
				lua_tovariant(L, i + 1 + iStackBase, var);
				array.push_back(var);
			}
			//return CComVariant(sa).Detach(pvarRes);
		}
		else
		{
			lua_tovariant(L, 1 + iStackBase, rVarResult);
		}
	}
	return S_OK;
}

int CLuaEngine::ApiInvoke(lua_State* L)
{
	return lua_callback_entry(L);
}

int CLuaEngine::LuaApi_loadfile(lua_State* L)
{
	bool success = false;
	CLuaEngine* pLuaEngine = FromLuaState(L);
	size_t len = 0;
	const char* fname = luaL_checklstring(L, 1, &len);
	if(fname && *fname != 0)
	{
		std::wstring strname = CA2W(fname);
		HRESULT hr = pLuaEngine->Parse(strname);
		if(SUCCEEDED(hr))
		{
			success = true;
		}
	}
	lua_pushboolean(L, success);
	return 1;
}

int CLuaEngine::LuaApi_loadlib(
	lua_State *L)
{
	return FromLuaState(L)->LuaApi_loadlib();
}

typedef HRESULT (__stdcall *DllCreateObjectFunc)(LPCWSTR name, void **ppv);

int CLuaEngine::LuaApi_loadlib(void)
{
	lua_State *L = m_pLuaState;

	size_t len = 0;
	const char* pszLibname = luaL_checklstring(L, 1, &len);
	const char* pszfull_name = luaL_checklstring(L, 2, &len);
	if( pszLibname == NULL || pszfull_name == NULL ||
		*pszLibname == 0 || *pszfull_name == 0 )
	{
		return 0;
	}

	HMODULE hModule = LoadLibraryA(pszLibname);
	if(hModule)
	{
		DllCreateObjectFunc func = (DllCreateObjectFunc)::GetProcAddress(hModule, "DllCreateObject");
		if (func)
		{
			ILuaApi* pApi(NULL);
			func(CA2W(pszfull_name),(void**)&pApi);
			if(pApi)
			{
				m_LibList.push_back(hModule);
				m_ApiList.push_back(pApi);
				lua_push(L, pApi);
				return 1;
			}
		}
	}


	FreeLibrary(hModule);
	return 0;
}

// call(luafunc, ...)
int CLuaEngine::LuaApi_call(lua_State *L)
{
	luaL_checkany(L, 1);
	lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
	return lua_gettop(L);
}

int CLuaEngine::LuaApi_glocked_call(lua_State *L)
{
	//CComObjectLockT<CComMultiThreadModel> AutoLock(FromLuaState(L)->m_pEngine);
	luaL_checkany(L, 1);
	lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
	return lua_gettop(L);
}

int CLuaEngine::LuaApi_typeof(lua_State *L)
{
	luaL_checkany(L, 1);
	lua_pushinteger(L, lua_type(L, 1));
	return 1;
}

}}