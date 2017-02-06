#pragma once
#include <windows.h>
#include <vector>

namespace binding{namespace strings{

	std::wstring Trim(const std::wstring &source, const std::wstring &targets);

	std::string  ws2s(const std::wstring& s_src,UINT CodePage = CP_UTF8);
	std::wstring s2ws( const std::string& s_src,UINT CodePage = CP_UTF8);

	std::wstring int2persent(int nNum);

	std::string&  replace_all_distinct(std::string& str,const std::string& old_value,const std::string&  new_value);
	std::wstring  replace_all_distinct(std::wstring& str,const std::wstring old_value,const std::wstring  new_value);

	size_t utf8_2_utf16(const char *src, wchar_t *dest);

	bool   getTwoString(const std::wstring& sValues,const wchar_t sTag,std::wstring& s1,std::wstring& s2);
	void   splitString(const std::wstring& sSrc,const wchar_t sTag,std::vector<std::wstring>& retVec);
	void   splitString2(const std::wstring& sSrc, const std::wstring& sTag, std::vector<std::wstring>& retVec);
	void   splitStringA(const std::string& sSrc,const char sTag,std::vector<std::string>& retVec);
	void   splitStringA2(const std::string& sSrc,const std::string& sTag,std::vector<std::string>& retVec);

	// Returns true if the string passed in matches the pattern. The pattern
	// string can contain wildcards like * and ?
	// The backslash character (\) is an escape character for * and ?
	// We limit the patterns to having a max of 16 * or ? characters.
	// ? matches 0 or 1 character, while * matches 0 or more characters.
	bool MatchPattern(const std::wstring& eval, const std::wstring& pattern);

	bool IsPatternString(const std::wstring& str);
	
	std::string url_encode(const std::string& in, const char* valid = "._-=");
	std::string url_decode(const std::string& in);

	std::wstring trim_all(const std::wstring& in);

	bool HexStringToInt(const char* ptr, int len, int* val);
	bool StringToInt(const char* ptr, int len, int* val);
	bool StringToInt(const std::wstring& str, int* val);
	bool StringToDouble(const char* ptr, int len, double* val);

	std::string HexEncode( const void* bytes, size_t size );

	std::wstring to_lower(const std::wstring& in);
	std::string  to_lower(const std::string& in);

	std::wstring to_upper(const std::wstring& in);
	std::string  to_upper(const std::string& in);

	bool CompareNoCaseA(const std::string &l,const std::string &r);
	bool CompareNoCase(const std::wstring &l,const std::wstring &r);
}}
