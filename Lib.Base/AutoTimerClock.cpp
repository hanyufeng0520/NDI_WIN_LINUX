#include "AutoTimerClock.h"
#include "platform.h"

AutoTimerClock::AutoTimerClock()
{
}

AutoTimerClock::~AutoTimerClock()
{
}

VOID CALLBACK AutoTimerClock::TimerRoutine(_In_ PVOID lpParameter, _In_ BOOLEAN )
{
	AutoTimerClock* pthis = (AutoTimerClock*)lpParameter;
	pthis->setEventClock();
}

void AutoTimerClock::start(int _frameDen, int _frameNum)
{
	CreateTimerQueueTimer(&m_hTimerRender, nullptr, TimerRoutine, (void*)this, 0, 1000 / (_frameNum / _frameDen), WT_EXECUTEDEFAULT);
}

void AutoTimerClock::stop()
{
	if (m_hTimerRender != nullptr)
	{
		DeleteTimerQueueTimer(nullptr, m_hTimerRender, INVALID_HANDLE_VALUE);
		m_hTimerRender = nullptr;
	}
}

void AutoTimerClock::setEventClock()
{
	m_event.raiseEvent();
}

bool AutoTimerClock::waitEventClock(int nWaitTime /*= 50*/)
{
	return m_event.waitEvent(nWaitTime);
}