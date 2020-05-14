#pragma once
#define WatchDogNotifyBackendFormat		"B595A554-2999-4EBA-8D34-6221F544F267"
#define BackendNotifyWatchDogFormatEnd	"6CB96EC2-8798-4C10-A7CB-707977343999"
#define WatchDogNotifyBackendShutdown	"C72823F2-FD20-40FD-876B-3D2ECBB3D79E"
#define WatchDogNotifyHideBackend		"26D8493C-9454-40BA-9D66-1E8145AEB8C2"
#define WatchDogNotifyDisplayBackend	"A73D4A86-0E3D-41ED-9148-DDABCDF9B93B"
#define GPUDownNotifyBackendShutdown	"10DE38FA-677B-4C36-AE6D-FF64AC1E918D"

class EventClock
{
	void* m_event = nullptr;
public:
	EventClock();
	explicit EventClock(const char* _evtName);
	~EventClock();

	void raiseEvent() const;
	bool waitEvent(int nWaitTime = 100) const;
};
