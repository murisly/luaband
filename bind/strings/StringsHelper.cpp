#include "stdafx.h"

#include <string>
#include <algorithm>

#include "strings\StringsHelper.h"

namespace binding{namespace strings{

	std::wstring Trim(const std::wstring &source, const std::wstring &targets)
	{
		std::wstring::size_type start = 0;
		std::wstring::size_type end = 0;
		for (start = 0; start < source.size(); start++)
		{
			bool bIsTarget = false;
			for (std::wstring::size_type i = 0; i < targets.size(); i++)
			{
				if (source[start] == targets[i])
				{
					bIsTarget = true;
					break;
				}
			}
			if (!bIsTarget)
			{
				break;
			}
		}
		for (end = source.size() - 1; (int)end >= 0; end--)
		{
			bool bIsTarget = false;
			for (std::wstring::size_type i = 0; i < targets.size(); i++)
			{
				if (source[end] == targets[i])
				{
					bIsTarget = true;
					break;
				}
			}
			if (!bIsTarget)
			{
				break;
			}
		}
		std::wstring result = L"";
		if (end >= start && start < source.size() && end >= 0)
		{
			result = source.substr(start, end-start+1);
		}

		return result;
	}

	std::string ws2s(const std::wstring& wide, UINT CodePage)
	{
		int wide_length = static_cast<int>(wide.length());
		if (wide_length == 0)
			return std::string();

		// Compute the length of the buffer we'll need.
		int charcount = WideCharToMultiByte(CodePage, 0, wide.data(), wide_length,
			NULL, 0, NULL, NULL);
		if (charcount == 0)
			return std::string();

		std::string mb;
		mb.resize(charcount);
		WideCharToMultiByte(CodePage, 0, wide.data(), wide_length,
			&mb[0], charcount, NULL, NULL);

		return mb;
	}

	std::wstring s2ws( const std::string& mb,UINT CodePage)
	{
		if (mb.empty())
			return std::wstring();

		int mb_length = static_cast<int>(mb.length());
		// Compute the length of the buffer.
		int charcount = MultiByteToWideChar(CodePage, 0,
			mb.data(), mb_length, NULL, 0);
		if (charcount == 0)
			return std::wstring();

		std::wstring wide;
		wide.resize(charcount);
		MultiByteToWideChar(CodePage, 0, mb.data(), mb_length, &wide[0], charcount);

		return wide;
	}

	std::wstring int2persent(int nNum)
	{
		wchar_t strPos[200] = {0};
		_itow_s(nNum,strPos,10);
		std::wstring sPos = strPos;
		sPos.append(L"%");
		return sPos;
	}

	std::string&  replace_all_distinct(std::string& str,const std::string& old_value,const std::string&  new_value)   
	{   
		for(std::string::size_type   pos(0);   pos!=std::string::npos;   pos+=new_value.length())   {   
			if(   (pos=str.find(old_value,pos))!=std::string::npos   )   
				str.replace(pos,old_value.length(),new_value);   
			else   break;   
		}   
		return   str;   
	} 

	std::wstring  replace_all_distinct(std::wstring& str,const std::wstring old_value,const std::wstring  new_value)   
	{   
		for(std::wstring::size_type   pos(0);   pos!=std::wstring::npos;   pos+=new_value.length())   {   
			if(   (pos=str.find(old_value,pos))!=std::wstring::npos   )   
				str.replace(pos,old_value.length(),new_value);   
			else   break;   
		}   
		return   str;   
	} 

	size_t utf8_2_utf16(const char *src, wchar_t *dest)
	{
		unsigned long c;
		size_t extra_bytes;
		size_t len = 0;

		while (*src)
		{
			c = (unsigned long)*src++ & 0xFFUL;

			if ((c & 0x80UL) == 0UL)
			{
				*dest++ = (wchar_t)c;
				len++;
				continue;
			}
			else if ((c & 0xE0UL) == 0xC0UL)
			{
				c -= 0xC0UL;
				extra_bytes = 1;
			}
			else if ((c & 0xF0UL) == 0xE0UL)
			{
				c -= 0xE0UL;
				extra_bytes = 2;
			}
			else if ((c & 0xF8UL) == 0xF0UL)
			{
				c -= 0xF0UL;
				extra_bytes = 3;
			}
			else
				return 0;

			while (extra_bytes)
			{
				if (*src == 0) return 0;
				if ((*src & 0xC0UL) != 0x80UL) return 0;
				c <<= 6;
				c += (unsigned long)*src++ & 0x3FUL;
				extra_bytes--;
			}

			if (c < 0x10000UL)
			{
				if (c >= 0xD800UL && c <= 0xDFFFUL) return 0;
				*dest++ = (wchar_t)c;
				len++;
			}
			else
			{
				c -= 0x10000UL;
				if (c >= 0x100000UL) return 0; 
				*dest++ = (wchar_t)(0xD800UL + (c >> 10));
				*dest++ = (wchar_t)(0xDC00UL + (c & 0x3FFUL));
				len += 2;
			}
		}

		*dest = (wchar_t)'\0';    
		return len;
	}

	bool getTwoString(const std::wstring& sValues,const wchar_t sTag,std::wstring& s1,std::wstring& s2)
	{
		int ipos = sValues.find(sTag);
		if (ipos != -1)
		{
			s1 = sValues.substr(0,ipos);
			s2 = sValues.substr(ipos+1);

			return true;
		}

		return false;
	}

	void splitString(const std::wstring& sSrc,const wchar_t sTag,std::vector<std::wstring>& retVec)
	{
		std::wstring temp = sSrc;

		while (true)
		{
			int ipos = temp.find(sTag);
			if (ipos == -1)
			{
				retVec.push_back(temp);
				break;
			}

			retVec.push_back(temp.substr(0,ipos));
			temp = temp.substr(ipos+1);
		}
	}

	void splitString2(const std::wstring& sSrc, const std::wstring& sTag, std::vector<std::wstring>& retVec)	
	{
		std::wstring temp = sSrc;

		while (true)
		{
			int ipos = temp.find(sTag);
			if (ipos == -1)
			{
				retVec.push_back(temp);
				break;
			}

			retVec.push_back(temp.substr(0, ipos));
			temp = temp.substr(ipos + sTag.length());
		}
	}

	void splitStringA(const std::string& sSrc,const char sTag,std::vector<std::string>& retVec)
	{
		std::string temp = sSrc;

		while (true)
		{
			int ipos = temp.find(sTag);
			if (ipos == -1)
			{
				retVec.push_back(temp);
				break;
			}

			retVec.push_back(temp.substr(0,ipos));
			temp = temp.substr(ipos+1);
		}
	}

	void splitStringA2(const std::string& sSrc,const std::string& sTag,std::vector<std::string>& retVec)
	{
		std::string temp = sSrc;

		while (true)
		{
			int ipos = temp.find(sTag);
			if (ipos == -1)
			{
				retVec.push_back(temp);
				break;
			}

			retVec.push_back(temp.substr(0,ipos));
			temp = temp.substr(ipos + sTag.length());
		}
	}

	static bool IsWildcard(unsigned int character) {
		return character == '*' || character == '?';
	}

	// Move the strings pointers to the point where they start to differ.
	template <typename CHAR, typename NEXT>
	static void EatSameChars(const CHAR** pattern, const CHAR* pattern_end,
		const CHAR** string, const CHAR* string_end,
		NEXT next) {
			const CHAR* escape = NULL;
			while (*pattern != pattern_end && *string != string_end) {
				if (!escape && IsWildcard(**pattern)) {
					// We don't want to match wildcard here, except if it's escaped.
					return;
				}

				// Check if the escapement char is found. If so, skip it and move to the
				// next character.
// 				if (!escape && **pattern == '\\') {
// 					escape = *pattern;
// 					next(pattern, pattern_end);
// 					continue;
// 				}

				// Check if the chars match, if so, increment the ptrs.
				const CHAR* pattern_next = *pattern;
				const CHAR* string_next = *string;
				unsigned int pattern_char = next(&pattern_next, pattern_end);
				if (pattern_char == next(&string_next, string_end) &&
					pattern_char != (unsigned int) -1) {
						*pattern = pattern_next;
						*string = string_next;
				} else {
					// Uh ho, it did not match, we are done. If the last char was an
					// escapement, that means that it was an error to advance the ptr here,
					// let's put it back where it was. This also mean that the MatchPattern
					// function will return false because if we can't match an escape char
					// here, then no one will.
					if (escape) {
						*pattern = escape;
					}
					return;
				}

				escape = NULL;
			}
	}

	template <typename CHAR, typename NEXT>
	static void EatWildcard(const CHAR** pattern, const CHAR* end, NEXT next) {
		while (*pattern != end) {
			if (!IsWildcard(**pattern))
				return;
			next(pattern, end);
		}
	}

	template <typename CHAR, typename NEXT>
	static bool MatchPatternT(const CHAR* eval, const CHAR* eval_end,
		const CHAR* pattern, const CHAR* pattern_end,
		int depth,
		NEXT next) {
			const int kMaxDepth = 16;
			if (depth > kMaxDepth)
				return false;

			// Eat all the matching chars.
			EatSameChars(&pattern, pattern_end, &eval, eval_end, next);

			// If the string is empty, then the pattern must be empty too, or contains
			// only wildcards.
			if (eval == eval_end) {
				EatWildcard(&pattern, pattern_end, next);
				return pattern == pattern_end;
			}

			// Pattern is empty but not string, this is not a match.
			if (pattern == pattern_end)
				return false;

			// If this is a question mark, then we need to compare the rest with
			// the current string or the string with one character eaten.
			const CHAR* next_pattern = pattern;
			next(&next_pattern, pattern_end);
			if (pattern[0] == '?') {
				if (MatchPatternT(eval, eval_end, next_pattern, pattern_end,
					depth + 1, next))
					return true;
				const CHAR* next_eval = eval;
				next(&next_eval, eval_end);
				if (MatchPatternT(next_eval, eval_end, next_pattern, pattern_end,
					depth + 1, next))
					return true;
			}

			// This is a *, try to match all the possible substrings with the remainder
			// of the pattern.
			if (pattern[0] == '*') {
				// Collapse duplicate wild cards (********** into *) so that the
				// method does not recurse unnecessarily. http://crbug.com/52839
				EatWildcard(&next_pattern, pattern_end, next);

				while (eval != eval_end) {
					if (MatchPatternT(eval, eval_end, next_pattern, pattern_end,
						depth + 1, next))
						return true;
					eval++;
				}

				// We reached the end of the string, let see if the pattern contains only
				// wildcards.
				if (eval == eval_end) {
					EatWildcard(&pattern, pattern_end, next);
					if (pattern != pattern_end)
						return false;
					return true;
				}
			}

			return false;
	}

#define CBU16_IS_LEAD(c) (((c)&0xfffffc00)==0xd800)
#define CBU16_IS_TRAIL(c) (((c)&0xfffffc00)==0xdc00)
#define CBU16_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)
#define CBU16_GET_SUPPLEMENTARY(lead, trail) \
	(((unsigned int)(lead)<<10UL)+(unsigned int)(trail)-CBU16_SURROGATE_OFFSET)

#define CBU16_NEXT(s, i, length, c) { \
	(c)=(s)[(i)++]; \
	if(CBU16_IS_LEAD(c)) { \
		unsigned short __c2; \
		if((i)<(length) && CBU16_IS_TRAIL(__c2=(s)[(i)])) { \
			++(i); \
			(c)=CBU16_GET_SUPPLEMENTARY((c), __c2); \
		} \
	} \
}

// 	struct NextCharUTF8 {
// 		unsigned int operator()(const char** p, const char* end) {
// 			unsigned int c;
// 			int offset = 0;
// 			CBU8_NEXT(*p, offset, end - *p, c);
// 			*p += offset;
// 			return c;
// 		}
// 	};

	struct NextCharUTF16 {
		unsigned int operator()(const wchar_t** p, const wchar_t* end) {
			unsigned int c;
			int offset = 0;
			CBU16_NEXT(*p, offset, end - *p, c);
			*p += offset;
			return c;
		}
	};

	bool MatchPattern( const std::wstring& eval, const std::wstring& pattern )
	{
		return MatchPatternT(eval.c_str(), eval.c_str() + eval.size(),
			pattern.c_str(), pattern.c_str() + pattern.size(),
			0, NextCharUTF16());
	}

	std::string url_encode( const std::string& in , const char *valid)
	{
		std::string out;
		for( size_t ix = 0; ix < in.size(); ix++ )
		{
			char ch = in[ix];
			if( isalnum( (BYTE)ch ) || strchr(valid, (BYTE)ch) )
			{      
				out.push_back(ch);
			}
			else
			{
				int high, low;
				high = ch >> 4;
				low = ch % 16;
				out.push_back('%');
				out.push_back((high > 9 ? high -10 + 'A': high + '0'));
				out.push_back((low > 9 ? low -10 + 'A': low + '0'));
			}
		}
		return out;
	}

	std::string url_decode( const std::string& in )
	{
		std::string out;
		for( size_t ix = 0; ix < in.size(); ix++ )
		{
			BYTE ch = 0;
			if(in[ix]=='%')
			{
				int high, low;
				high = in[ix+1];
				low = in[ix+2];
				ch = isdigit(high) ? high-'0' : high-'A'+10;
				ch = ch << 4;
				ch |= (isdigit(low) ? low-'0' : low-'A'+10);
				
				ix += 2;
			}
			else if(in[ix] == '+')
			{
				ch = ' ';
			}
			else
			{
				ch = in[ix];
			}
			out += (char)ch;
		}
		return out;
	}

#define WHITESPACE_UNICODE \
	0x0009, /* <control-0009> to <control-000D> */ \
	0x000A,                                        \
	0x000B,                                        \
	0x000C,                                        \
	0x000D,                                        \
	0x0020, /* Space */                            \
	0x0085, /* <control-0085> */                   \
	0x00A0, /* No-Break Space */                   \
	0x1680, /* Ogham Space Mark */                 \
	0x180E, /* Mongolian Vowel Separator */        \
	0x2000, /* En Quad to Hair Space */            \
	0x2001,                                        \
	0x2002,                                        \
	0x2003,                                        \
	0x2004,                                        \
	0x2005,                                        \
	0x2006,                                        \
	0x2007,                                        \
	0x2008,                                        \
	0x2009,                                        \
	0x200A,                                        \
	0x200C, /* Zero Width Non-Joiner */            \
	0x2028, /* Line Separator */                   \
	0x2029, /* Paragraph Separator */              \
	0x202F, /* Narrow No-Break Space */            \
	0x205F, /* Medium Mathematical Space */        \
	0x3000, /* Ideographic Space */                \
	0

	const unsigned short kWhitespaceUTF16[] = {
		WHITESPACE_UNICODE
	};

	std::wstring trim_all( const std::wstring& input )
	{
		// Find the edges of leading/trailing whitespace as desired.
		const std::wstring::size_type last_char = input.length() - 1;
		const std::wstring::size_type first_good_char = input.find_first_not_of((wchar_t*)kWhitespaceUTF16);
		const std::wstring::size_type last_good_char = input.find_last_not_of((wchar_t*)kWhitespaceUTF16);

		std::wstring output;
		// When the string was all whitespace, report that we stripped off whitespace
		// from whichever position the caller was interested in.  For empty input, we
		// stripped no whitespace, but we still need to clear |output|.
		if (input.empty() ||
			(first_good_char == std::wstring::npos) || (last_good_char == std::wstring::npos)) {
				bool input_was_empty = input.empty();  // in case output == &input
				return output;
		}

		// Trim the whitespace.
		output =
			input.substr(first_good_char, last_good_char - first_good_char + 1);

		// Return where we trimmed from.
		return output;
	}

	bool HexStringToInt( const char* ptr, int len, int* val )
	{
		if (ptr == NULL || len == 0)
			return false;

		std::string tmp_str(ptr, len);
		char* stop = NULL;
		*val = strtol(tmp_str.c_str(), &stop, 16);
		if (!(stop && *stop == 0))
			return false;
		return true;
	}

	bool StringToInt( const char* ptr, int len, int* val )
	{
		if (ptr == NULL || len == 0)
			return false;

		errno = 0;
		std::string tmp_str(ptr, len);
		char* stop = NULL;
		*val = strtol(tmp_str.c_str(), &stop, 10);
		if (!(stop && *stop == 0))
			return false;

		if (errno == ERANGE)//overflow 
			return false;
		return true;
	}

	bool StringToInt(const std::wstring& str, int* val)
	{
		if (str.empty())
			return false;

		errno = 0;
		wchar_t* stop = NULL;
		*val = wcstol(str.c_str(), &stop, 10);
		if (!(stop && *stop == 0))
			return false;

		if (errno == ERANGE)//overflow 
			return false;
		return true;
	}

	bool StringToDouble( const char* ptr, int len, double* val )
	{
		if (ptr == NULL || len == 0)
			return false;

		std::string tmp_str(ptr, len);
		char* stop = NULL;
		*val = strtod(tmp_str.c_str(), &stop);
		if (!(stop && *stop == 0))
			return false;
		return true;
	}

	std::string HexEncode( const void* bytes, size_t size )
	{
		static const char kHexChars[] = "0123456789abcdef";

		// Each input byte creates two output hex characters.
		std::string ret(size * 2, '\0');

		for (size_t i = 0; i < size; ++i) {
			char b = reinterpret_cast<const char*>(bytes)[i];
			ret[(i * 2)] = kHexChars[(b >> 4) & 0xf];
			ret[(i * 2) + 1] = kHexChars[b & 0xf];
		}
		return ret;
	}

	std::wstring to_lower( const std::wstring& in )
	{
		std::wstring out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (wchar_t)towlower(in[i]);
		}
		return out;
	}

	std::string to_lower( const std::string& in )
	{
		std::string out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (char)tolower(in[i]);
		}
		return out;
	}

	std::wstring to_upper( const std::wstring& in )
	{
		std::wstring out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (wchar_t)toupper(in[i]);
		}
		return out;
	}

	std::string to_upper( const std::string& in )
	{
		std::string out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (char)toupper(in[i]);
		}
		return out;
	}

	bool CompareNoCaseA(const std::string &l,const std::string &r)
	{
		return l.size() == r.size()
			&& equal(l.cbegin(), l.cend(), r.cbegin(),
			[](std::string::value_type l1, std::string::value_type r1)
		{ return toupper(l1) == toupper(r1); });
	}

	bool CompareNoCase(const std::wstring &l,const std::wstring &r)
	{
		return l.size() == r.size()
			&& equal(l.cbegin(), l.cend(), r.cbegin(),
			[](std::wstring::value_type l1, std::wstring::value_type r1)
		{ return towupper(l1) == towupper(r1); });
	}
}}