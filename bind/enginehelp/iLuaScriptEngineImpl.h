#pragma once

#include "interface/ILuaScriptHost.h"
#include "memory/ref_counted.h"

namespace binding{namespace lua{

class CLuaAcitveScriptImpl;

class CLuaScriptEngineImpl : public ILuaScriptEngine,
	public binding::RefCountedImpl<CLuaScriptEngineImpl>
{
public:
	CLuaScriptEngineImpl();
	~CLuaScriptEngineImpl();
public:
	static CLuaScriptEngineImpl* GetInstance();
//ILuaScriptEngine
	virtual void AddRef();
	virtual void Release();
	virtual long Open(IScriptLibStorage* pstg);
	virtual long Close(void);
	virtual long AddNamedItem(const std::wstring& strName,ILuaApi* pApi);
	virtual long GetNamedItem(const std::wstring& strName, ILuaApi** ppApi);
	virtual long AddScriptSite(ILuaScriptEngineSite* pScriptSite);
	virtual long RemoveScriptSite(ILuaScriptEngineSite* pScriptSite);
	virtual long Execute(const std::wstring& strScriptName, const std::wstring& strCommandLine);
	virtual long Execute2(const std::wstring& strScriptName, const std::wstring& strCommandLine, CLuaVariant& rvarRes);
	virtual long Load(const std::wstring& strScriptName, ILuaAcitveScript** ppActiveScript);
private:
	long ActiveScriptLoad(const std::wstring& strScriptName,CLuaAcitveScriptImpl* pLuaActiveScript);
	std::map<std::wstring,ILuaApi*> m_mapNameItem;
	ILuaScriptEngineSite* m_pLuaScriptEngineSite;
	IScriptLibStorage* m_spStorage;
};

}}