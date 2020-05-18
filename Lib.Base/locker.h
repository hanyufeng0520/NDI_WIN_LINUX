#pragma once
#include <mutex>

class Locker
{
	std::recursive_mutex	 m_locker;

public:
	Locker() {		
	};
	~Locker() {		
	};

	Locker& operator=(const Locker&) = delete;
	Locker(const Locker &) = delete;

	void lock()
	{
		m_locker.lock();
	};
	bool try_lock()
	{
		return m_locker.try_lock();
	};
	void unlock() {
		m_locker.unlock();
	};
};

class LockHolder
{
	Locker& m_locker;

public:
	explicit LockHolder(Locker& _locker) : m_locker(_locker)
	{
		m_locker.lock();
	};
	~LockHolder()
	{
		m_locker.unlock();
	};

	LockHolder& operator=(const LockHolder&) = delete;
	LockHolder(const LockHolder &) = delete;

	void lock() const
	{
		m_locker.lock();
	};
	bool try_lock() const
	{
		return m_locker.try_lock();
	};
	void unlock() const
	{
		m_locker.unlock();
	};
};
