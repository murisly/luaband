#include "stdafx.h"
#include <windows.h>
#include "enginehelp/Mutex.h"

namespace binding{ namespace thread{

Mutex::Mutex ()
{
#if defined WIN32
    ::InitializeCriticalSection (&_mutex);
#elif HAVE_PTHREAD
    if (int error = ::pthread_mutex_init (&_mutex, 0))
        printf("Cannot initialize mutex %d\n", error);
#endif
}

Mutex::~Mutex ()
{
#if defined WIN32
    ::DeleteCriticalSection (&_mutex);
#elif HAVE_PTHREAD
    if(int error = ::pthread_mutex_destroy (&_mutex))
        printf("Cannot de-initialize mutex %d\n", error);
#endif
}

void
Mutex::lock () const
{
#if defined WIN32
    ::EnterCriticalSection (&_mutex);
#elif HAVE_PTHREAD
    if (int error = ::pthread_mutex_lock (&_mutex))
        printf("Cannot lock mutex %d \n", error);
#endif
}


void
Mutex::unlock () const
{
#if defined WIN32
    ::LeaveCriticalSection (&_mutex);
#elif HAVE_PTHREAD
    if (int error = ::pthread_mutex_unlock (&_mutex))
        printf("Cannot unlock mutex %d\n", error);
#endif
}

}}