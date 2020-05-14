#pragma once
#include "eventClock.h"
#include "IMasterClock.h"

class ManualClock : public IMasterClock
{
public:
	ManualClock();
	~ManualClock();
	void start(int _frameDen, int _frameNum) override;
	void setEventClock() override;
	bool waitEventClock(int nWaitTime = 50) override;
	void stop() override;

protected:
	EventClock m_event;
};
