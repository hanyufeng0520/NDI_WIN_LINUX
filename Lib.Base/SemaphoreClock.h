#pragma once

#ifdef _MSC_VER
#include <Windows.h>
#else
#include<semaphore.h>
#endif _MSC_VER

class SemaphoreClock
{
#ifdef _MSC_VER
	void* m_handle = nullptr;
#else
	sem_t  m_handle;
#endif _MSC_VER
public:
	SemaphoreClock();
	~SemaphoreClock();

	void raiseEvent();
	bool waitEvent(int nTimeOut = 50);
};
