#include "stdafx.h"
#include "strings\stringprintf.h"
#include <vector>
#include "memory/basictypes.h"

namespace binding
{
	namespace strings
	{
		namespace
		{
			inline int vsnprintfT(char* buffer,
				size_t buf_size,
				const char* format,
				va_list argptr) 
			{
				int length = _vsprintf_p(buffer, buf_size, format, argptr);
				if (length < 0) {
					if (buf_size > 0)
						buffer[0] = 0;
					return _vscprintf_p(format, argptr);
				}
				return length;
			}

			inline int vsnprintfT(wchar_t* buffer,
				size_t buf_size,
				const wchar_t* format,
				va_list argptr) 
			{
				int length = _vswprintf_p(buffer, buf_size, format, argptr);
				if (length < 0) {
					if (buf_size > 0)
						buffer[0] = 0;
					return _vscwprintf_p(format, argptr);
				}
				return length;
			}

			// Templatized backend for StringPrintF/StringAppendF. This does not finalize
			// the va_list, the caller is expected to do that.
			template <class StringType>
			static void StringAppendVT(StringType* dst,
				const typename StringType::value_type* format,
				va_list ap) 
			{
				// First try with a small fixed size buffer.
				// This buffer size should be kept in sync with StringUtilTest.GrowBoundary
				// and StringUtilTest.StringPrintfBounds.
				typename StringType::value_type stack_buf[1024];

				va_list ap_copy;
				(ap_copy = ap);

				int result = vsnprintfT(stack_buf, arraysize(stack_buf), format, ap_copy);
				va_end(ap_copy);

				if (result >= 0 && result < static_cast<int>(arraysize(stack_buf))) {
					// It fit.
					dst->append(stack_buf, result);
					return;
				}

				// Repeatedly increase buffer size until it fits.
				int mem_length = arraysize(stack_buf);
				while (true) {
					if (result < 0) {
						// On Windows, vsnprintfT always returns the number of characters in a
						// fully-formatted string, so if we reach this point, something else is
						// wrong and no amount of buffer-doubling is going to fix it.
						if (errno != 0 && errno != EOVERFLOW)
						{
							// If an error other than overflow occurred, it's never going to work.
							//DLOG(WARNING) << "Unable to printf the requested string due to error.";
							return;
						}
						// Try doubling the buffer size.
						mem_length *= 2;
					} else {
						// We need exactly "result + 1" characters.
						mem_length = result + 1;
					}

					if (mem_length > 32 * 1024 * 1024) {
						// That should be plenty, don't try anything larger.  This protects
						// against huge allocations when using vsnprintfT implementations that
						// return -1 for reasons other than overflow without setting errno.
						//DLOG(WARNING) << "Unable to printf the requested string due to size.";
						return;
					}

					std::vector<typename StringType::value_type> mem_buf(mem_length);

					// NOTE: You can only use a va_list once.  Since we're in a while loop, we
					// need to make a new copy each time so we don't use up the original.
					(ap_copy = ap);
					result = vsnprintfT(&mem_buf[0], mem_length, format, ap_copy);
					va_end(ap_copy);

					if ((result >= 0) && (result < mem_length)) {
						// It fit.
						dst->append(&mem_buf[0], result);
						return;
					}
				}
			}
		}
		std::string Printf( const char* format, ... )
		{
			va_list ap;
			va_start(ap, format);
			std::string result;
			AppendV(&result, format, ap);
			va_end(ap);
			return result;
		}

		std::wstring Printf( const wchar_t* format, ... )
		{
			va_list ap;
			va_start(ap, format);
			std::wstring result;
			AppendV(&result, format, ap);
			va_end(ap);
			return result;
		}

		std::string PrintV( const char* format, va_list ap )
		{
			std::string result;
			AppendV(&result, format, ap);
			return result;
		}

		const std::string& SPrintf( std::string* dst, const char* format, ... )
		{
			va_list ap;
			va_start(ap, format);
			dst->clear();
			AppendV(dst, format, ap);
			va_end(ap);
			return *dst;
		}

		const std::wstring& SPrintf( std::wstring* dst, const wchar_t* format, ... )
		{
			va_list ap;
			va_start(ap, format);
			dst->clear();
			AppendV(dst, format, ap);
			va_end(ap);
			return *dst;
		}

		void AppendF( std::string* dst, const char* format, ... )
		{
			va_list ap;
			va_start(ap, format);
			AppendV(dst, format, ap);
			va_end(ap);
		}

		void AppendF( std::wstring* dst, const wchar_t* format, ... )
		{
			va_list ap;
			va_start(ap, format);
			AppendV(dst, format, ap);
			va_end(ap);
		}

		void AppendV( std::string* dst, const char* format, va_list ap )
		{
			StringAppendVT(dst, format, ap);
		}

		void AppendV( std::wstring* dst, const wchar_t* format, va_list ap )
		{
			StringAppendVT(dst, format, ap);
		}
	}
}