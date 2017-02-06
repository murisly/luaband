#include "stdafx.h"
#include <atlmem.h>
#include "LuaHelper.h"
#include "LuaVariantTable.h"
#include "lstate.h"
#include "LuaDebug.h"


namespace binding{namespace lua{

#define abs_index(L, i)		((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : \
	lua_gettop(L) + (i) + 1)

CWin32Heap g_luaHeap((HANDLE)_get_heap_handle());

extern std::wstring GetScriptName(lua_State* L);

void stackDump (lua_State *L) { 
	int i; 
	int top = lua_gettop(L); 
	for (i = 1; i <= top; i++) {  /* repeat for each level */ 
		int t = lua_type(L, i); 
		switch (t) { 

		case LUA_TSTRING:  /* strings */ 
			printf("`%s'", lua_tostring(L, i)); 
			break; 

		case LUA_TBOOLEAN:  /* booleans */ 
			printf(lua_toboolean(L, i) ? "true" : "false"); 
			break; 

		case LUA_TNUMBER:  /* numbers */ 
			printf("%g", lua_tonumber(L, i)); 
			break; 
		default:  /* other values */ 
			printf("%s", lua_typename(L, t)); 
			break; 

		} 
		printf("  ");  /* put a separator */ 
	} 
	printf("\n");   /* end the listing */ 
}

void lua_push(lua_State* L, const std::wstring& str)
{
	CW2A sz(str.c_str(), CP_UTF8);
	lua_pushlstring(L, sz, strlen(sz));
}

void lua_push(lua_State* L, ILuaElementCollection* pColl)
{
	size_t lCount = 0;
	pColl->get_count(lCount);
	lua_createtable(L, 0, (int)lCount);
	for(size_t i = 0; i < lCount; ++i)
	{
		CLuaVariant key, val;
		pColl->get_key(i, key);
		pColl->item(CLuaVariant(), i, val);
		if(key.getType() != CLuaVariant::VAR_EMPTY)
			lua_push(L, key);
		else
			lua_push(L, i + 1);
		lua_push(L, val);
		lua_rawset(L, -3);
	}
}

void lua_push(lua_State* L, ILuaApi* pApi)
{
	lua_createtable(L, 0, 4);
	lua_pushlstring(L, "__dispex", 8);
	lua_pushlightuserdata(L, pApi);
	lua_rawset(L, -3);
	lua_settable(L, pApi);
}

void lua_push(lua_State* L, const CLuaVariant& var)
{
	switch(var.getType())
	{
	case CLuaVariant::VAR_INT:
		lua_push(L, var.getValue<int>());
		break;	
	case CLuaVariant::VAR_CHAR:
		lua_push(L, var.getValue<char>());
		break;		
	case CLuaVariant::VAR_SHORT:
		lua_push(L, var.getValue<short>());
		break;
	case CLuaVariant::VAR_LONG:
		lua_push(L, var.getValue<long>());
		break;
	case CLuaVariant::VAR_LONGLONG:
		lua_push(L, var.getValue<__int64>());
		break;
	case CLuaVariant::VAR_UINT:
		lua_push(L, var.getValue<unsigned int>());
		break;
	case CLuaVariant::VAR_UCHAR:
		lua_push(L, var.getValue<unsigned char>());
		break;
	case CLuaVariant::VAR_USHORT:
		lua_push(L, var.getValue<unsigned short>());
		break;
	case CLuaVariant::VAR_ULONG:
		lua_push(L, var.getValue<unsigned long>());
		break;
	case CLuaVariant::VAR_ULONGLONG:
		lua_push(L, var.getValue<unsigned __int64>());
		break;
	case CLuaVariant::VAR_FLOAT:
		lua_push(L, var.getValue<float>());
		break;
	case CLuaVariant::VAR_DOUBLE:
		lua_push(L, var.getValue<double>());
		break;
	case CLuaVariant::VAR_BOOL:
		lua_push(L, var.getValue<bool>());
		break;
	case CLuaVariant::VAR_WSTRING:
		lua_push(L, var.getValue<std::wstring>());
		break;
	case CLuaVariant::VAR_ILUAEC:
		lua_push(L, var.getValue<ILuaElementCollection*>());
		break;
	case CLuaVariant::VAR_ILUAAPI:
		lua_push(L, var.getValue<ILuaApi*>());
		break;
	case CLuaVariant::VAR_BINARY:
		lua_push(L, var.getValue<std::vector<char>>());
		break;
	default:
		lua_pushnil(L);
		break;
	}
}

int lua_push(lua_State* L, const LuaVarArray& varArray)
{
	for(size_t i = 0 ; i < varArray.size() ; i++)
	{
		lua_push(L,varArray[i]);
	}
	return varArray.size();
}

void lua_push(lua_State* L, const std::vector<char>& vaBinary)
{
	lua_pushlstring(L, &vaBinary[0], vaBinary.size());
}

void lua_tovariant(lua_State* L, int idx, CLuaVariant& v)
{
	int iType = lua_type(L, idx);
	switch(iType)
	{
	case LUA_TBOOLEAN:
		v.setValue<bool>( lua_toboolean(L, idx) != 0 );
		break;
	case LUA_TNUMBER:
		{
			double d = lua_tonumber(L, idx);
			int ll = (int)d;
			if(d == (double)ll)
			{
				v.setValue<int>(ll);
			}
			else
			{
				v.setValue<double>(d);
			}
		}
		break;

	case LUA_TSTRING:
		{
			size_t len = 0;
			const char* psz = lua_tolstring(L, idx, &len);
			if(psz && len > 0)
			{
				if(psz[0] == '\0')
				{
					std::vector<char> vecBuff;
					vecBuff.resize(len - 1,0);
					if(len - 1 > 0)
						memcpy_s(&vecBuff[0],len -1 ,psz+1,len -1 );
					v.setValue<std::vector<char>>(vecBuff);
				}
				else
				{
					v.setValue<std::wstring>(std::wstring(CA2W(psz, CP_UTF8)));
				}
		
			}
			else
			{
				v.setValue<std::wstring>(L"");
			}
		}
		break;

	case LUA_TTABLE:
		{
			lua_getfield(L, idx, "__dispex");
			if(!lua_isnil(L, -1))
			{
				v.setValue<ILuaApi*>((ILuaApi*)lua_touserdata(L, -1));
				lua_pop(L, 1);
			}
			else
			{
				lua_pop(L, 1);
				lua_totable(L, idx, v);
			}
		}
		break;
	case LUA_TNIL:
		v.Clear();
		break;
	default:
		break;
	}
}

void lua_totable(lua_State* L, int idx, CLuaVariant& var)
{
	CLuaVariantTable* pColl = NULL;
	CLuaVariantTable::Create(&pColl);
	if(pColl == NULL)
		return;

	lua_pushnil(L);
	while(lua_next(L, idx) != 0)
	{
		CLuaVariant key, val;
		lua_tovariant(L, abs_index(L, -2), key);
		lua_tovariant(L, abs_index(L, -1), val);
		pColl->Add(key,val);
		lua_pop(L, 1);
	}
	var.setValue<ILuaElementCollection*>(pColl);
	pColl->Release();
}

#pragma warning(push)
#pragma warning(disable: 6011)

int ApiInvoke(lua_State* L,ILuaApi* pApi,DISPID dispid)
{
	long hr = E_FAIL;
	CLuaVariant VarResult;
	{
		DISP_PARAMS VarVector;
		int iNumArgs = lua_gettop(L);

		for(int i = 0; i < iNumArgs; ++i)
		{
			CLuaVariant var;
			lua_tovariant(L, i + 1, var);
			VarVector.push_back(var);
		}

		hr = pApi->Invoke(
			dispid,
			VarVector,
			VarResult);
#if (defined _DEBUG || defined _RELEASE_TEST)
		if(FAILED(hr))
		{
			DebugInvokError(
				L,
				hr,
				GetScriptName(L),
				pApi,
				dispid,
				VarVector);
		}
#endif
	}

	lua_pushinteger(L, hr);
	INT iNumResult = 1;
	if(SUCCEEDED(hr))
	{
		if(VarResult.getType() == CLuaVariant::VAR_VARARRAY)
		{
			iNumResult += lua_push(L,VarResult.getValue<LuaVarArray>());
		}
		else if(VarResult.getType() != CLuaVariant::VAR_EMPTY)
		{
			lua_push(L, VarResult);
			++iNumResult;
		}
	}
	return iNumResult;
}

#pragma warning(pop)

int lua_callback_entry(lua_State* L)
{
	ILuaApi* pApi = (ILuaApi*)lua_touserdata(L, lua_upvalueindex(1));
	DISPID dispid = (DISPID)(LONG_PTR)lua_touserdata(L, lua_upvalueindex(2));
	return ApiInvoke(L,pApi,dispid);
}

void lua_settable(lua_State* L, ILuaApi* pApi)
{
	ILuaApi::DispTable table;
	pApi->GetDispTable(table);
	for(auto it : table)
	{
		lua_pushlightuserdata(L, pApi);
		lua_pushlightuserdata(L, LongToPtr(it.first));
		lua_pushcclosure(L, lua_callback_entry, 2);
		lua_setfield(L, -2, it.second.c_str());
	}
}

void Register(lua_State *L,const char* pszGlobalName,ILuaApi* p) 
{
	const char* libname = ( (pszGlobalName != NULL && *pszGlobalName != 0) ? pszGlobalName : "_G" );
	// check whether lib already exists
	luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, -1, libname);  // get _LOADED[libname]
	if (!lua_istable(L, -1))
	{  
		// not found?
		lua_pop(L, 1);  // remove previous result

		// try global variable (and create one if it does not exist)
		lua_getglobal(L,libname);
		if(!lua_isnil(L,-1))
		{  
			lua_pop(L, 1);  // remove previous result
			luaL_error(L, "name conflict for module " LUA_QS, libname);
		}

		lua_pop(L, 1);  // remove previous result
		lua_newtable(L);
		lua_setglobal(L, libname);
		lua_getglobal(L,libname);

		lua_pushvalue(L, -1);
		lua_setfield(L, -3, libname);  // _LOADED[T::classname] = new table
	}
	lua_remove(L, -2);  // remove _LOADED table
	lua_settable(L,p);
	lua_pop(L, 1); // °ÑÕ»¶¥µÄT::classname tableÒÆ³ý
}

#if LUA_SAVE_STATE

static DWORD LuaTlsInit(void);

static DWORD g_dwLuaTlsIndex = LuaTlsInit();

static void LuaTlsUninit(void)
{
	if(g_dwLuaTlsIndex)
	{
		TlsFree(g_dwLuaTlsIndex);
		g_dwLuaTlsIndex = 0;
	}
}

static DWORD LuaTlsInit(void)
{
	atexit(LuaTlsUninit);
	return TlsAlloc();
}

void SetCurLuaState(lua_State* L)
{
	if(g_dwLuaTlsIndex)
		TlsSetValue(g_dwLuaTlsIndex, L);
}

lua_State* GetCurLuaState(void)
{
	if(g_dwLuaTlsIndex == 0)
		return NULL;
	return (lua_State*)TlsGetValue(g_dwLuaTlsIndex);
}

#endif // LUA_SAVE_STATE

LUA_C_EXTERN void __cdecl lua_cppthrow(int _Code)
{
	ATLTRACE("%s(%d): lua_cppthrow(%d) call.\n", __FILE__, __LINE__, _Code);
	char luaStateMem[sizeof(lua_State)] = {0};
	char glbStateMem[sizeof(global_State)] = {0};
	ULONG_PTR Args[3];

	lua_State* L = GetCurLuaState();
	if(L)
	{
		CopyMemory(luaStateMem, L, sizeof(luaStateMem));
		if(G(L))
		{
			CopyMemory(glbStateMem, G(L), sizeof(glbStateMem));
		}
	}

	Args[0] = (ULONG_PTR)_Code;
	Args[1] = (ULONG_PTR)luaStateMem;
	Args[2] = (ULONG_PTR)glbStateMem;
	AtlThrow(PtrToUlong(&Args));
}

int lua_panic_none(lua_State *L)
{
	lua_cppthrow(0);
	return 0;
}

int lua_pcall2(lua_State* L, int nargs, int nresults, lua_CFunction errfunc)
{
	int base = 0;
	if(errfunc)
	{
		base = lua_gettop(L) - nargs;
		lua_pushcfunction(L, errfunc);
		lua_insert(L, base);
	}
	int res = lua_pcall(L, nargs, nresults, base);
	if(errfunc)
	{
		ATLASSERT(lua_isfunction(L, base));
		lua_remove(L, base);
	}
	return res;
}

}}