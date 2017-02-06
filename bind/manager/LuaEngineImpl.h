#pragma once

#include "interface/ILuaApiT.h"
#include "interface/ILuaScriptHost.h"
#include "enginehelp/Mutex.h"

using namespace binding::lua;

class ILuaCallBack;

class CLuaEngineImpl :  public CApiDispImplRootT<CLuaEngineImpl> ,public binding::lua::ILuaScriptEngineSite
{
public:
	BEGIN_APIDISP_MAP(CLuaEngineImpl)

		APIDISP_ENTRY_F(Print, "s")

	END_APIDISP_MAP()
public:
	CLuaEngineImpl();
	~CLuaEngineImpl();
	bool AddLuaCallBack(const std::wstring &wstrId,  ILuaCallBack* pCallBack);
	bool RemoveLuaCallBack(const std::wstring &wstrId );

	//ILuaCallBack function only for lua script
	long AddStartupResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long AddScanFileResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long AddScanRegResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long AddScanRegValueResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long GetAllStartups(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);

	DECLARE_LUAFUNC(Print)

	//ILuaScriptEngineSite
	long OnEnterScript(void);
	long OnLeaveScript(void);
	long OnScriptError(const EXCEPINFO* pexcepinfo);
	long OnScriptTerminate(const CLuaVariant& rvarRes, const EXCEPINFO *pexcepinfo);
private:
	ILuaCallBack* m_pLuaCallBack;
	std::wstring m_wstrId;
	binding::thread::Mutex m_Mutex_LuaCallBack;
};