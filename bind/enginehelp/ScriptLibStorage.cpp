#include "stdafx.h"
#include <fstream>
#include <vector>
#include <atlstr.h>
#include "strings/StringsHelper.h"
#include "ScriptLibStorage.h"


//////////////////////////////////////////////////////////////////////////
// CScriptLibStream
// IScriptLibStream
long CScriptLibStream::Read(void *pv,ULONG cb,ULONG *pcbRead)
{
	DWORD dwRead = 0;
	HRESULT hr = m_file.Read(
		pv,
		cb,
		dwRead);
	if(pcbRead)
		*pcbRead = dwRead;
	return hr;
}

long CScriptFileStream::Read(void *pv,ULONG cb,ULONG *pcbRead)
{
	DWORD dwRead = 0;
	HRESULT hr = m_file.Read(
		pv,
		cb,
		dwRead);
	if(pcbRead)
		*pcbRead = dwRead;
	return hr;
}

//////////////////////////////////////////////////////////////////////////
// CScriptLibStorage
CScriptLibStorage* CScriptLibStorage::GetInstance()
{
	CScriptLibStorage* p = new CScriptLibStorage;
	p->AddRef();
	return p;
}

long CScriptLibStorage::Open(const std::wstring& strFileName)
{
	if(strFileName.empty())
		return E_POINTER;

	m_strPath = strFileName;

	std::fstream file;
	file.open(m_strPath,std::ios::in|std::ios::binary|std::ios::_Nocreate);
	if(file.fail())
	{
		return E_INVALIDARG;
	}

	file.seekg(0, std::ios::end);      //设置文件指针到文件流的尾部
	std::ios::pos_type ps = file.tellg();  //读取文件指针的位置
	unsigned int uSize = (unsigned int)ps;
// 	if (uSize <= sizeof(TsScriptLibHeader))
// 	{
// 		file.close();
// 		return S_FALSE;
// 	}

	file.seekg(0, std::ios::beg);      //设置文件指针到文件流的头部
	std::vector<char> vecBuff;
	vecBuff.resize(uSize,0);
	file.read(&vecBuff[0],uSize);

	file.close();

	return OpenBuff(vecBuff);
}

long CScriptLibStorage::OpenBuff(const std::vector<char>& vecBuff)
{
	unsigned int uSize = vecBuff.size();
/*
	if (uSize <= sizeof(TsScriptLibHeader))
	{
		return S_FALSE;
	}
	TsScriptLibHeader* pHeader = (TsScriptLibHeader*)&vecBuff[0];
	DWORD dwNodeOffset = _roundup(sizeof(TsScriptLibHeader), 16);
	if(pHeader->dwNodeOffset != dwNodeOffset)
	{
		return S_FALSE;
	}

	if(uSize < pHeader->dwNodeOffset)
		return S_FALSE;

	unsigned long crc32 = binding::safe::GetCRC32(uSize - pHeader->dwNodeOffset,const_cast<char*>(&vecBuff[0]) + pHeader->dwNodeOffset);
	if (pHeader->dwSign != GCSCRIPTLIB_DAT_MARK ||
		pHeader->wMajorVer > GCSCRIPTLIB_MAX_SUPPORT_VER ||
		pHeader->wMajorVer < GCSCRIPTLIB_MIN_SUPPORT_VER ||
		pHeader->dwNodeNum == 0 ||
		pHeader->dwNodeOffset < sizeof(TsScriptLibHeader) ||
		pHeader->dwNodeLength != sizeof(TsScriptLibNode) ||
		pHeader->dwCheckSum != crc32 ||
		pHeader->dwDataLength == 0)
	{
		return S_FALSE;
	}

	std::vector<byte> vecData;
	vecData.resize(pHeader->dwDataLength,0);

	byte* pTemp = (byte*)&vecBuff[0] + pHeader->dwNodeOffset;
	DWORD dwTemp = pHeader->dwDataLength;
	if (!binding::safe::ZlibDe(pTemp, uSize - pHeader->dwNodeOffset, &vecData[0], &dwTemp))
	{
		return S_FALSE;
	}

	DWORD dwDataOffset = 0;
	for (UINT i = 0; i < pHeader->dwNodeNum; ++i)
	{
		TsScriptLibNode* pNode = (TsScriptLibNode*)(&vecData[0] + i*pHeader->dwNodeLength);
		if (i == 0)
		{
			dwDataOffset = pNode->dwDataOffset;
		}
		PBYTE pID = &vecData[0] + pNode->dwIDOffset - pHeader->dwNodeOffset;

		CStringA strGUID = "";
		for(ULONG j = 0; j < GUID_LENGTH; ++j)
		{
			strGUID.AppendFormat("%.2x", pID[j]);
		}
		TCHAR szGUID[MAX_PATH + 1] = {0};
		::MultiByteToWideChar(CP_ACP, 0, strGUID, -1, szGUID, MAX_PATH);
		szGUID[MAX_PATH] = _T('\0');


		NodeInfo st;
		st.dwOffset = pNode->dwDataOffset - dwDataOffset;
		st.dwLen = pNode->dwDataLen;
		m_mapGuid[szGUID] = st;
	}

	INT iIndexAndGUIDLen = dwDataOffset - pHeader->dwNodeOffset;
	m_mem.Write(&vecData[0] + iIndexAndGUIDLen, pHeader->dwDataLength - iIndexAndGUIDLen);
*/
	return S_OK;
}

std::wstring pathcombine( const std::wstring& dir, const wchar_t* file )
{
	wchar_t buffer[MAX_PATH] = {0};
	PathCombineW(buffer, dir.c_str(), file);
	return std::wstring(buffer);
}

// IScriptLibStorage
long CScriptLibStorage::OpenStream(const std::wstring& strStreamName,binding::lua::IScriptLibStream** ppstm)
{
	if(ppstm == NULL || strStreamName.empty())
		return E_POINTER;

	std::wstring strFilePath = pathcombine(m_strPath,strStreamName.c_str());
	if(!PathFileExists(strFilePath.c_str()) || PathIsDirectory(strFilePath.c_str()))
		return E_INVALIDARG;


	std::fstream file;
	file.open(strFilePath,std::ios::in|std::ios::binary|std::ios::_Nocreate);
	if(file.fail())
	{
		return E_ACCESSDENIED;
	}

	file.seekg(0, std::ios::end);      //设置文件指针到文件流的尾部
	std::ios::pos_type ps = file.tellg();  //读取文件指针的位置
	std::streamsize uSize = ps;
	file.seekg(0, std::ios::beg);      //设置文件指针到文件流的头部

	char header[3] = {};  
	char uniTxt[] = {(char)0xFF, (char)0xFE};          // Unicode file header  
	char endianTxt[] = {(char)0xFE, (char)0xFF};       // Unicode big endian file header  
	char utf8Txt[] = {(char)0xEF, (char)0xBB, (char)0xBF};   // UTF_8 file header  

	int offset(0);
	file.read(header,3);
	std::string vecBuff;
	if(header[0] == uniTxt[0]&&header[1] == uniTxt[1])
	{
		offset = 2;
		file.seekg(2, std::ios::beg);
		std::wstring buff;
		while (!file.eof())
		{
			wchar_t wch;
			file.read((char*)(&wch), 2);
			buff.push_back(wch);
		}
		vecBuff = binding::strings::ws2s(buff);
	}
	else if(header[0] == endianTxt[0]&&header[1] == endianTxt[1])
	{
		offset = 2;
		file.seekg(2, std::ios::beg);    
		std::wstring buff;
		while (!file.eof())
		{
			wchar_t wch;
			file.read((char*)(&wch), 2);
			buff.push_back(wch);
		}
		vecBuff = binding::strings::ws2s(buff);
	}
	else if(header[0] == utf8Txt[0]&&header[1] == utf8Txt[1]&&header[2] == utf8Txt[2])
	{
		offset = 3;
		file.seekg(3, std::ios::beg); 
		vecBuff.resize((int)uSize - offset,0);
		file.read(&vecBuff[0],uSize - offset);
	}
	else
	{
		file.seekg(0, std::ios::beg); 
		vecBuff.resize((int)uSize - offset,0);
		file.read(&vecBuff[0],uSize - offset);
	}
	file.close();

	CScriptFileStream* p = new CScriptFileStream;
	p->m_file.Write(&vecBuff[0], vecBuff.size());
	p->m_file.Seek(0, FILE_BEGIN);

	p->AddRef();
	*ppstm = p;
	return S_OK;
}