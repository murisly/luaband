#pragma once

#include <windows.h>

namespace binding{ namespace thread{

class Lock;

class Mutex
{
public:
    
    Mutex ();
    virtual ~Mutex ();
    
private:
    
    void	lock () const;
    void	unlock () const;
    
#if defined WIN32
	mutable CRITICAL_SECTION _mutex;
#elif HAVE_PTHREAD
	mutable pthread_mutex_t _mutex;
#endif
    
    void operator = (const Mutex& M);	// not implemented
    Mutex (const Mutex& M);		// not implemented
    
    friend class Lock;
};


class Lock
{
public:
    
    Lock (const Mutex& m, bool autoLock = true):
	_mutex (m),
	_locked (false)
    {
        if (autoLock)
        {
            _mutex.lock();
            _locked = true;
        }
    }
    
    ~Lock ()
    {
        if (_locked)
            _mutex.unlock();
    }
    
    void acquire ()
    {
        _mutex.lock();
        _locked = true;
    }
    
    void release ()
    {
        _mutex.unlock();
        _locked = false;
    }
    
    bool locked ()
    {
        return _locked;
    }
    
private:
    
    const Mutex &	_mutex;
    bool		_locked;
};

}}
