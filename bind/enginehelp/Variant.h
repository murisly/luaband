#pragma once

#include "boost\variant.hpp"

class TVariant
{
	typedef boost::variant<int,char,short,std::string> var_t;
private:
	var_t m_var;
	bool m_bEmpty;
public:
	enum VAR_ENUM
	{
		VAR_EMPTY = -1,
		VAR_INT = 0,
		VAR_CHAR,
		VAR_SHORT,
		VAR_STRING,
	};
	TVariant()
	{
		m_bEmpty = true;
	}
	~TVariant()
	{}

	//同类型之间的赋值
	TVariant& operator=(const TVariant& varSrc)
	{
		if (this != &varSrc)
		{
			Clear();
			if (!varSrc.IsEmpty())
			{
				m_var = varSrc.m_var;
				m_bEmpty = false;
			}
		}
		return *this;
	}

	//整形赋值给类
	TVariant& operator=(int iSrc)
	{
		setValue<int>(iSrc);
		return *this;
	}

	TVariant& operator=(char cSrc)
	{
		setValue<char>(cSrc);
		return *this;
	}

	//类型转化运算符
 	operator int() const
 	{
 		return getValue<int>();
 	}

	operator char() const
	{
		return getValue<char>();
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

	VAR_ENUM getType() const
	{
		if (m_bEmpty)
			return  VAR_EMPTY;

		return (VAR_ENUM)m_var.which();
	}

	template<typename T>
	void setValue(const T& rValue)
	{
		Clear();
		m_var = rValue;
		m_bEmpty = false;
	}

	void Clear()
	{
		m_bEmpty = true;
	}
	bool IsEmpty() const
	{
		return m_bEmpty;
	}
};

class TestA
{
private:
	int a;
public:
	TestA(int i){a = i;};
	void SetValue(TestA& t)
	{
		a = t.a;
	};
};

class TestB
{
private:
	int b;
public:
	TestB(int i){b = i;};
};