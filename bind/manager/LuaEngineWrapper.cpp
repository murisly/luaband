#include "stdafx.h"
#include "LuaEngineWrapper.h"
#include "LuaEngineImpl.h"

CLuaEngineWrapper::CLuaEngineWrapper()
{
}

CLuaEngineWrapper::~CLuaEngineWrapper()
{
}

bool CLuaEngineWrapper::Init(CLuaEngineImpl* pLuaEngineImpl)
{
	bool bRet = InitScriptEngine(pLuaEngineImpl);
	if (bRet)
	{
		bRet = InitScriptApi();
		if(bRet)
		{
			m_spScriptEngine->AddScriptSite(pLuaEngineImpl);
		}
	}

	return bRet;
}

bool CLuaEngineWrapper::OpenLuaBuff(const std::vector<char>& vecBuff)
{
	CSmartPtr<binding::lua::IScriptLibStorage> spStorage;

	if(S_OK != spStorage->OpenBuff(vecBuff))
		return false;

	if(S_OK != m_spScriptEngine->Close())
		return false;

	if(S_OK != m_spScriptEngine->Open(spStorage))
		return false;

	return true;
}

bool CLuaEngineWrapper::InitScriptEngine(CLuaEngineImpl *pLuaEngineImpl)
{
	m_spScriptEngine->AddNamedItem(L"oaApi", pLuaEngineImpl);
	return true;
}

bool CLuaEngineWrapper::InitScriptApi()
{
	m_spScriptEngine->AddNamedItem(L"osApi", m_spOsApi);
	return true;
}

void CLuaEngineWrapper::Uninit()
{
	m_spScriptEngine.Release();
	m_spOsApi.Release();
	m_spLogicApi.Release();
}

bool CLuaEngineWrapper::Execute(const std::wstring &strGuidFile, const std::wstring &strCmd,CLuaVariant& rResult)
{
	if (!m_spScriptEngine || m_spScriptEngine->Execute2(strGuidFile, strCmd,rResult) != S_OK)
	{
		return false;
	}
	return true;
}