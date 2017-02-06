#pragma once

#include <assert.h>
#include <string>
#include "boost/variant.hpp"
#include "interface/ILuaApi.h"

namespace binding{namespace lua{
//////////////////////////////////////////////////////////////////////////
// CLuaVariant
class CLuaVariant
{
	typedef boost::variant<int,char,short,long,__int64,unsigned int,unsigned char,unsigned short,unsigned long,unsigned __int64,float,double,bool,std::wstring,ILuaElementCollection*,ILuaApi*,LuaVarArray,std::vector<char>> var_t;
public:
	enum VAR_ENUM
	{
		VAR_EMPTY = -1,
		VAR_INT = 0,
		VAR_CHAR,
		VAR_SHORT,
		VAR_LONG,
		VAR_LONGLONG,
		VAR_UINT,
		VAR_UCHAR,
		VAR_USHORT,
		VAR_ULONG,
		VAR_ULONGLONG,
		VAR_FLOAT,
		VAR_DOUBLE,
		VAR_BOOL,
		VAR_WSTRING,
		VAR_ILUAEC,
		VAR_ILUAAPI,
		VAR_VARARRAY,
		VAR_BINARY
	};
	CLuaVariant()
	{
		m_bEmpty = true;
	}
	~CLuaVariant()
	{
		Clear();
	}
	CLuaVariant(const CLuaVariant& other)
	{
		m_bEmpty = true;
		if(!other.IsEmpty())
		{
			m_var = other.m_var;
			m_bEmpty = false;
			VAR_ENUM vType = getType();
			if(vType == VAR_ILUAEC)
			{
				ILuaElementCollection* pColl = boost::get<ILuaElementCollection*>(m_var);
				if(pColl)
					pColl->AddRef();
			}
		}
	}
	CLuaVariant(int iSrc)
	{
		setValue<int>(iSrc);
	}
	CLuaVariant(char cSrc)
	{
		setValue<char>(cSrc);
	}
	CLuaVariant(short nSrc)
	{
		setValue<short>(nSrc);
	}
	CLuaVariant(long nSrc)
	{
		setValue<long>(nSrc);
	}
	CLuaVariant(__int64 iSrc)
	{
		setValue<__int64>(iSrc);
	}
	CLuaVariant(unsigned int nSrc)
	{
		setValue<unsigned int>(nSrc);
	}
	CLuaVariant(unsigned char nSrc)
	{
		setValue<unsigned char>(nSrc);
	}
	CLuaVariant(unsigned short nSrc)
	{
		setValue<unsigned short>(nSrc);
	}
	CLuaVariant(unsigned long nSrc)
	{
		setValue<unsigned long>(nSrc);
	}
	CLuaVariant(unsigned __int64 nSrc)
	{
		setValue<unsigned __int64>(nSrc);
	}
	CLuaVariant(float fSrc)
	{
		setValue<float>(fSrc);
	}
	CLuaVariant(double dSrc)
	{
		setValue<double>(dSrc);
	}
	CLuaVariant(bool bSrc)
	{
		setValue<bool>(bSrc);
	}
	CLuaVariant(LPCWSTR lpcwSrc)
	{
		setValue<std::wstring>(lpcwSrc);
	}
	CLuaVariant(const std::wstring& strSrc)
	{
		setValue<std::wstring>(strSrc);
	}
	CLuaVariant(ILuaElementCollection* pSrc)
	{
		setValue<ILuaElementCollection*>(pSrc);
	}
	CLuaVariant(ILuaApi* pSrc)
	{
		setValue<ILuaApi*>(pSrc);
	}
	CLuaVariant(const LuaVarArray& arSrc)
	{
		setValue<LuaVarArray>(arSrc);
	}
	CLuaVariant(const std::vector<char>& vbSrc)
	{
		setValue<std::vector<char> >(vbSrc);
	}
	CLuaVariant& operator=(const CLuaVariant& varSrc)
	{
		if(this!=&varSrc)
		{
			Clear();
			if(!varSrc.IsEmpty())
			{
				m_var = varSrc.m_var;
				m_bEmpty = false;
				VAR_ENUM vType = getType();
				if(vType == VAR_ILUAEC)
				{
					ILuaElementCollection* pColl = boost::get<ILuaElementCollection*>(m_var);
					if(pColl)
						pColl->AddRef();
				}
			}
		}
		return *this;
	}
	CLuaVariant& operator=(int iSrc)
	{
		setValue<int>(iSrc);
		return *this;
	}
	CLuaVariant& operator=(char cSrc)
	{
		setValue<char>(cSrc);
		return *this;
	}
	CLuaVariant& operator=(short nSrc)
	{
		setValue<short>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(long nSrc)
	{
		setValue<long>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(__int64 iSrc)
	{
		setValue<__int64>(iSrc);
		return *this;
	}
	CLuaVariant& operator=(unsigned int nSrc)
	{
		setValue<unsigned int>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(unsigned char nSrc)
	{
		setValue<unsigned char>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(unsigned short nSrc)
	{
		setValue<unsigned short>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(unsigned long nSrc)
	{
		setValue<unsigned long>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(unsigned __int64 nSrc)
	{
		setValue<unsigned __int64>(nSrc);
		return *this;
	}
	CLuaVariant& operator=(float fSrc)
	{
		setValue<float>(fSrc);
		return *this;
	}
	CLuaVariant& operator=(double dSrc)
	{
		setValue<double>(dSrc);
		return *this;
	}
	CLuaVariant& operator=(bool bSrc)
	{
		setValue<bool>(bSrc);
		return *this;
	}
	CLuaVariant& operator=(LPCWSTR lpcwSrc)
	{
		setValue<std::wstring>(lpcwSrc);
		return *this;
	}
	CLuaVariant& operator=(const std::wstring& strSrc)
	{
		setValue<std::wstring>(strSrc);
		return *this;
	}
	CLuaVariant& operator=(ILuaElementCollection* pSrc)
	{
		setValue<ILuaElementCollection*>(pSrc);
		return *this;
	}
	CLuaVariant& operator=(ILuaApi* pSrc)
	{
		setValue<ILuaApi*>(pSrc);
		return *this;
	}
	CLuaVariant& operator=(const LuaVarArray& arSrc)
	{
		setValue<LuaVarArray>(arSrc);
		return *this;
	}

	CLuaVariant& operator=(const std::vector<char>& vbSrc)
	{
		setValue<std::vector<char> >(vbSrc);
		return *this;
	}

	operator int() const
	{
		return getValue<int>();
	}
	operator char() const
	{
		return getValue<char>();
	}
	operator short() const
	{
		return getValue<short>();
	}
	operator long() const
	{
		return getValue<long>();
	}
	operator __int64() const
	{
		return getValue<__int64>();
	}
	operator unsigned int() const
	{
		return getValue<unsigned int>();
	}
	operator unsigned char() const
	{
		return getValue<unsigned char>();
	}
	operator unsigned short() const
	{
		return getValue<unsigned short>();
	}
	operator unsigned long() const
	{
		return getValue<unsigned long>();
	}
	operator unsigned __int64() const
	{
		return getValue<unsigned __int64>();
	}
	operator float() const
	{
		return getValue<float>();
	}
	operator double() const
	{
		return getValue<double>();
	}
	operator bool() const
	{
		return getValue<bool>();
	}
	operator std::wstring() const
	{
		return getValue<std::wstring>();
	}
	operator ILuaElementCollection*() const
	{
		return getValue<ILuaElementCollection*>();
	}
	operator ILuaApi*() const
	{
		return getValue<ILuaApi*>();
	}
	operator LuaVarArray() const
	{
		return getValue<LuaVarArray>();
	}
	operator std::vector<char>() const
	{
		return getValue<std::vector<char> >();
	}
	bool operator==(const CLuaVariant& rhs) const
	{
		if (this->getType() != rhs.getType())
			return false;

		return m_var == rhs.m_var;
	}
	bool operator<(const CLuaVariant& rhs) const
	{
		if (this->getType() != rhs.getType())
			return this->getType() < rhs.getType();

		return m_var < rhs.m_var;
	}
	VAR_ENUM getType() const
	{
		if(m_bEmpty)
			return VAR_EMPTY;

		return (VAR_ENUM)m_var.which();
	}
	template<typename T>
	T getValue() const
	{
		if(!m_bEmpty && m_var.type() == typeid(T))
			return boost::get<T>(m_var);
		else
		{
			assert(0);
		}
		return T(0);
	}
	template<>
	std::wstring getValue() const
	{
		if(!m_bEmpty && m_var.type() == typeid(std::wstring))
			return boost::get<std::wstring>(m_var);
		else
		{
			assert(0);
		}
		return L"";
	}
	template<>
	bool getValue() const
	{
		if(!m_bEmpty)
		{
			if(m_var.type() == typeid(bool))
			{
				return boost::get<bool>(m_var);
			}
			else if(m_var.type() == typeid(int))
			{
				return boost::get<int>(m_var) != 0;
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			assert(0);
		}
		return false;
	}
	template<typename T>
	void setValue(const T& rValue)
	{
		Clear();
		m_var = rValue;
		m_bEmpty = false;
		VAR_ENUM vType = getType();
		if(vType == VAR_ILUAEC)
		{
			ILuaElementCollection* pColl = boost::get<ILuaElementCollection*>(m_var);
			if(pColl)
				pColl->AddRef();
		}
	}
	void Clear()
	{
		VAR_ENUM vType = getType();
		if(vType!= VAR_EMPTY)
		{
			if(vType == VAR_ILUAEC)
			{
				ILuaElementCollection* pColl = boost::get<ILuaElementCollection*>(m_var);
				if(pColl)
					pColl->Release();
			}
		}
		m_bEmpty = true;
	}
	bool IsEmpty() const
	{
		return m_bEmpty;
	}
private:
	var_t m_var;
	bool m_bEmpty;
};
}}