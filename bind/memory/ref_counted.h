#pragma once

#include <assert.h>

#include "../memory/atomicops.h"

namespace binding{
	class RefCountedBase {
	public:
		bool HasOneRef() const { 
			return ref_count_ == 1; 
		}

	protected:
		RefCountedBase(): ref_count_(0)
		{
#ifndef NDEBUG
			in_dtor_ = false;
#endif
		}
		~RefCountedBase()
		{
#ifndef NDEBUG
			assert(in_dtor_);
			//<< "RefCountedThreadSafe object deleted without "
			//"calling Release()";
#endif
		}

		void AddRef() const
		{
#ifndef NDEBUG
			assert(!in_dtor_);
#endif
			subtle::NoBarrier_AtomicIncrement(&ref_count_, 1);
		}

		// Returns true if the object should self-delete.
		bool Release() const
		{
#ifndef NDEBUG
			assert(!in_dtor_);
			//assert(subtle::Acquire_Load(ptr) != 0);
#endif
			if (subtle::Barrier_AtomicIncrement(&ref_count_, -1) == 0) {
#ifndef NDEBUG
				in_dtor_ = true;
#endif
				return true;
			}
			return false;
		}

	private:
#ifndef NDEBUG
		mutable bool in_dtor_;
#endif
		mutable subtle::Atomic32 ref_count_;

		DISALLOW_COPY_AND_ASSIGN(RefCountedBase);
	};

	template <class T>
	class RefCounted : public RefCountedBase {
	public:
		RefCounted() {}

		void AddRef() const {
			RefCountedBase::AddRef();
		}

		void Release() const {
			if (RefCountedBase::Release()) {
				delete static_cast<const T*>(this);
			}
		}

	protected:
		~RefCounted() {}

	private:
		DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
	};

	template <class T>
	class RefCountedImpl : public RefCountedBase {
	public:
		RefCountedImpl() {}

		void InternalAddRef() const {
			RefCountedBase::AddRef();
		}

		void InternalRelease() const {
			if (RefCountedBase::Release()) {
				delete static_cast<const T*>(this);
			}
		}

	protected:
		~RefCountedImpl() {}

	private:
		DISALLOW_COPY_AND_ASSIGN(RefCountedImpl<T>);
	};
}
