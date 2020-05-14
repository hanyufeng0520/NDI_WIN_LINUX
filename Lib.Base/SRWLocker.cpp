#include "SRWLocker.h"

SRWLocker::SRWLocker()
{
	InitializeSRWLock(&m_lock);
}

void SRWLocker::lock_r()
{
	AcquireSRWLockShared(&m_lock);
}

void SRWLocker::unlock_r()
{
	ReleaseSRWLockShared(&m_lock);
}

void SRWLocker::lock_w()
{
	AcquireSRWLockExclusive(&m_lock);
}

void SRWLocker::unlock_w()
{
	ReleaseSRWLockExclusive(&m_lock);
}

SRWLockHolder::SRWLockHolder(SRWLocker& _locker, bool read) : m_locker(_locker), m_read(read)
{
	if (m_read)
		m_locker.lock_r();
	else
		m_locker.lock_w();
}

SRWLockHolder::~SRWLockHolder()
{
	if (m_read)
		m_locker.unlock_r();
	else
		m_locker.unlock_w();
}

void SRWLockHolder::lock_r() const
{
	m_locker.lock_r();
}

void SRWLockHolder::unlock_r() const
{
	m_locker.unlock_r();
}

void SRWLockHolder::lock_w() const
{
	m_locker.lock_w();
}

void SRWLockHolder::unlock_w() const
{
	m_locker.unlock_w();
}