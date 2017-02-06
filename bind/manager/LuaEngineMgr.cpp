#include "stdafx.h"
#include "LuaEngineMgr.h"
#include "LuaEngineImpl.h"
#include "LuaEngineWrapper.h"

CLuaEngineMgr::CLuaEngineMgr()
{
	m_pLuaEngineImpl = NULL;
	m_pLuaEngineWrapper = NULL;
}

CLuaEngineMgr::~CLuaEngineMgr()
{
	if (m_pLuaEngineWrapper)
	{
		delete m_pLuaEngineWrapper;
		m_pLuaEngineWrapper = NULL;
	}
	if (m_pLuaEngineImpl)
	{
		m_pLuaEngineImpl->Release();
		m_pLuaEngineImpl = NULL;
	}
}

static CLuaEngineMgr* g_CLuaEngineMgr = NULL;

CLuaEngineMgr* CLuaEngineMgr::GetInstance()
{
	if(!g_CLuaEngineMgr)
	{
		g_CLuaEngineMgr = new CLuaEngineMgr;
		g_CLuaEngineMgr->_Init();
	}

	return g_CLuaEngineMgr;
}

bool CLuaEngineMgr::Excute(const std::vector<char>& vecBuff,ILuaCallBack* pCallBack,const std::wstring& strLuaGuid,const std::wstring& strCmd)
{
	CLuaVariant varResult;
	return Excute2(vecBuff,pCallBack,strLuaGuid,strCmd,varResult);
}

bool CLuaEngineMgr::Excute2(const std::vector<char>& vecBuff,ILuaCallBack* pCallBack,const std::wstring& strLuaGuid,const std::wstring& strCmd,CLuaVariant& rResult)
{
	bool bRet = false;

	if (m_pLuaEngineWrapper && m_pLuaEngineImpl)
	{
		if(m_pLuaEngineWrapper->OpenLuaBuff(vecBuff))
		{
			m_pLuaEngineImpl->AddLuaCallBack(strLuaGuid, pCallBack);
			bRet = m_pLuaEngineWrapper->Execute(strLuaGuid, strCmd,rResult);
			m_pLuaEngineImpl->RemoveLuaCallBack(strLuaGuid);
		}
	}

	return bRet;
}

void CLuaEngineMgr::_Init()
{
	if (!m_pLuaEngineWrapper)
	{
		m_pLuaEngineWrapper = new CLuaEngineWrapper;
		if(!m_pLuaEngineImpl)
		{
			m_pLuaEngineImpl = new CLuaEngineImpl();
			m_pLuaEngineImpl->AddRef();
		}

		m_pLuaEngineWrapper->Init(m_pLuaEngineImpl);
	}
}

void CLuaEngineMgr::Uninit()
{
	if (m_pLuaEngineWrapper)
	{
		m_pLuaEngineWrapper->Uninit();
	}

	g_CLuaEngineMgr = NULL;
	delete this;
}