#include "stdafx.h"
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include "LuaDebug.h"
#include "LuaHelper.h"
#include "LuaVarint.h"
#include "strings\stringprintf.h"
#include "strings\StringsHelper.h"

namespace binding{namespace lua{

int ShowStack(lua_State* L)
{
#if (defined _DEBUG || defined _RELEASE_TEST)
	int iIndex = 0;
	lua_Debug debugInfo;
	std::wstring str(L"API参数传入错误详细信息:\n");
	while (lua_getstack(L, iIndex++, &debugInfo)) 
	{
		lua_getinfo(L, "Snl", &debugInfo);
		{
			binding::strings::AppendF(&str,L"%s File(%d): %s\n",
				binding::strings::s2ws(debugInfo.short_src).c_str(),
				debugInfo.currentline,
				debugInfo.name ? binding::strings::s2ws(debugInfo.name).c_str() : L"");
		}
	}
	_ASSERT_EXPR(0, str.c_str());
#endif // (defined _DEBUG || defined _RELEASE_TEST)
    return 1;
}

void DumpArgsInfo(std::wstring& str,const DISP_PARAMS& rParams)
{
	for(size_t i = 0;i < rParams.size();i++)
	{
		switch(rParams[i].getType())
		{
		case CLuaVariant::VAR_WSTRING:
			binding::strings::AppendF(&str,_T("参数%d(string) : %s\n"),i+1,rParams[i].getValue<std::wstring>().c_str());
			break;
		case CLuaVariant::VAR_INT:
			binding::strings::AppendF(&str,_T("参数%d(int) : %d\n"),i+1,rParams[i].getValue<int>());
			break;
		case CLuaVariant::VAR_BOOL:
			binding::strings::AppendF(&str,_T("参数%d(bool) : %s\n"),i+1,rParams[i].getValue<bool>()?_T("true") : _T("false"));
			break;
		case CLuaVariant::VAR_EMPTY:
			binding::strings::AppendF(&str,_T("参数%d(nil) : nil\n"),i+1);
			break;
		default:
			binding::strings::AppendF(&str,_T("参数%d(nil) : vt=%d\n"),i+1,rParams[i].getType());
			break;
		}
	}
}

void DebugInvokError(lua_State* L,HRESULT hr,const std::wstring& strScriptName,ILuaApi* pApi,DISP_ID dispid,const DISP_PARAMS& rParams)
{
#if (defined _DEBUG || defined _RELEASE_TEST)
	std::wstring strFuncName;
	if(pApi)
	{
		ILuaApi::DispTable table;
		pApi->GetDispTable(table);
		for(auto it : table)
		{
			if(dispid == it.first)
			{
				strFuncName = binding::strings::s2ws(it.second);
				break;
			}
		}
	}
 	if (DISP_E_TYPEMISMATCH == hr)
 	{	
		std::wstring str;
		binding::strings::AppendF(&str,_T("API函数传入参数错误脚本名称%s，函数名称: %s\r\n"),
			strScriptName.c_str(),
			strFuncName.c_str());
 		DumpArgsInfo(str, rParams);
 		_ASSERT_EXPR(0, str.c_str());
 		ShowStack(L);
 	}
	else if(0 && FAILED(hr) && hr != DISP_E_NONAMEDARGS)
	{
		std::wstring str;
		binding::strings::AppendF(&str,_T("API函数执行失败脚本名称%s，函数名称: %s\r\n错误码: %x\r\n"),
			strScriptName.c_str(),
			strFuncName.c_str(),
			hr);
		DumpArgsInfo(str, rParams);
		_ASSERT_EXPR(0, str.c_str());
	}
#endif // (defined _DEBUG || defined _RELEASE_TEST)
}

int ErrorHandle(lua_State* L)
{
#if (defined _DEBUG || defined RELEASE_TEST)
	const char* pszMessage = lua_tostring(L,-1);
	if (NULL != pszMessage)
	{			
		int iIndex = 0;
		lua_Debug debugInfo;
		std::string str;
		str += pszMessage;
		str += "\n";
		while (lua_getstack(L, iIndex++, &debugInfo)) 
		{
			lua_getinfo(L, "Snl", &debugInfo);
			{
				binding::strings::AppendF(&str,
					"%s(%d): %s\n",
					debugInfo.short_src[0] != 0 ? debugInfo.short_src : "<unknown file>",
					debugInfo.currentline,
					debugInfo.name ? debugInfo.name : "<unknown>");
			}
		}
		_ASSERT_EXPR(0, binding::strings::s2ws(str).c_str());
	}
#endif
	return 1;
}

#ifdef _DEBUG

#define DebugHelperReport	_CrtDbgReportW

#else // !defined _DEBUG

inline int DebugHelperReport(
	int /*_ReportType*/, 
	const wchar_t * _Filename, 
	__in int _LineNumber, 
	__in_z_opt const wchar_t * _ModuleName, 
	__in_z_opt const wchar_t * _Format, 
	...)
{
	if(_ModuleName == NULL)
		_ModuleName = _T("...");
	CAtlStringW str;
	str.Format( 
		L"Debug Assertion Failed!\n"
		L"                                                                                       \n"
		L"Program: %s\n"
		L"File: %s\n"
		L"Line: %d\n"
		L"\n"
		L"Message: ",
		_ModuleName,
		_Filename,
		_LineNumber);
	va_list ap;
	va_start(ap, _Format);
	str.AppendFormatV(_Format, ap);
	va_end(ap);
	str += 
		L"\n"
		L"(Press Retry to debug the application)\n"
		L"\n";
	int res = ::MessageBoxW(
		NULL,
		str,
		L"Debug Assertion Break",
		MB_ICONEXCLAMATION|MB_ABORTRETRYIGNORE);
	switch(res)
	{
	case IDABORT:
		ExitProcess(IDABORT);
		break;
	case IDRETRY:
		break;
	case IDIGNORE:
		break;
	}
	return res;
}

#endif // #ifdef _DEBUG


#if (defined _DEBUG || defined _RELEASE_TEST)
//////////////////////////////////////////////////////////////////////////
// CLuaStackDebugCheck
CLuaStackDebugCheck::CLuaStackDebugCheck(
	lua_State* L, 
	const char* func,
	const char* file,
	int line)
	: m_L(L)
	, m_pszFunc(func)
	, m_pszFile(file)
	, m_iLine(line)
{
	m_top = lua_gettop(L);
}

CLuaStackDebugCheck::~CLuaStackDebugCheck(void)
{
	int top = lua_gettop(m_L);
	if(top != m_top)
	{
		DebugHelperReport(
			_CRT_ASSERT,
			CAtlStringW(m_pszFile),
			m_iLine,
			NULL,
			L"在调用函数%s过程中检测到Lua栈泄漏。\n"
			L"调用函数前top %d，调用函数后top %d。",
			CAtlStringW(m_pszFunc),
			m_top,
			top);
	}
}

#endif // (defined _DEBUG || defined _RELEASE_TEST)

}}