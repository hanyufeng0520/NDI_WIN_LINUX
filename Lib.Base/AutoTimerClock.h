#pragma once
#include <cstdint>
#include "eventClock.h"
#include "IMasterClock.h"
#include <windows.h>

class AutoTimerClock : public IMasterClock
{
	EventClock m_event;
	void*      m_hTimerRender = nullptr;

	static void CALLBACK TimerRoutine(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired);
	void setEventClock() override; //Don't call this....

public:
	AutoTimerClock();
	~AutoTimerClock();
	void start(int _frameDen, int _frameNum) override;
	bool waitEventClock(int nWaitTime = 50) override;
	void stop() override;
};
