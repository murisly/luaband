#pragma once

#include <string>
#include "interface/ILuaScriptHost.h"
#include "interface/AutoPtrT.h"
#include "enginehelp/LuaVarint.h"

class CLuaEngineImpl;

class CLuaEngineWrapper
{
public:
	CLuaEngineWrapper();
	~CLuaEngineWrapper();
	bool Init(CLuaEngineImpl* pLuaEngineImpl);
	void Uninit();
	bool OpenLuaBuff(const std::vector<char>& vecBuff);
	bool Execute(const std::wstring &strGuidFile, const std::wstring &strCmd,binding::lua::CLuaVariant& rResult);
private:
	bool InitScriptEngine(CLuaEngineImpl *pLuaEngineImpl);
	bool InitScriptApi();

	CSmartPtr<binding::lua::ILuaScriptEngine> m_spScriptEngine;
	CSmartPtr<binding::lua::ILuaApi> m_spOsApi;
	CSmartPtr<binding::lua::ILuaApi> m_spLogicApi;
};