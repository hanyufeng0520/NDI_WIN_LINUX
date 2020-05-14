#include "platform.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/time.h>
#endif _MSC_VER

void getCPUfreq(cpuFreq& _freq)
{
#ifdef _MSC_VER
	QueryPerformanceFrequency((LARGE_INTEGER*)&_freq);
#else
	_freq = 1000000;
#endif _MSC_VER
}

void getTickCount(cpuTick& _tick)
{
#ifdef _MSC_VER
	QueryPerformanceCounter((LARGE_INTEGER*)&_tick);
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	_tick = now.tv_sec + now.tv_usec / 1000000.0;
#endif _MSC_VER
}

cpuTick getTickCount()
{
	cpuTick ret;
	getTickCount(ret);
	return ret;
}

