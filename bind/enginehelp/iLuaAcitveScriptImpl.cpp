#include "stdafx.h"
#include "iLuaAcitveScriptImpl.h"

namespace binding{namespace lua{

CLuaAcitveScriptImpl* CLuaAcitveScriptImpl::GetInstance()
{
	CLuaAcitveScriptImpl* pImpl  = new CLuaAcitveScriptImpl;
	pImpl->AddRef();
	return pImpl;
}

CLuaAcitveScriptImpl::CLuaAcitveScriptImpl()
{
	m_bException = false;
}

CLuaAcitveScriptImpl::~CLuaAcitveScriptImpl()
{
}

void CLuaAcitveScriptImpl::AddRef()
{
	InternalAddRef();
}

void CLuaAcitveScriptImpl::Release()
{
	if(HasOneRef())
	{
		try
		{
			CLuaEngine::CloseState();
		}
		catch(...)
		{

		}
	}
	InternalRelease();
}

long CLuaAcitveScriptImpl::AddNamedItem(const std::wstring& strName,ILuaApi* pApi)
{
	if(m_bException)
		return E_FAIL;

	try
	{
		std::string str = CW2A(strName.c_str());
		return CLuaEngine::Register(pApi,str);
	}
	catch(...)
	{
		m_bException = true;
	}
	return E_FAIL;
}

long CLuaAcitveScriptImpl::GetNamedItem(const std::wstring& strName, ILuaApi** ppApi)
{
	return E_NOTIMPL;
}

long CLuaAcitveScriptImpl::AddScriptSite(ILuaScriptEngineSite* pScriptSite)
{
	CLuaEngine::SetScriptSite(pScriptSite);
	return S_OK;
}

long CLuaAcitveScriptImpl::RemoveScriptSite(ILuaScriptEngineSite* pScriptSite)
{
	if(m_pLuaScriptEngineSite  == pScriptSite)
		m_pLuaScriptEngineSite = NULL;
	return 0;
}

long CLuaAcitveScriptImpl::Invoke(const std::wstring& strFuncName, const DISP_PARAMS& rdp, CLuaVariant& rvarRes)
{
	if(m_bException)
		return E_FAIL;

	try
	{
		return CLuaEngine::Invoke(strFuncName,rdp,rvarRes);
	}
	catch(...)
	{
		m_bException = true;
	}
	return E_FAIL;
}

long CLuaAcitveScriptImpl::Load(const std::wstring& strScriptName)
{
	if(m_bException)
		return E_FAIL;

	try
	{
		return Parse(strScriptName);		
	}
	catch(...)
	{
		m_bException = true;
	}
	return E_FAIL;
}

long CLuaAcitveScriptImpl::Open()
{
	m_bException = false;
	try
	{
		return CLuaEngine::OpenState();
	}
	catch(...)
	{
		m_bException = true;
	}
	return E_FAIL;
}

}}