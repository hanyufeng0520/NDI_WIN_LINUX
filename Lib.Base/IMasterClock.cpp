#include "IMasterClock.h"
#include "AutoLoopClock.h"
#include "ManualClock.h"
#include "AutoTimerClock.h"

IMasterClock* IMasterClock::CreateInstance(ClockType _type)
{
	switch (_type)
	{
	case ClockType::ManualClock: 	return new ManualClock();
	case ClockType::AutoLoopClock: return new AutoLoopClock();
	case ClockType::AutoTimerClock:return new AutoTimerClock();
	default:						return nullptr;
	}
}

void IMasterClock::releaseInstance(IMasterClock*& pInstance)
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}