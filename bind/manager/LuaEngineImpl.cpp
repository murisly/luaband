#include "stdafx.h"
#include "LuaEngineImpl.h"
#include "ILuaCallBack.h"
#include "interface/AutoPtrT.h"
#include "enginehelp/LuaVariantTable.h"

CLuaEngineImpl::CLuaEngineImpl()
{
}

CLuaEngineImpl::~CLuaEngineImpl()
{
}

long CLuaEngineImpl::OnEnterScript(void)
{
	return 0;
}

long CLuaEngineImpl::OnLeaveScript(void)
{
	return 0;
}

long CLuaEngineImpl::OnScriptError(const EXCEPINFO* pexcepinfo)
{
	std::wstring des  = pexcepinfo->bstrDescription;
	std::wstring luaid = pexcepinfo->bstrSource;
	
	luaid += L".excute.end.error";
	return 0;
}

long CLuaEngineImpl::OnScriptTerminate(const CLuaVariant& rvarRes, const EXCEPINFO *pexcepinfo)
{
	std::wstring des  = pexcepinfo->bstrDescription;
	std::wstring luaid = pexcepinfo->bstrSource;
	luaid += L".excute.end.terminate";
	return 0;
}

long CLuaEngineImpl::GetAllStartups(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::vector<std::wstring> v;
	CSmartPtr<CLuaVariantTable> pColl = NULL;
	CLuaVariantTable::Create(&pColl);
	if(pColl == NULL)
		return -1;

	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->GetAllStartups(v,rDispParams[0].getValue<bool>());
		for(const std::wstring& st : v)
		{
			CLuaVariant var;				
			var.setValue<std::wstring>(st);			 
			pColl->Add(CLuaVariant(),var);	
		}
	}
	rVarResult.setValue<ILuaElementCollection*>(pColl);
	return 0;
}

long CLuaEngineImpl::AddStartupResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->AddStartupResult(rDispParams[1].getValue<std::wstring>(), 
			rDispParams[2].getValue<int>(), 
			rDispParams[3].getValue<int>());
	}
	return 0;
}

long CLuaEngineImpl::AddScanFileResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->AddScanFileResult(rDispParams[1].getValue<std::wstring>(), 
			rDispParams[2].getValue<bool>(), 
			rDispParams[3].getValue<bool>());
	}
	return 0;
}

long CLuaEngineImpl::AddScanRegResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->AddScanRegResult(
			rDispParams[1].getValue<std::wstring>(), 
			rDispParams[2].getValue<bool>());
	}
	return 0;
}

long CLuaEngineImpl::AddScanRegValueResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	if (m_pLuaCallBack)
	{
		m_pLuaCallBack->AddScanRegValueResult(
			rDispParams[1].getValue<std::wstring>(),
			rDispParams[2].getValue<std::wstring>(), 
			rDispParams[3].getValue<bool>());
	}
	return 0;
}

bool CLuaEngineImpl::AddLuaCallBack(const std::wstring &wstrId, ILuaCallBack* pCallBack)
{
	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	m_pLuaCallBack = pCallBack;
	m_wstrId = wstrId;
	return true;
}

bool CLuaEngineImpl::RemoveLuaCallBack(const std::wstring &wstrId )
{
	binding::thread::Lock lock(m_Mutex_LuaCallBack);
	if ( wstrId == m_wstrId)
	{
		m_pLuaCallBack = NULL;
		return true;
	}
	return false;
}

long CLuaEngineImpl::Print(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring str = rDispParams[0];
	return 0;
}
