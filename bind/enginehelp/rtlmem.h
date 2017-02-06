#pragma once

#include "rtlbase.h"

NAMESPACE_RTL_BEGIN

//////////////////////////////////////////////////////////////////////////
// CRtlCRTAllocator
class CRtlCRTAllocator 
{
public:
	static PVOID Allocate(
		SIZE_T nBytes)
	{
		return malloc(nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes)
	{
		return realloc(p, nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes)
	{
		return realloc(p, nBytes);
	}

	static VOID Free(
		PVOID p)
	{
		free(p);
		p = NULL ;
	}

	static VOID Free(
		PVOID p,
		SIZE_T)
	{
		free(p);
		p = NULL ;
	}

#if (defined _DEBUG)
	static PVOID Allocate(
		SIZE_T nBytes,
		PCSTR lpszFile,
		INT nLine)
	{
		return _malloc_dbg(nBytes, _NORMAL_BLOCK, lpszFile, nLine);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		PCSTR lpszFile,
		INT nLine)
	{
		return _realloc_dbg(p, nBytes, _NORMAL_BLOCK, lpszFile, nLine);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes,
		PCSTR lpszFile,
		INT nLine)
	{
		return _realloc_dbg(p, nBytes, _NORMAL_BLOCK, lpszFile, nLine);
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
// CRtlLocalAllocator
class CRtlLocalAllocator
{
public:
	static PVOID Allocate(
		SIZE_T nBytes)
	{
		return ::LocalAlloc(
			LMEM_FIXED,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes)
	{
		return ::LocalReAlloc(
			p,
			nBytes,
			0);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes)
	{
		return ::LocalReAlloc(
			p,
			nBytes,
			0);
	}

	static VOID Free(
		PVOID p)
	{
		::LocalFree(p);
	}

	static VOID Free(
		PVOID p,
		SIZE_T)
	{
		::LocalFree(p);
	}

#if (defined _DEBUG)
	static PVOID Allocate(
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::LocalAlloc(
			LMEM_FIXED,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::LocalReAlloc(
			p,
			nBytes,
			0);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::LocalReAlloc(
			p,
			nBytes,
			0);
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
// CRtlGlobalAllocator
class CRtlGlobalAllocator
{
public:
	static PVOID Allocate(
		SIZE_T nBytes)
	{
		return ::GlobalAlloc(
			GMEM_FIXED,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes)
	{
		return ::GlobalReAlloc(
			p,
			nBytes,
			0);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes)
	{
		return ::GlobalReAlloc(
			p,
			nBytes,
			0);
	}

	static VOID Free(
		PVOID p)
	{
		::GlobalFree(p);
	}

	static VOID Free(
		PVOID p,
		SIZE_T)
	{
		::GlobalFree(p);
	}

#if (defined _DEBUG)
	static PVOID Allocate(
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::GlobalAlloc(
			GMEM_FIXED,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		PCSTR, INT)
	{
		return ::GlobalReAlloc(
			p,
			nBytes,
			0);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::GlobalReAlloc(
			p,
			nBytes,
			0);
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
// CRtlProcessHeapAllocator
class CRtlProcessHeapAllocator
{
public:
	static PVOID Allocate(
		SIZE_T nBytes)
	{
		return ::HeapAlloc(
			GetProcessHeap(),
			0,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes)
	{
		return ::HeapReAlloc(
			GetProcessHeap(),
			0,
			p,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes)
	{
		return ::HeapReAlloc(
			GetProcessHeap(),
			0,
			p,
			nBytes);
	}

	static VOID Free(
		PVOID p)
	{
		HeapFree(
			GetProcessHeap(),
			0,
			p);
	}

	static VOID Free(
		PVOID p,
		SIZE_T)
	{
		HeapFree(
			GetProcessHeap(),
			0,
			p);
	}

#if (defined _DEBUG)
	static PVOID Allocate(
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::HeapAlloc(
			GetProcessHeap(),
			0,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		PCSTR, INT)
	{
		return ::HeapReAlloc(
			GetProcessHeap(),
			0,
			p,
			nBytes);
	}

	static PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return ::HeapReAlloc(
			GetProcessHeap(),
			0,
			p,
			nBytes);
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
// CRtlHeapAllocator
class CRtlHeapAllocator
{
public:
	HANDLE m_hHeap;

public:
	CRtlHeapAllocator(VOID)
		: m_hHeap(GetProcessHeap())
	{
	}

	CRtlHeapAllocator(
		SIZE_T dwInitialSize,
		SIZE_T dwMaximumSize,
		DWORD flOptions = 0)
	{
		BOOL bResult;
		bResult = Create(
			dwInitialSize,
			dwMaximumSize,
			flOptions);
		if(!bResult)
		{
			AtlThrow(
				HRESULT_FROM_WIN32(GetLastError()));
		}
	}

	~CRtlHeapAllocator(VOID)
	{
		Destroy();
	}

	BOOL Create(
		SIZE_T dwInitialSize = 0,
		SIZE_T dwMaximumSize = 0,
		DWORD flOptions = 0)
	{
		Destroy();
		m_hHeap = HeapCreate(
			flOptions,
			dwInitialSize,
			dwMaximumSize);
		return (NULL != m_hHeap);
	}

	BOOL Destroy(
		VOID)
	{
		BOOL bResult = TRUE;
		if( (m_hHeap != NULL) &&
			(m_hHeap != GetProcessHeap()))
		{
			bResult = HeapDestroy(m_hHeap);
			m_hHeap = NULL;
		}
		return bResult;
	}

	PVOID Allocate(
		SIZE_T nBytes)
	{
		return HeapAlloc(
			m_hHeap,
			0,
			nBytes);
	}

	PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes)
	{
		return HeapReAlloc(
			m_hHeap,
			0,
			p,
			nBytes);
	}

	PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes)
	{
		return HeapReAlloc(
			m_hHeap,
			0,
			p,
			nBytes);
	}

	VOID Free(
		PVOID p)
	{
		HeapFree(
			m_hHeap,
			0,
			p);
	}

	VOID Free(
		PVOID p,
		SIZE_T)
	{
		HeapFree(
			m_hHeap,
			0,
			p);
	}

#if (defined _DEBUG)
	PVOID Allocate(
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return Allocate(
			nBytes);
	}
	PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return Reallocate(
			p,
			nBytes);
	}

	PVOID Reallocate(
		PVOID p,
		SIZE_T,
		SIZE_T nBytes,
		PCSTR,
		INT)
	{
		return Reallocate(
			p,
			nBytes);
	}
#endif
};

typedef struct CHAIN { CHAIN* pNext; } *PCHAIN;
//////////////////////////////////////////////////////////////////////////
// CRtlLFAllocator - Low-fragmentation allocator
template< INT ALIGN_SHIFT = 3, INT MAX_BYTES = 128, INT MIN_BYTES = 0 >
class CRtlLFAllocatorTraits
{
public:
	enum
	{
		_MIN_BYTES = MIN_BYTES,
		_MAX_BYTES = MAX_BYTES,
		_ALIGN_SHIFT = ALIGN_SHIFT,
		_ALIGN = ( 1 << _ALIGN_SHIFT )
	};

	static SIZE_T S_Roundup(
		SIZE_T nBytes)
	{
		return _roundup(
			nBytes,
			_ALIGN);
	}
};

template<
	class Alloc = CRtlCRTAllocator,
	class Traits = CRtlLFAllocatorTraits<>,
	class Lock = CComFakeCriticalSection
>
class CRtlLFAllocator
{
public:
	enum
	{
		_MIN_BYTES = Traits::_MIN_BYTES,
		_MAX_BYTES = Traits::_MAX_BYTES,
		_ALIGN_SHIFT = Traits::_ALIGN_SHIFT,
		_ALIGN = ( 1 << _ALIGN_SHIFT ),
		_FREE_LISTS = ( (_MAX_BYTES >> _ALIGN_SHIFT) - (_MIN_BYTES >> _ALIGN_SHIFT) )
	};

protected:
	Lock	m_Lock;
	PCHAIN	m_ppFreeList[_FREE_LISTS];
	SIZE_T	m_nHeapSize;
	PSTR	m_pStartFree;
	PSTR	m_pEndFree;

protected:
	SIZE_T GetFreeListIndex(
		SIZE_T nBytes)
	{
		ATLASSERT(nBytes > _MIN_BYTES && nBytes <= _MAX_BYTES);
		return( ((nBytes - 1)>>_ALIGN_SHIFT) - (_MIN_BYTES >> _ALIGN_SHIFT) );
	}

	PVOID Refill(
		SIZE_T nBytes)
	{
		INT nObjs;
		PSTR pChunk;
		PCHAIN pResult;
		PCHAIN pNextObj;
		PCHAIN pCurrentObj;
		PCHAIN* ppFreeList;

		nObjs = 20;
		nBytes = Traits::S_Roundup(nBytes);
		pChunk = ChunkAlloc(
			nBytes,
			nObjs);

		if(1 == nObjs || pChunk == NULL)
		{
			return pChunk;
		}

		pResult = (PCHAIN)pChunk;
		ppFreeList = m_ppFreeList + GetFreeListIndex(nBytes);
		*ppFreeList = pNextObj = (PCHAIN)(pChunk + nBytes);

		for(INT i = 1; ; ++i)
		{
			pCurrentObj = pNextObj;
			pNextObj = (PCHAIN)((PSTR)pNextObj + nBytes);
			if(nObjs - 1 == i)
			{
				pCurrentObj->pNext = NULL;
				break;
			}
			else
			{
				pCurrentObj->pNext = pNextObj;
			}
		}
		return pResult;
	}

	PSTR ChunkAlloc(
		SIZE_T nSize,
		INT& nObjs)
	{
		PSTR pResult;
		SIZE_T nBytesLeft;
		SIZE_T nTotalBytes;

		nTotalBytes = nSize * nObjs;
		nBytesLeft = m_pEndFree - m_pStartFree;
		if(nBytesLeft >= nTotalBytes)
		{
			pResult = m_pStartFree;
			m_pStartFree += nTotalBytes;
			return pResult;
		}
		else if(nBytesLeft >= nSize)
		{
			nObjs = (INT)(nBytesLeft / nSize);
			nTotalBytes = nSize * nObjs;
			pResult = m_pStartFree;
			m_pStartFree += nTotalBytes;
			return pResult;
		}
		else
		{
			SIZE_T nBytesToGet =
				Traits::S_Roundup(2 * nTotalBytes + (m_nHeapSize >> 4));
			if(nBytesLeft >= _MIN_BYTES + _ALIGN)
			{
				PCHAIN* ppFreeList;
				ppFreeList = m_ppFreeList + GetFreeListIndex(nBytesLeft);
				((PCHAIN)m_pStartFree)->pNext = *ppFreeList;
				*ppFreeList = (PCHAIN)m_pStartFree;
			}
			m_pStartFree = (PSTR)Alloc::Allocate(nBytesToGet);
			if(m_pStartFree == NULL)
			{
				PCHAIN p;
				PCHAIN* ppFreeList;
				for(SIZE_T i = nSize;
					i <= (SIZE_T)_MAX_BYTES; 
					i += (SIZE_T)_ALIGN)
				{
					ppFreeList = m_ppFreeList + GetFreeListIndex(i);
					p = *ppFreeList;
					if(p != NULL)
					{
						*ppFreeList = p->pNext;
						m_pStartFree = (PSTR)p;
						m_pEndFree = m_pStartFree + i;
						return( ChunkAlloc(nSize, nObjs) );
					}
				}
				m_pEndFree = NULL;
				return NULL;
			}
			m_nHeapSize += nBytesToGet;
			m_pEndFree = m_pStartFree + nBytesToGet;
			return( ChunkAlloc(nSize, nObjs) );
		}
	}

	PVOID _Allocate(
		SIZE_T nBytes)
	{
		PVOID p;
		PCHAIN* ppFreeList = m_ppFreeList + GetFreeListIndex(nBytes);
		m_Lock.Lock();
		p = *ppFreeList;
		if(p != NULL)
		{
			*ppFreeList = ((PCHAIN)p)->pNext;
		}
		else
		{
			p = Refill(nBytes);
		}
		m_Lock.Unlock();
		return p;
	}

	VOID _Free(
		PVOID p,
		SIZE_T nBytes)
	{
		PCHAIN* ppFreeList = m_ppFreeList + GetFreeListIndex(nBytes);
		m_Lock.Lock();
		((PCHAIN)p)->pNext = *ppFreeList;
		*ppFreeList = (PCHAIN)p;
		m_Lock.Unlock();
	}

public:
	CRtlLFAllocator(
		VOID)
	{
		HRESULT hr = m_Lock.Init();
		if(FAILED(hr))
			AtlThrow(hr);
		ATLASSERT(_MIN_BYTES % _ALIGN == 0);
		ATLASSERT(_MAX_BYTES % _ALIGN == 0);
		ATLASSERT(_MIN_BYTES < _MAX_BYTES);
		m_nHeapSize = 0;
		m_pStartFree = m_pEndFree = NULL;
		ZeroMemory(
			m_ppFreeList,
			sizeof(m_ppFreeList));
	}

	CRtlLFAllocator(
		SIZE_T nInitSize)
	{
		HRESULT hr = CComCriticalSection::Init();
		if(FAILED(hr))
			AtlThrow(hr);
		ZeroMemory(
			m_ppFreeList,
			sizeof(m_ppFreeList));
		nInitSize = Traits::S_Roundup(nInitSize);
		m_pStartFree = (PSTR)Alloc::Allocate(
			nInitSize);
		if(m_pStartFree != NULL)
		{
			m_nHeapSize = nInitSize;
			m_pEndFree = m_pStartFree + nInitSize;
		}
		else
		{
			m_nHeapSize = 0;
			m_pEndFree = NULL;
		}
	}

	~CRtlLFAllocator(
		VOID)
	{
		m_Lock.Term();
	}

	PVOID Allocate(
		SIZE_T nBytes)
	{
		ATLASSERT(nBytes > (SIZE_T)_MIN_BYTES);
		if(nBytes > (SIZE_T)_MAX_BYTES)
		{
			return Alloc::Allocate(
				Traits::S_Roundup(nBytes));
		}
		else
		{
			return _Allocate(
				nBytes);
		}
	}

	PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		SIZE_T nNewBytes)
	{
		PVOID pMem;
		ATLASSERT(nBytes > (SIZE_T)_MIN_BYTES);
		if( (nBytes > _MAX_BYTES) &&
			(nNewBytes > _MAX_BYTES) )
		{
			return Alloc::Reallocate(
				p,
				Traits::S_Roundup(nBytes),
				Traits::S_Roundup(nNewBytes));
		}
		pMem = Allocate(nNewBytes);
		if(pMem && p)
		{
			CopyMemory(
				pMem,
				p,
				nBytes);
		}
		if(p)
		{
			Free(
				p,
				nBytes);
		}
		return pMem;
	}

	VOID Free(
		PVOID p,
		SIZE_T nBytes)
	{
		ATLASSERT(nBytes > (SIZE_T)_MIN_BYTES);
		if(nBytes > (SIZE_T)_MAX_BYTES)
		{
			Alloc::Free(
				p,
				Traits::S_Roundup(nBytes));
		}
		else
		{
			_Free(p, nBytes);
		}
	}

#if (defined _DEBUG)
	PVOID Allocate(
		SIZE_T nBytes,
		PCSTR lpszFile,
		INT nLine)
	{
		if(nBytes > (SIZE_T)_MAX_BYTES)
		{
			return Alloc::Allocate(
				Traits::S_Roundup(nBytes),
				lpszFile,
				nLine);
		}
		else
		{
			return _Allocate(
				nBytes);
		}
	}

	PVOID Reallocate(
		PVOID p,
		SIZE_T nBytes,
		SIZE_T nNewBytes,
		PCSTR lpszFile,
		INT nLine)
	{
		PVOID pMem;
		if( (nBytes > _MAX_BYTES) &&
			(nNewBytes > _MAX_BYTES) )
		{
			return Alloc::Reallocate(
				p,
				Traits::S_Roundup(nBytes),
				Traits::S_Roundup(nNewBytes),
				lpszFile,
				nLine);
		}
		pMem = Allocate(
			nNewBytes,
			lpszFile,
			nLine);
		if(pMem && p)
		{
			CopyMemory(pMem,
				p,
				nBytes);
		}
		if(p)
		{
			Free(p,
				nBytes);
		}
		return pMem;
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
// CRtlHeapPoolT
template < class Alloc = CRtlCRTAllocator >
class CRtlHeapPoolT
{
protected:
	const SIZE_T m_nBlockSize;
	PCHAIN m_pDataChain;
	SIZE_T m_nBufferSize;
	PVOID m_pBuffer;

public:
	CRtlHeapPoolT(
		SIZE_T nBlockSize = 4096)
		: m_nBlockSize(_roundup(nBlockSize, 1024))
		, m_pDataChain(NULL)
		, m_nBufferSize(0)
		, m_pBuffer(NULL)
	{
		ATLASSERT(m_nBlockSize > 0);
	}
	
	~CRtlHeapPoolT(
		VOID)
	{
		FreeDataChain();
	}

	VOID FreeDataChain(
		VOID)
	{
		PCHAIN pKill;
		PCHAIN pNext = m_pDataChain;
		while(pNext)
		{
			pKill = pNext;
			pNext = pNext->pNext;
			Alloc::Free(pKill);
		}
		m_pDataChain = NULL;
		m_nBufferSize = 0;
		m_pBuffer = NULL;
	}

	PVOID Allocate(
		SIZE_T nBytes)
	{
		if(m_nBufferSize < nBytes)
		{
			SIZE_T nAllocSize = max(m_nBlockSize, nBytes + sizeof(CHAIN));
			nAllocSize = _roundup(nAllocSize, 1024);
			PCHAIN pBlock = (PCHAIN)Alloc::Allocate(
				nAllocSize);
			if(pBlock == NULL)
				return NULL;
			pBlock->pNext = m_pDataChain;
			m_pDataChain = pBlock;
			m_pBuffer = (pBlock + 1);
			m_nBufferSize = nAllocSize - sizeof(CHAIN);
		}
		ATLASSERT(m_nBufferSize >= nBytes);
		PVOID p = m_pBuffer;
		m_pBuffer = (PBYTE)m_pBuffer + nBytes;
		m_nBufferSize -= nBytes;
		return p;
	}
};

//////////////////////////////////////////////////////////////////////////
// CRtlBlkAllocator
template < class Alloc = CRtlCRTAllocator >
class CRtlBlkAllocator
{
protected:
	const SIZE_T m_nBlockSize;
	SIZE_T m_nBlockCount;
	PCHAIN m_pDataChain;
	PCHAIN m_pFree;

public:
	CRtlBlkAllocator(
		SIZE_T nBlockSize,
		SIZE_T nBlockCount)
		: m_pFree(NULL)
		, m_pDataChain(NULL)
		, m_nBlockSize(max(nBlockSize, sizeof(CHAIN)))
	{
		SetBlockCount(
			nBlockCount);
	}

	~CRtlBlkAllocator(
		VOID)
	{
		FreeDataChain();
	}

	PVOID Allocate(
		VOID)
	{
		PCHAIN pBlock;
		if(m_pFree == NULL)
		{
			SIZE_T nAllocSize;
			nAllocSize = m_nBlockSize * m_nBlockCount + sizeof(CHAIN);
			pBlock = (PCHAIN)Alloc::Allocate(
				nAllocSize);
			if(pBlock == NULL)
			{
				AtlThrow(ENOMEM);
			}
			pBlock->pNext = m_pDataChain;
			m_pDataChain = pBlock++;
			for(SIZE_T i = 0; i < m_nBlockCount; ++i)
			{
				pBlock->pNext = m_pFree;
				m_pFree	= pBlock;
				pBlock = (PCHAIN)((PBYTE)pBlock + m_nBlockSize);
			}
		}
		pBlock = m_pFree;
		m_pFree = m_pFree->pNext;
		return (PVOID)pBlock;
	}

	PVOID Allocate(
		SIZE_T nSize)
	{
		ATLASSERT(nSize <= m_nBlockSize);
		return Allocate();
	}

	VOID Free(
		PVOID p)
	{
		PCHAIN pBlock = (PCHAIN)p;
		pBlock->pNext = m_pFree;
		m_pFree	= pBlock;
	}

	VOID Free(
		PVOID p,
		SIZE_T nSize)
	{
		ATLASSERT(nSize <= m_nBlockSize);
		return Free(p);
	}

	VOID FreeDataChain(
		VOID)
	{
		PCHAIN pKill;
		PCHAIN pNext = m_pDataChain;
		while(pNext)
		{
			pKill = pNext;
			pNext = pNext->pNext;
			Alloc::Free(pKill);
		}
		m_pDataChain = NULL;
		m_pFree = NULL;
	}

	SIZE_T SetBlockCount(
		SIZE_T nBlockCount)
	{
		SIZE_T nAllocSize;
		nAllocSize = m_nBlockSize * nBlockCount + sizeof(CHAIN);
		m_nBlockCount = (nAllocSize - sizeof(CHAIN)) / m_nBlockSize;
		return m_nBlockCount;
	}

	SIZE_T GetBlockSize(
		VOID) const
	{
		return m_nBlockSize;
	}

#if (defined _DEBUG)
	PVOID Allocate(
		PCSTR,
		INT)
	{
		return Allocate();
	}

	PVOID Allocate(
		SIZE_T nSize,
		PCSTR,
		INT)
	{
		return Allocate(nSize);
	}
#endif
};

#pragma push_macro("new")
#undef new
//////////////////////////////////////////////////////////////////////////
// CRtlTypeAllocator
template < class T, class Alloc = CRtlCRTAllocator >
class CRtlTypeAllocator : public CRtlBlkAllocator< Alloc >
{
public:
	CRtlTypeAllocator(
		SIZE_T nBlockCount = 10)
		: CRtlBlkAllocator< Alloc >(sizeof(T), nBlockCount)
	{
	}
		
	T* Allocate(
		VOID)
	{
		T* p = (T*)CRtlBlkAllocator< Alloc >::Allocate( sizeof(T) );
		::new(p) T;
		return p;
	}

	T* Allocate(
		SIZE_T nSize)
	{
		ATLASSERT(nSize <= m_nBlockSize);
		return Allocate();
	}

	VOID Free(
		T* p)
	{
		p->T::~T();
		CRtlBlkAllocator< Alloc >::Free(p);
	}

	VOID Free(
		T* p,
		SIZE_T nSize)
	{
		ATLASSERT(nSize <= m_nBlockSize);
		return Free(p);
	}

#if (defined _DEBUG)
	PVOID Allocate(
		PCSTR,
		INT)
	{
		return Allocate();
	}

	PVOID Allocate(
		SIZE_T nSize,
		PCSTR,
		INT)
	{
		return Allocate(nSize);
	}
#endif
};
#pragma pop_macro("new")

//////////////////////////////////////////////////////////////////////////
// CRtlMemFileT
template < class Alloc = CRtlCRTAllocator, ULONG nGrowupShift = 3 >
class CRtlMemFileT
{
protected:
	Alloc m_allocator;
	PBYTE  m_pBuffer;
	SIZE_T m_nBufSize;
	SIZE_T m_nFileSize;
	SIZE_T m_nPosition;

public:
	SIZE_T m_nGrowBy;

public:
	CRtlMemFileT(
		SIZE_T nGrowBy = 128)
		: m_nGrowBy(nGrowBy)
		, m_pBuffer(NULL)
		, m_nFileSize(0)
		, m_nPosition(0)
		, m_nBufSize(0)
	{
	}

	~CRtlMemFileT(
		VOID)
	{
		Close();
	}

	VOID Close(
		VOID)
	{
		if(m_pBuffer)
		{
			m_allocator.Free(
				m_pBuffer,
				m_nBufSize);
			m_pBuffer = NULL;
			m_nFileSize = 0;
			m_nPosition = 0;
			m_nBufSize = 0;
		}
	}

	PVOID GetData(
		VOID)
	{
		return m_pBuffer;
	}

	SIZE_T GetDataSize(
		VOID)
	{
		return m_nFileSize;
	}

	SIZE_T GetPosition(
		VOID)
	{
		return m_nPosition;
	}

	HRESULT Create(
		LPCTSTR szFilename,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		LPSECURITY_ATTRIBUTES lpsa = NULL,
		HANDLE hTemplateFile = NULL)
	{
		return E_NOTIMPL;
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		DWORD* pBytesRead = NULL)
	{
		DWORD dwBytesRead = 0;
		if(m_nPosition <= m_nFileSize)
		{
			if(nBufSize > (DWORD)(m_nFileSize - m_nPosition))
			{
				dwBytesRead = (DWORD)(m_nFileSize - m_nPosition);
			}
			else
			{
				dwBytesRead = nBufSize;
			}
			if(dwBytesRead > 0)
			{
				CopyMemory(
					pBuffer,
					m_pBuffer + m_nPosition,
					dwBytesRead);
				m_nPosition += dwBytesRead;
			}
		}
		if(pBytesRead)
		{
			*pBytesRead = dwBytesRead;
		}
		return( dwBytesRead == nBufSize ? S_OK : S_FALSE );
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		DWORD& nBytesRead)
	{
		return Read(
			pBuffer,
			nBufSize,
			&nBytesRead);
	}

	template< typename T >
	BOOL Read(
		T& value)
	{
		return Read(
			&value,
			sizeof(T));
	}

	template < typename T >
	HRESULT Write(
		T& value)
	{
		return Write(
			&value,
			sizeof(T));
	}

	template < typename T >
	HRESULT Write2(
		T value)
	{
		return Write(
			&value,
			sizeof(T));
	}

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		DWORD* pnBytesWritten = NULL)
	{
		HRESULT hr = GrowBuffer(
			m_nPosition + nBufSize);
		if(FAILED(hr))
		{
			return hr;
		}
		ATLASSERT(m_nPosition + nBufSize <= m_nBufSize);
		CopyMemory(
			m_pBuffer + m_nPosition,
			pBuffer,
			nBufSize);
		m_nPosition += nBufSize;
		if(m_nPosition > m_nFileSize)
		{
			m_nFileSize = m_nPosition;
		}
		if(pnBytesWritten)
			*pnBytesWritten = nBufSize;
		return S_OK;
	}

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		DWORD& dwBytesWritten)
	{
		return Write(
			pBuffer,
			nBufSize,
			&dwBytesWritten);
	}

	HRESULT Seek(
		LARGE_INTEGER dlibMove,
		DWORD dwFrom = SEEK_CUR,
		ULARGE_INTEGER *plibNewPosition = NULL)
	{
		ATLASSERT(
			dwFrom == SEEK_SET ||
			dwFrom == SEEK_END ||
			dwFrom == SEEK_CUR);
		LONGLONG lNewPos;
		if (dwFrom == SEEK_SET)
		{
			lNewPos = 0;
		}
		else if(dwFrom == SEEK_CUR)
		{
			lNewPos = m_nPosition;
		}
		else if(dwFrom == SEEK_END)
		{
			lNewPos = m_nFileSize;
		}
		else
		{
			return E_INVALIDARG;
		}
		lNewPos += dlibMove.QuadPart;
		m_nPosition = (SIZE_T)lNewPos;
		if(plibNewPosition)
			plibNewPosition->QuadPart = lNewPos;
		return S_OK;
	}

	HRESULT Seek(
		LONGLONG nOffset,
		DWORD dwFrom = SEEK_CUR,
		ULONGLONG* pnNewPosition = NULL)
	{
		LARGE_INTEGER li;
		ULARGE_INTEGER uli;
		li.QuadPart = nOffset;
		uli.QuadPart = 0;
		HRESULT hr = Seek(
			li,
			dwFrom,
			&uli);
		if(SUCCEEDED(hr))
		{
			if(pnNewPosition)
				*pnNewPosition = uli.QuadPart;
		}
		return hr;

	}

	HRESULT GetPosition(
		ULONGLONG& nPos)
	{
		nPos = m_nPosition;
		return S_OK;
	}

	HRESULT Flush(
		VOID)
	{
		return S_OK;
	}

	HRESULT LockRange(
		ULONGLONG nPos, 
		ULONGLONG nCount)
	{
		return E_NOTIMPL;
	}

	HRESULT UnlockRange(
		ULONGLONG nPos,
		ULONGLONG nCount)
	{
		return E_NOTIMPL;
	}

	HRESULT SetSize(
		ULONGLONG nNewLen)
	{
		HRESULT hr = GrowBuffer((SIZE_T)nNewLen);
		if(FAILED(hr))
		{
			return hr;
		}
		if(nNewLen < (ULONGLONG)m_nPosition)
			m_nPosition = (SIZE_T)nNewLen;
		m_nFileSize = (SIZE_T)nNewLen;
		return S_OK;
	}

	HRESULT GetSize(
		ULONGLONG& nLen)
	{
		nLen = m_nFileSize;
		return S_OK;
	}

	HRESULT SetSizeRelease(
		ULONGLONG nNewLen)
	{
		HRESULT hr = GrowBuffer(
			(SIZE_T)nNewLen,
			TRUE);
		if(FAILED(hr))
		{
			return hr;
		}
		m_nFileSize = (SIZE_T)nNewLen;
		return S_OK;
	}

protected:
	HRESULT GrowBuffer(
		SIZE_T dwNewLen,
		BOOL bRelease = FALSE)
	{
		HRESULT hr = S_OK;
		if(dwNewLen > m_nBufSize || bRelease)
		{
			PBYTE pBuffer;
			SIZE_T nBufSize;
			if(m_nGrowBy == 0)
			{
				nBufSize = dwNewLen;
			}
			else
			{
				nBufSize = ((dwNewLen + m_nGrowBy - 1) / m_nGrowBy) * m_nGrowBy;
			}
			pBuffer = (PBYTE)m_allocator.Reallocate(
				m_pBuffer,
				m_nBufSize,
				nBufSize);
			if(pBuffer == NULL)
			{
				hr = E_OUTOFMEMORY;
			}
			else
			{
				m_pBuffer = pBuffer;
				m_nBufSize = nBufSize;
			}
		}
		return hr;
	}
};

//////////////////////////////////////////////////////////////////////////
// CRtlMemFileRead
class CRtlMemFileRead
{
protected:
	LPBYTE m_pBuffer;
	SIZE_T m_nFileSize;
	SIZE_T m_nPosition;

public:
	CRtlMemFileRead(
		const PVOID pBuffer = NULL,
		SIZE_T nBufSize = 0)
	{
		Attach(
			pBuffer,
			nBufSize);
	}

	PVOID GetData(
		VOID)
	{
		return m_pBuffer;
	}

	SIZE_T GetDataSize(
		VOID)
	{
		return m_nFileSize;
	}

	SIZE_T GetPosition(
		VOID)
	{
		return m_nPosition;
	}

	VOID Attach(
		const PVOID pBuffer,
		SIZE_T nBufSize)
	{
		m_pBuffer = (PBYTE)pBuffer;
		m_nFileSize = nBufSize;
		m_nPosition = 0;
	}

	HRESULT Create(
		LPCTSTR szFilename,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		LPSECURITY_ATTRIBUTES lpsa = NULL,
		HANDLE hTemplateFile = NULL)
	{
		return E_NOTIMPL;
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		DWORD* pBytesRead = NULL)
	{
		DWORD dwBytesRead = 0;
		if(m_nPosition <= m_nFileSize)
		{
			if(nBufSize > (DWORD)(m_nFileSize - m_nPosition))
			{
				dwBytesRead = (DWORD)(m_nFileSize - m_nPosition);
			}
			else
			{
				dwBytesRead = nBufSize;
			}
			if(dwBytesRead > 0)
			{
				CopyMemory(
					pBuffer,
					m_pBuffer + m_nPosition,
					dwBytesRead);
				m_nPosition += dwBytesRead;
			}
		}
		if(pBytesRead)
		{
			*pBytesRead = dwBytesRead;
		}
		return( dwBytesRead == nBufSize ? S_OK : S_FALSE );
	}

	HRESULT Read(
		LPVOID pBuffer,
		DWORD nBufSize,
		DWORD& nBytesRead)
	{
		return Read(
			pBuffer,
			nBufSize,
			&nBytesRead);
	}

	template< typename T >
	BOOL Read(
		T& value)
	{
		return Read(
			&value,
			sizeof(T));
	}

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		DWORD* pnBytesWritten = NULL)
	{
		return E_ACCESSDENIED;
	}

	HRESULT Write(
		LPCVOID pBuffer,
		DWORD nBufSize,
		DWORD& dwBytesWritten)
	{
		return E_ACCESSDENIED;
	}

	HRESULT Seek(
		LARGE_INTEGER dlibMove,
		DWORD dwFrom = SEEK_CUR,
		ULARGE_INTEGER *plibNewPosition = NULL)
	{
		ATLASSERT(
			dwFrom == SEEK_SET ||
			dwFrom == SEEK_END ||
			dwFrom == SEEK_CUR);
		LONGLONG lNewPos;
		if (dwFrom == SEEK_SET)
		{
			lNewPos = 0;
		}
		else if(dwFrom == SEEK_CUR)
		{
			lNewPos = m_nPosition;
		}
		else if(dwFrom == SEEK_END)
		{
			lNewPos = m_nFileSize;
		}
		else
		{
			return E_INVALIDARG;
		}
		lNewPos += dlibMove.QuadPart;
		m_nPosition = (SIZE_T)lNewPos;
		if(plibNewPosition)
			plibNewPosition->QuadPart = lNewPos;
		return S_OK;
	}

	HRESULT Seek(
		LONGLONG nOffset,
		DWORD dwFrom = SEEK_CUR,
		ULONGLONG* pnNewPosition = NULL)
	{
		LARGE_INTEGER li;
		ULARGE_INTEGER uli;
		li.QuadPart = nOffset;
		uli.QuadPart = 0;
		HRESULT hr = Seek(
			li,
			dwFrom,
			&uli);
		if(SUCCEEDED(hr))
		{
			if(pnNewPosition)
				*pnNewPosition = uli.QuadPart;
		}
		return hr;

	}

	HRESULT GetPosition(
		ULONGLONG& nPos)
	{
		nPos = m_nPosition;
		return S_OK;
	}

	HRESULT Flush(
		VOID)
	{
		return S_OK;
	}

	HRESULT LockRange(
		ULONGLONG nPos, 
		ULONGLONG nCount)
	{
		return E_NOTIMPL;
	}

	HRESULT UnlockRange(
		ULONGLONG nPos,
		ULONGLONG nCount)
	{
		return E_NOTIMPL;
	}

	HRESULT SetSize(
		ULONGLONG nNewLen)
	{
		return E_ACCESSDENIED;
	}

	HRESULT GetSize(
		ULONGLONG& nLen)
	{
		nLen = m_nFileSize;
		return S_OK;
	}
};

NAMESPACE_RTL_END
