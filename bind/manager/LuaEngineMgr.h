#pragma once

#include <string>
#include "enginehelp/LuaVarint.h"

#define		LUA_RETURN_SUCCESS		0x0001

class ILuaCallBack;
class CLuaEngineImpl;
class CLuaEngineWrapper;

class CLuaEngineMgr
{
	CLuaEngineMgr();
	~CLuaEngineMgr();
public:
	static CLuaEngineMgr* GetInstance();
	bool Excute(const std::vector<char>& vecBuff,ILuaCallBack* pCallBack,const std::wstring& strLuaGuid,const std::wstring& strCmd);
	bool Excute2(const std::vector<char>& vecBuff,ILuaCallBack* pCallBack,const std::wstring& strLuaGuid,const std::wstring& strCmd,binding::lua::CLuaVariant& rResult);
	void Uninit();
	int  GetReturn();
private:
	void _Init();
	CLuaEngineImpl* m_pLuaEngineImpl;
	CLuaEngineWrapper* m_pLuaEngineWrapper;
};