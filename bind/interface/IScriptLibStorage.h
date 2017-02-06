#pragma once

#include <string>
#include <vector>

namespace binding{namespace lua{

class IScriptLibStream 
{
public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual long Read(void *pv,ULONG cb,ULONG *pcbRead) = 0;
};

class IScriptLibStorage 
{
public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual long Open(const std::wstring& strFileName) = 0;
	virtual long OpenBuff(const std::vector<char>& vecBuff) = 0;
	virtual long OpenStream(const std::wstring& strStreamName,IScriptLibStream** ppstm) = 0;
};
}}