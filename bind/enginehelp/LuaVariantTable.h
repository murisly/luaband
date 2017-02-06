#pragma once

#include "LuaVarint.h"
#include "memory\ref_counted.h"

namespace binding{namespace lua{
/////////////////////////////////////////////////////////
// CLuaElementCollectionlmplT
template< typename T>
class __declspec(novtable) CLuaElementCollectionlmplT
	:public ILuaElementCollection
	,public binding::RefCountedImpl<T>
{
public:
	void AddRef()
	{
		InternalAddRef();
	}
	void Release()
	{
		InternalRelease();
	}
	long get_count(size_t& pcnt)
	{
		assert(m_vKey.size() == m_vValue.size());
		pcnt = (long)m_vKey.size();
		return 1;
	}
	long get_key(size_t index, CLuaVariant& rvarKey)
	{
		assert(m_vKey.size() == m_vValue.size());
		if(index < 0 || index >= m_vKey.size())
			return 0;

		rvarKey = m_vKey[index];
		return 1;
	}
	long item(const CLuaVariant& varKey, size_t index, CLuaVariant& rvarRes)
	{
		assert(m_vKey.size() == m_vValue.size());
		if(varKey.IsEmpty())
		{
			if(index < 0 || index >= m_vValue.size())
				return 0;

			rvarRes = m_vValue[index];
			return 1;
		}
		else
		{
			for(size_t i = 0 ; i < m_vKey.size() ;  i ++)
			{
				if(m_vKey[i] == varKey)
				{
					rvarRes = m_vValue[i];
					return 1;
				}
			}
		}
		return 0;
	}
	long add(const CLuaVariant& varKey, const CLuaVariant& varValue)
	{
		m_vKey.push_back(varKey);
		m_vValue.push_back(varValue);
		return 1;
	}
protected:
	LuaVarArray m_vKey;
	LuaVarArray m_vValue;
};

//////////////////////////////////////////////////////////////////////////
// CLuaVariantTable
class  CLuaVariantTable
	: public CLuaElementCollectionlmplT<CLuaVariantTable>
{
public:
	template<class Interface>
	static void Create(Interface** ppv)
	{
		if(ppv == NULL)
			return;

		CLuaVariantTable* p = new CLuaVariantTable;
		*ppv = p;
		if(*ppv)
			(*ppv)->AddRef();
	}

	long Add(const CLuaVariant& varKey, const CLuaVariant& varValue)
	{
		return CLuaElementCollectionlmplT<CLuaVariantTable>::add(varKey,varValue);
	}
};
}}