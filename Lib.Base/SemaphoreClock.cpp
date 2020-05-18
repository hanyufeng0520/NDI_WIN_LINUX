#include "SemaphoreClock.h"
#include <climits>

SemaphoreClock::SemaphoreClock()
{
#ifdef _MSC_VER
	m_handle = ::CreateSemaphore(nullptr, 0, LONG_MAX, nullptr);
#else
	sem_init(&m_handle, 0, LONG_MAX);
#endif _MSC_VER
}

SemaphoreClock::~SemaphoreClock()
{
#ifdef _MSC_VER
	CloseHandle(m_handle);
#else
	sem_destroy(&m_handle);
#endif _MSC_VER	
}

void SemaphoreClock::raiseEvent()
{
#ifdef _MSC_VER
	ReleaseSemaphore(m_handle, 1, nullptr);
#else
	sem_post(&m_handle);
#endif _MSC_VER	
	
}

bool SemaphoreClock::waitEvent(int nTimeOut)
{
#ifdef _MSC_VER
	return WaitForSingleObject(m_handle, nTimeOut) == WAIT_OBJECT_0;
#else
	return sem_wait(&m_handle) == 0;//here we don't have time 
#endif _MSC_VER	
	
}