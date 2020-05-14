#include "eventClock.h"
#include <Windows.h>

EventClock::EventClock(const char* _evtName)
{
	m_event = CreateEventA(nullptr, FALSE, FALSE, _evtName);
}

EventClock::EventClock()
{
	m_event = CreateEventA(nullptr, FALSE, FALSE, nullptr);
}

EventClock::~EventClock()
{
	if (m_event)
		CloseHandle(m_event);
}

void EventClock::raiseEvent() const
{
	if (m_event)
		SetEvent(m_event);
}

bool EventClock::waitEvent(int nWaitTime) const
{
	if (!m_event)
		return false;
	int nRet = WaitForSingleObject(m_event, nWaitTime);
	return nRet == WAIT_OBJECT_0;
}