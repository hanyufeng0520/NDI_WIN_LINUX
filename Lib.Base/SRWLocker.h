#pragma once
#include <Windows.h>

class SRWLocker
{
	SRWLOCK m_lock;

public:
	SRWLocker();
	~SRWLocker() = default;
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