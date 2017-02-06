#pragma once

#include "ILuaApi.h"
#include "enginehelp/LuaVarint.h"
#include "memory/ref_counted.h"

template <class T> 
struct _APIDISP_MAP_ENTRY
{
	PCSTR pszName;
	long (T::*pfn)(const binding::lua::DISP_PARAMS&, binding::lua::CLuaVariant&);
	PCSTR pszArgFlags;
};

#define BEGIN_APIDISP_MAP(_class)\
	static const _APIDISP_MAP_ENTRY<_class>* GetApiDispMap(SIZE_T& nCount)\
{\
	static const _APIDISP_MAP_ENTRY<_class> dispmap[] = { { "", &DispDefault, NULL }, 

#define END_APIDISP_MAP() }; nCount = _countof(dispmap); return dispmap; };

#define APIDISP_ENTRY_EX(name, func, flags)  { name, func, flags },
#define APIDISP_ENTRY_F(name, flags)		APIDISP_ENTRY_EX(#name, &name, flags)
#define APIDISP_ENTRY(name)					APIDISP_ENTRY_EX(#name, &name, NULL)

#define DECLARE_LUAFUNC(name)			long name(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);

// i - 整数，u - unsigned整数, s - 字符串, d - double, U - ILuaElementCollection, * - 任意
#define API_PARAM_NULL			""
#define API_PARAM_INT			"i"
#define API_PARAM_UINT			"u"
#define API_PARAM_BOOL			"i"
#define API_PARAM_STRING		"s"
#define API_PARAM_DOUBLE		"d"
#define API_PARAM_ILUAELEMENTCOLLECTION	 "U"
#define API_PARAM_ANY			"*"

//////////////////////////////////////////////////////////////////////////
// CApiDispTraits
class CApiDispTraits
{
public:
	// 参数检查函数
	// i - 整数，u - unsigned整数, s - 字符串, d - double, U - ILuaElementCollection, b - 二进制数组,* - 任意
	static BOOL CheckDispParams(
		const binding::lua::DISP_PARAMS& rDispParams,
		PCSTR pszFlags)
	{
		if(pszFlags == NULL)
			return TRUE;
		PSTR p = (PSTR)pszFlags;
		for(size_t i = 0; i < rDispParams.size(); ++i, ++p)
		{
			switch(*p)
			{
			case '*':
				break;
			case 'i':
			case 'u':
				break;
			case 'd':
				if(rDispParams[i].getType() != binding::lua::CLuaVariant::VAR_DOUBLE)
					return FALSE;
				break;
			case 's':
				if( rDispParams[i].getType() != binding::lua::CLuaVariant::VAR_WSTRING )
				{
					return FALSE;
				}
				break;
			case 'U': // ILuaElementCollection
				if( rDispParams[i].getType() != binding::lua::CLuaVariant::VAR_ILUAEC ||
					rDispParams[i].getValue<binding::lua::ILuaElementCollection*>() == NULL )
				{
					return FALSE;
				}
				break;
			case 'b':
				if( rDispParams[i].getType() != binding::lua::CLuaVariant::VAR_BINARY )
				{
					return FALSE;
				}
				break;
			default:
				assert(FALSE && "CheckDispParams: 无效的参数标识");
			case '\0':
				return FALSE;
			}
		}
		//如果最后面是"*"则通过验证，但是这种情况只有最后是"*"才可以
		BOOL bOk = TRUE;
		while(*p != '\0')
		{
			if (*p != '*')
			{
				bOk = FALSE;
				break;
			}
			p++;
		}
		return bOk;
	}
};

//////////////////////////////////////////////////////////////////////////
// CApiDispImplRootT
template <class T>
class __declspec(novtable) CApiDispImplRootT 
	:public binding::lua::ILuaApi
	,public CApiDispTraits
	,public binding::RefCountedImpl<T>
{
public:
	static binding::lua::ILuaApi* GetInstance()
	{
		T* pImpl  = new T;
		pImpl->AddRef();
		return pImpl;
	}
	virtual ~CApiDispImplRootT()
	{
	}

	static const _APIDISP_MAP_ENTRY< CApiDispImplRootT<T> >* GetApiDispMap(SIZE_T& nCount)
	{
		nCount = 0;
		return NULL;
	}

	long DispDefault(const binding::lua::DISP_PARAMS&, binding::lua::CLuaVariant&)
	{
		return E_FAIL;
	}
public:
	// ILuaApi
	virtual void AddRef()
	{
		InternalAddRef();
	}

	virtual void Release()
	{
		InternalRelease();
	}

	virtual long Invoke(binding::lua::DISP_ID dispIdMember,const binding::lua::DISP_PARAMS& rDispParams,binding::lua::CLuaVariant& rVarResult)
	{
		SIZE_T nCount = 0;
		const _APIDISP_MAP_ENTRY<T>* pApiMap = T::GetApiDispMap(nCount);
		if(dispIdMember < 0 || dispIdMember >= (binding::lua::DISP_ID)nCount)
		{
			return DISP_E_MEMBERNOTFOUND;
		}
		if(!CheckDispParams(rDispParams, pApiMap[dispIdMember].pszArgFlags))
		{
			return DISP_E_TYPEMISMATCH;
		}
		T* pThis = (T*)this;
		return (pThis->*pApiMap[dispIdMember].pfn)(
			rDispParams,
			rVarResult);
	}
	virtual long GetDispTable(DispTable& table)
	{
		SIZE_T nCount = 0;
		const _APIDISP_MAP_ENTRY<T>* pApiMap = T::GetApiDispMap(nCount);
		for(SIZE_T index = 1 ; index < nCount ;  index++)
		{
			table.insert(DispTable::value_type(index,pApiMap[index].pszName));
		}
		return 0;
	}
};