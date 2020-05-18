#include "SRWLocker.h"

SRWLocker::SRWLocker()
{
#ifdef _MSC_VER
	InitializeSRWLock(&m_lock);
#else
	pthread_rwlock_init(&m_lock, NULL);
#endif _MSC_VER
}
SRWLocker::~SRWLocker()
{
#ifdef _MSC_VER	
#else
	pthread_rwlock_destroy(&m_lock);
#endif _MSC_VER
}
void SRWLocker::lock_r()
{
#ifdef _MSC_VER
	AcquireSRWLockShared(&m_lock);
#else
	pthread_rwlock_rdlock(&m_lock);
#endif _MSC_VER	
}

void SRWLocker::unlock_r()
{
#ifdef _MSC_VER
	ReleaseSRWLockShared(&m_lock);
#else
	pthread_rwlock_unlock(&m_lock);
#endif _MSC_VER	
}

void SRWLocker::lock_w()
{
#ifdef _MSC_VER
	AcquireSRWLockExclusive(&m_lock);
#else
	pthread_rwlock_wrlock(&m_lock);
#endif _MSC_VER	
}

void SRWLocker::unlock_w()
{
#ifdef _MSC_VER
	ReleaseSRWLockExclusive(&m_lock);
#else
	pthread_rwlock_unlock(&m_lock);
#endif _MSC_VER

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