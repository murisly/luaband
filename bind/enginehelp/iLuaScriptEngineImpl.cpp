#include "stdafx.h"
#include <shellapi.h>
#include "iLuaScriptEngineImpl.h"
#include "LuaVarint.h"
#include "iLuaAcitveScriptImpl.h"
#include "LuaVariantTable.h"

namespace binding{namespace lua{

CLuaScriptEngineImpl* CLuaScriptEngineImpl::GetInstance()
{
	CLuaScriptEngineImpl* pImpl  = new CLuaScriptEngineImpl;
	pImpl->AddRef();
	return pImpl;
}

CLuaScriptEngineImpl::CLuaScriptEngineImpl()
{
	m_pLuaScriptEngineSite = NULL;
	m_spStorage = NULL;
}

CLuaScriptEngineImpl::~CLuaScriptEngineImpl()
{
	for(auto it : m_mapNameItem)
	{
		if(it.second)
			it.second->Release();
	}
	m_mapNameItem.clear();
	if(m_spStorage)
	{
		m_spStorage->Release();
		m_spStorage = NULL;
	}
	m_pLuaScriptEngineSite = NULL;
}

void CLuaScriptEngineImpl::AddRef()
{
	InternalAddRef();
}

void CLuaScriptEngineImpl::Release()
{
	InternalRelease();
}

long CLuaScriptEngineImpl::Open(IScriptLibStorage* pstg)
{
	m_spStorage = pstg;
	if(m_spStorage)
		m_spStorage->AddRef();
	return 0;
}

long CLuaScriptEngineImpl::Close(void)
{
	if(m_spStorage)
	{
		m_spStorage->Release();
		m_spStorage = NULL;
	}

	return 0;
}

long CLuaScriptEngineImpl::AddNamedItem(const std::wstring& strName,ILuaApi* pApi)
{
	auto it  = m_mapNameItem.find(strName);
	if(it != m_mapNameItem.end())
	{
		it->second->Release();
		it->second = NULL;
		m_mapNameItem.erase(it);
	}

	m_mapNameItem.insert(std::map<std::wstring,ILuaApi*>::value_type(strName,pApi));
	if(pApi)
		pApi->AddRef();

	return 0;
}

long CLuaScriptEngineImpl::GetNamedItem(const std::wstring& strName, ILuaApi** ppApi)
{
	if(ppApi == NULL)
		return 1;

	auto it  = m_mapNameItem.find(strName);
	if(it != m_mapNameItem.end())
	{
		(*ppApi) = it->second;
		if((*ppApi) != NULL)
			(*ppApi)->AddRef();
	}
	return 0;
}

long CLuaScriptEngineImpl::AddScriptSite(ILuaScriptEngineSite* pScriptSite)
{
	m_pLuaScriptEngineSite = pScriptSite;
	return 0;
}

long CLuaScriptEngineImpl::RemoveScriptSite(ILuaScriptEngineSite* pScriptSite)
{
	if(m_pLuaScriptEngineSite == pScriptSite)
		m_pLuaScriptEngineSite = NULL;
	return 0;
}

long CLuaScriptEngineImpl::Execute(const std::wstring& strScriptName, const std::wstring& strCommandLine)
{
	CLuaVariant var;
	return Execute2(strScriptName,strCommandLine,var);
}

long CLuaScriptEngineImpl::Execute2(const std::wstring& strScriptName, const std::wstring& strCommandLine, CLuaVariant& rvarRes)
{
	CObjectStack<CLuaAcitveScriptImpl> LuaActiveScript;
	LuaActiveScript.SetEngine(this);
	long lRet = ActiveScriptLoad(strScriptName,&LuaActiveScript);
	if(lRet < 0)
	{
		return lRet;
	}

	CLuaVariantTable* pColl = NULL;
	CLuaVariantTable::Create(&pColl);
	if(pColl == NULL)
		return -1;

	DISP_PARAMS dp;
	int iNumArgs = 0;
	if(!strCommandLine.empty())
	{
		PWSTR* pwszArgvs = CommandLineToArgvW(strCommandLine.c_str(),&iNumArgs);
		if(pwszArgvs)
		{
			for(int i = 0; i < iNumArgs; ++i)
			{
				CLuaVariant var;
				var.setValue<std::wstring>(std::wstring(pwszArgvs[i]));
				pColl->Add(CLuaVariant(),var);
			}
			LocalFree(pwszArgvs);
		}
	}
	CLuaVariant varNumArgs;
	varNumArgs.setValue<int>(iNumArgs);
	dp.push_back(varNumArgs);
	CLuaVariant varColl;
	varColl.setValue<ILuaElementCollection*>(pColl);
	dp.push_back(varColl);
	pColl->Release();

	return LuaActiveScript.Invoke(L"main",dp,rvarRes);
}

long CLuaScriptEngineImpl::Load(const std::wstring& strScriptName, ILuaAcitveScript** ppActiveScript)
{
	if(ppActiveScript == NULL)
		return E_POINTER;

	*ppActiveScript = NULL;

	CLuaAcitveScriptImpl* p = CLuaAcitveScriptImpl::GetInstance();
	p->SetEngine(this);
	long lRet = ActiveScriptLoad(strScriptName, p);
	if(lRet == 0)
	{
		*ppActiveScript = p;
	}
	else
	{
		p->Release();
	}
	return lRet;
}

long CLuaScriptEngineImpl::ActiveScriptLoad(const std::wstring& strScriptName,CLuaAcitveScriptImpl* pLuaActiveScript)
{
	long lRet = pLuaActiveScript->Open();
	if(0 != lRet)
		return lRet;

	// Register Native APIs
	{

	}
	// Register APIs
	{
		for(auto it : m_mapNameItem)
		{
			lRet = pLuaActiveScript->AddNamedItem(it.first,it.second);
			if(lRet != 0)
				return lRet;
		}
		pLuaActiveScript->SetStorage(m_spStorage);
		pLuaActiveScript->AddScriptSite(m_pLuaScriptEngineSite);
	}
	return pLuaActiveScript->Load(strScriptName);
}

}}