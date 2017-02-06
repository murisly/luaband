#pragma once

#ifndef __cplusplus
#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
#error rtlapp.h requires atlbase.h to be included first
#endif

#define _RTL_VER	0x0201

#define RTL_NAMESPACE_NAME		ATL

#define NAMESPACE_RTL_BEGIN		namespace RTL_NAMESPACE_NAME {
#define NAMESPACE_RTL_END		};

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifndef _roundup
#define _roundup(s, a) \
	( (((s) + (a) - 1) & ~((a) - 1)) )
#define _rounddown(s, a) \
	( ((s) & ~((a) - 1)) )
#endif

#ifndef _offsetof
#define _offsetof(s, m) (SIZE_T)&(((s *)0)->m)
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#define CompareMemory	memcmp
