#pragma once

#include "interface/ILuaScriptHost.h"
#include "memory/ref_counted.h"
#include "LuaEngine.h"

namespace binding{namespace lua{

template<class Base>
class CObjectStack :public Base
{
public:
	typedef Base _BaseClass;
	CObjectStack()
	{
		RefCountedBase::AddRef();
	}
	virtual ~CObjectStack()
	{
		RefCountedBase::Release();
	}
	virtual void AddRef()
	{
	}
	virtual void Release()
	{
	}
};

class CLuaAcitveScriptImpl 
	:public ILuaAcitveScript
	,public binding::RefCountedImpl<CLuaAcitveScriptImpl>
	,public CLuaEngine
{
	bool m_bException;
public:
	CLuaAcitveScriptImpl();
	~CLuaAcitveScriptImpl();
public:
	static CLuaAcitveScriptImpl* GetInstance();
	//ILuaAcitveScript
	virtual void AddRef();
	virtual void Release();
	virtual long AddNamedItem(const std::wstring& strName,ILuaApi* pApi);
	virtual long GetNamedItem(const std::wstring& strName, ILuaApi** ppApi);
	virtual long AddScriptSite(ILuaScriptEngineSite* pScriptSite);
	virtual long RemoveScriptSite(ILuaScriptEngineSite* pScriptSite);
	virtual long Invoke(const std::wstring& strFuncName, const DISP_PARAMS& rdp, CLuaVariant& rvarRes);
	virtual long Load(const std::wstring& strScriptName);

	long Open(void);
};
}}