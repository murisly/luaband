#pragma once

#include <objidl.h>
#include "ILuaApi.h"
#include "IScriptLibStorage.h"

namespace binding{namespace lua{

	class ILuaScriptEngineSite
	{
	public:
		virtual long OnEnterScript(void) = 0;
		virtual long OnLeaveScript(void) = 0;
		virtual long OnScriptError(const EXCEPINFO* pexcepinfo) = 0;
		virtual long OnScriptTerminate(const CLuaVariant& rvarRes, const EXCEPINFO *pexcepinfo) = 0;
	};

	class ILuaAcitveScript
	{
	public:
		virtual void AddRef() = 0;
		virtual void Release() = 0;
		virtual long AddNamedItem(const std::wstring& strName,ILuaApi* pApi) = 0;
		virtual long GetNamedItem(const std::wstring& strName, ILuaApi** ppApi) = 0;
		virtual long AddScriptSite(ILuaScriptEngineSite* pScriptSite) = 0;
		virtual long RemoveScriptSite(ILuaScriptEngineSite* pScriptSite) = 0;
		virtual long Invoke(const std::wstring& strFuncName, const DISP_PARAMS& rdp, CLuaVariant& rvarRes) = 0;
		virtual long Load(const std::wstring& strScriptName) = 0;
	};

	class ILuaScriptEngine
	{
	public:
		virtual void AddRef() = 0;
		virtual void Release() = 0;
		virtual long Open(IScriptLibStorage* pstg) = 0;
		virtual long Close(void) = 0;
		virtual long AddNamedItem(const std::wstring& strName,ILuaApi* pApi) = 0;
		virtual long GetNamedItem(const std::wstring& strName, ILuaApi** ppApi) = 0;
		virtual long AddScriptSite(ILuaScriptEngineSite* pScriptSite) = 0;
		virtual long RemoveScriptSite(ILuaScriptEngineSite* pScriptSite) = 0;
		virtual long Execute(const std::wstring& strScriptName, const std::wstring& strCommandLine) = 0;
		virtual long Execute2(const std::wstring& strScriptName, const std::wstring& strCommandLine, CLuaVariant& rvarRes) = 0;
		virtual long Load(const std::wstring& strScriptName, ILuaAcitveScript** ppActiveScript) = 0;
	};
}}