#pragma once

template <class T>
class _NoAddRefReleaseOnCSmartPtr : public T
{
private:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
};

template <class T>
class CSmartPtr
{
protected:
	void Swap(CSmartPtr& other)
	{
		T* pTemp = p;
		p = other.p;
		other.p = pTemp;
	}
public:
	CSmartPtr() throw()
	{
		p = NULL;
	}
	CSmartPtr(T* lp) throw()
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
	CSmartPtr(const CSmartPtr<T>& lp) throw() :
		CSmartPtr<T>(lp.p)
	{	
	}
	CSmartPtr(CSmartPtr<T>&& lp) throw()
	{	
		lp.Swap(*this);
	}
	typedef T _PtrClass;
	~CSmartPtr() throw()
	{
		if (p)
			p->Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		ATLENSURE(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		return &p;
	}
	_NoAddRefReleaseOnCSmartPtr<T>* operator->() const throw()
	{
		return (_NoAddRefReleaseOnCSmartPtr<T>*)p;
	}
	bool operator!() const throw()
	{	
		return (p == NULL);
	}
	bool operator<(T* pT) const throw()
	{
		return p < pT;
	}
	bool operator!=(T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(T* pT) const throw()
	{
		return p == pT;
	}

	T* operator=(T* lp) throw()
	{
		if(*this!=lp)
		{
			CSmartPtr(lp).Swap(*this);
		}
		return *this;
	}
	T* operator=(const CSmartPtr<T>& lp) throw()
	{
		if(*this!=lp)
		{
			CSmartPtr(lp).Swap(*this);
		}
		return *this;
	}	
	T* operator=(CSmartPtr<T>&& lp) throw()
	{			
		if (*this != lp)
		{
			CSmartPtr(static_cast<CSmartPtr&&>(lp)).Swap(*this);
		}
		return *this;		
	}
	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}

	// Attach to an existing interface (does not AddRef)
	void Attach(T* p2) throw()
	{
		if (p)
		{
			p->Release();
		}
		p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	bool CopyTo(T** ppT) throw()
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return false;
		*ppT = p;
		if (p)
			p->AddRef();
		return true;
	}
	T* p;
};