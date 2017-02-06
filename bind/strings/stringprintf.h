#pragma once

#include <stdarg.h>   // va_list
#include <string>

namespace binding
{
	namespace strings
	{
		std::string Printf(const char* format, ...);
		std::wstring Printf(const wchar_t* format, ...);
		std::string PrintV(const char* format, va_list ap);
		const std::string& SPrintf(std::string* dst, const char* format, ...);
		const std::wstring& SPrintf(std::wstring* dst, const wchar_t* format, ...);
		void AppendF(std::string* dst, const char* format, ...);
		void AppendF(std::wstring* dst, const wchar_t* format, ...);
		void AppendV(std::string* dst, const char* format, va_list ap);
		void AppendV(std::wstring* dst, const wchar_t* format, va_list ap);
	}
}