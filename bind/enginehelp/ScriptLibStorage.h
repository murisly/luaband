#pragma once

#include <map>
#include <string>
#include <Shlwapi.h>
#include <atlbase.h>
#include <atlcom.h>

#include "rtlmem.h"
#include "memory/ref_counted.h"
#include "interface/IScriptLibStorage.h"

class CScriptLibStorage;

//////////////////////////////////////////////////////////////////////////
// CScriptLibStream
class CScriptLibStream : public binding::lua::IScriptLibStream,public binding::RefCountedImpl<CScriptLibStream>
{
	friend CScriptLibStorage;
	CRtlMemFileT<> m_file;
public:
	// IScriptLibStream
	virtual void AddRef()
	{
		InternalAddRef();
	}

	virtual void Release()
	{
		InternalRelease();
	}
	virtual long Read(void *pv,ULONG cb,ULONG *pcbRead);
};

//////////////////////////////////////////////////////////////////////////
// CScriptFileStream
class CScriptFileStream : public binding::lua::IScriptLibStream,public binding::RefCountedImpl<CScriptFileStream>
{
	friend CScriptLibStorage;
	CRtlMemFileT<> m_file;
public:
	// IScriptLibStream
	virtual void AddRef()
	{
		InternalAddRef();
	}

	virtual void Release()
	{
		InternalRelease();
	}
	virtual long Read(void *pv,ULONG cb,ULONG *pcbRead);
};


//////////////////////////////////////////////////////////////////////////
// CScriptLibStorage
class CScriptLibStorage : public binding::lua::IScriptLibStorage,public binding::RefCountedImpl<CScriptLibStorage>
{
	std::wstring m_strPath;
	struct NodeInfo 
	{
		DWORD dwOffset;
		DWORD dwLen;
		NodeInfo()
		{
			dwOffset = 0;
			dwLen = 0;
		}
	};
	std::map<std::wstring,NodeInfo> m_mapGuid;
	CRtlMemFileT<> m_mem;
public:
	static CScriptLibStorage* GetInstance();
	// IScriptLibStorage
	virtual void AddRef()
	{
		InternalAddRef();
	}

	virtual void Release()
	{
		InternalRelease();
	}
	virtual long Open(const std::wstring& strFileName);
	virtual long OpenBuff(const std::vector<char>& vecBuff);
	virtual long OpenStream(const std::wstring& strStreamName,binding::lua::IScriptLibStream** ppstm);
};
