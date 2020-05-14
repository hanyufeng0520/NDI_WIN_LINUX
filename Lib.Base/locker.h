#pragma once
#include <Windows.h>

class Locker
{
	CRITICAL_SECTION	 m_locker;

public:
	Locker() {
		InitializeCriticalSection(&m_locker);
	};
	~Locker() {
		DeleteCriticalSection(&m_locker);
	};

	Locker& operator=(const Locker&) = delete;
	Locker(const Locker &) = delete;

	void lock()
	{
		EnterCriticalSection(&m_locker);
	};
	bool try_lock()
	{
		return TryEnterCriticalSection(&m_locker);
	};
	void unlock() {
		LeaveCriticalSection(&m_locker);
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
