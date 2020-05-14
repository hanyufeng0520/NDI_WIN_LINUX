#pragma once
#include "Lib.Base/SRWLocker.h"

/**
 *Usage Example:
	std::list<int> temp = { 0,1,2,3,45 };
	Synchronized<std::list<int>> m_container;
	m_container.with_lock([&](auto& item)
		{
			std::swap(item, temp);
		});
	size_t size = m_container.with_lock([&](auto& item)
		{
			return item.size();
		});
	size_t size = m_container.with_lock()->size();
	m_container.with_lock()->emplace_back(1);
	m_container.with_lock()->remove(1);
 */
 //https://github.com/facebook/folly/blob/master/folly/Synchronized.h

template <class SynchronizedType, class LockType>
class LockedPtr
{
	SynchronizedType*	m_datum = nullptr;
	LockType*			m_locker = nullptr;
	bool				m_read = false;
public:
	LockedPtr(SynchronizedType* datum, LockType* locker, bool read) :
		m_datum(datum), m_locker(locker), m_read(read)
	{
		if (m_read)
			m_locker->lock_r();
		else
			m_locker->lock_w();
	}

	LockedPtr(const LockedPtr& rhs) = delete;
	LockedPtr& operator=(const LockedPtr& rhs) = delete;
	LockedPtr() = default;
	LockedPtr(LockedPtr<SynchronizedType, LockType>&& other) noexcept = default;
	LockedPtr& operator=(LockedPtr<SynchronizedType, LockType>&& other) noexcept = default;

	~LockedPtr()
	{
		if (m_locker == nullptr)
			return;
		if (m_read)
			m_locker->unlock_r();
		else
			m_locker->unlock_w();
		m_locker = nullptr;
	}

	SynchronizedType* operator->() const { return m_datum; }
	SynchronizedType& operator*() const { return *m_datum; }
};

template<class T>
class Synchronized
{
	SRWLocker	m_locker;
	T			m_datum;
public:
	template <class Function>
	auto with_lock(Function&& function)
	{
		SRWLockHolder lock(m_locker, false);
		return function(m_datum);
	}

	template <class Function>
	auto rlock(Function&& function)
	{
		SRWLockHolder lock(m_locker, true);
		return function(m_datum);
	}

	template <class Function>
	auto wlock(Function&& function)
	{
		SRWLockHolder lock(m_locker, false);
		return function(m_datum);
	}

	LockedPtr<T, SRWLocker> with_lock()
	{
		return LockedPtr<T, SRWLocker>(&m_datum, &m_locker, false);
	}

	LockedPtr<T, SRWLocker> rlock()
	{
		return LockedPtr<T, SRWLocker>(&m_datum, &m_locker, true);
	}

	LockedPtr<T, SRWLocker> wlock()
	{
		return LockedPtr<T, SRWLocker>(&m_datum, &m_locker, false);
	}

	T& without_lock()
	{
		return m_datum;
	}
};
