#pragma once

enum class ClockType
{
	ManualClock,
	AutoLoopClock,
	AutoTimerClock
};

class IMasterClock
{
protected:
	IMasterClock() = default;
	virtual ~IMasterClock() = default;
public:
	static IMasterClock* CreateInstance(ClockType _type);
	static void releaseInstance(IMasterClock*& pInstance);

	virtual void start(int _frameDen, int _frameNum) = 0;
	virtual void setEventClock() = 0;
	virtual bool waitEventClock(int nWaitTime = 50) = 0;
	virtual void stop() = 0;
};
