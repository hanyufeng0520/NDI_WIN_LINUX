#pragma once
#include <cstdint>
#include "platform.h"
#include "eventClock.h"
#include "easyThread.h"
#include "IMasterClock.h"

class AutoLoopClock : public IMasterClock, public EasyThread
{
	cpuFreq    m_freq;
	uint64_t   m_tickPerFrame;
	EventClock m_event;

	void callBack() override;
	void setEventClock() override; //Don't call this....

public:
	AutoLoopClock();
	~AutoLoopClock();
	void start(int _frameDen, int _frameNum) override;
	bool waitEventClock(int nWaitTime = 50) override;
	void stop() override;
};
