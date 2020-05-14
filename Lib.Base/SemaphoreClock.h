#pragma once

class SemaphoreClock
{
	void* m_handle = nullptr;
public:
	SemaphoreClock();
	~SemaphoreClock();

	void raiseEvent() const;
	bool waitEvent(int nTimeOut = 50) const;
};
