#include "SemaphoreClock.h"
#include <Windows.h>
#include <climits>

SemaphoreClock::SemaphoreClock()
{
	m_handle = ::CreateSemaphore(nullptr, 0, LONG_MAX, nullptr);
}

SemaphoreClock::~SemaphoreClock()
{
	CloseHandle(m_handle);
}

void SemaphoreClock::raiseEvent() const
{
	ReleaseSemaphore(m_handle, 1, nullptr);
}

bool SemaphoreClock::waitEvent(int nTimeOut) const
{
	return WaitForSingleObject(m_handle, nTimeOut) == WAIT_OBJECT_0;
}