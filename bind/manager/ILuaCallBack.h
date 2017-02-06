#pragma once

#include <string>
#include <basetsd.h>
#include <windef.h>
#include "enginehelp/LuaVarint.h"

class ILuaCallBack
{
public:
	ILuaCallBack(){}
	virtual ~ILuaCallBack(){}

	virtual bool AddStartupResult(const std::wstring& strStartupQkey, int iApi = 0, int iFile = 0)
	{
		return true;
	}

	virtual bool AddScanFileResult(const std::wstring& strFilePath, bool bDir, bool bRedirect = true)
	{
		return true;
	}

	virtual bool AddScanRegResult(const std::wstring& strRegPath, bool bRedirect = true) 
	{
		return true;
	}

	virtual bool AddScanRegValueResult(const std::wstring& strRegPath, const std::wstring& strRegValueName, bool bRedirect = true) 
	{
		return true;
	}

	virtual bool GetAllStartups(std::vector<std::wstring>& startups,bool bIncludeDriver = false)
	{
		return true;
	}
	
	virtual void NotifySilenceTip(int iAdwCount,int iGarbageSize)
	{
	}
	
	virtual void DisableReportSuccess()
	{
	}

	void SetGuid( const std::wstring& GuiID )
	{
		m_wstrGuid = GuiID;
	}
	const std::wstring& GetGuid()const
	{
		return m_wstrGuid;
	}
protected:
	std::wstring m_wstrGuid;
};
