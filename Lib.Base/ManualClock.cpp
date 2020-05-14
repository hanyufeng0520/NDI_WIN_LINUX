#include "ManualClock.h"

ManualClock::ManualClock()
{
}

ManualClock::~ManualClock()
{
}

void ManualClock::start(int _frameDen, int _frameNum)
{

}

void ManualClock::setEventClock()
{
	m_event.raiseEvent();
}

bool ManualClock::waitEventClock(int nWaitTime)
{
	return m_event.waitEvent(nWaitTime);
}

void ManualClock::stop()
{
}