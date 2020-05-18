#pragma once
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif _MSC_VER

class SRWLocker
{
#ifdef _MSC_VER
	SRWLOCK m_lock;
#else
	pthread_rwlock_t m_lock;
#endif _MSC_VER

public:
	SRWLocker();
	~SRWLocker();
	SRWLocker& operator=(const SRWLocker&) = delete;
	SRWLocker(const SRWLocker &) = delete;

	void lock_r();
	void unlock_r();
	void lock_w();
	void unlock_w();
};

class SRWLockHolder
{
	SRWLocker& m_locker;
	bool	   m_read;

public:
	SRWLockHolder(SRWLocker& _locker, bool read);
	~SRWLockHolder();
	SRWLockHolder& operator=(const SRWLockHolder&) = delete;
	SRWLockHolder(const SRWLockHolder &) = delete;

	void lock_r() const;
	void unlock_r() const;
	void lock_w() const;
	void unlock_w() const;
};