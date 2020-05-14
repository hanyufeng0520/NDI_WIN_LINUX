#include "AutoLoopClock.h"
#include "platform.h"
#include <windows.h>

AutoLoopClock::AutoLoopClock()
{
	m_tickPerFrame = 0;
	getCPUfreq(m_freq);
}

AutoLoopClock::~AutoLoopClock()
{
}

void AutoLoopClock::start(int _frameDen, int _frameNum)
{
	m_tickPerFrame = (m_freq * _frameDen) / _frameNum;
	startThread();
}

void AutoLoopClock::stop()
{
	stopThread(1000);
}

void AutoLoopClock::callBack()
{
	cpuTick tickIn, tickOut;
	getTickCount(tickIn);

	while (isRunning())
	{
		Sleep(1);
		getTickCount(tickOut);

		uint64_t diff = tickOut - tickIn;
		if (diff >= m_tickPerFrame)
		{
			tickIn += m_tickPerFrame;
			setEventClock();
			Sleep(1);
		}
	}
}

void AutoLoopClock::setEventClock()
{
	m_event.raiseEvent();
}

bool AutoLoopClock::waitEventClock(int nWaitTime /*= 50*/)
{
	return m_event.waitEvent(nWaitTime);
}