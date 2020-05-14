#pragma once
#include <windows.h>

class EasyThread
{
	void *		  m_thread = nullptr;
	unsigned long m_threadId = 0;
	volatile bool m_exitRequested = false;

	static unsigned long __stdcall theThread(void* lpParam)
	{
		((EasyThread*)lpParam)->callBack();
		return 0;
	};

protected:
	virtual void callBack() = 0;

	enum class Priority
	{
		normal,
		above_normal,
		real_time
	};

	void startThread(Priority _priority = Priority::normal)
	{
		m_exitRequested = false;
		m_thread = CreateThread(nullptr, 0, theThread, (void*)this, CREATE_SUSPENDED, &m_threadId);
		switch (_priority)
		{
		case Priority::above_normal:	SetThreadPriority(m_thread, THREAD_PRIORITY_ABOVE_NORMAL); break;
		case Priority::real_time:		SetThreadPriority(m_thread, THREAD_PRIORITY_TIME_CRITICAL); break;
		case Priority::normal:
		default:						SetThreadPriority(m_thread, THREAD_PRIORITY_NORMAL); break;
		}
		ResumeThread(m_thread);
	};

	void stopThread(unsigned long _timeOut = 0xFFFFFFFF)
	{
		m_exitRequested = true;
		if (m_thread)
		{
			WaitForMultipleObjects(1, &m_thread, TRUE, _timeOut);
			CloseHandle(m_thread);
			m_thread = nullptr;
		}
	};

	bool isRunning() const {
		return !m_exitRequested;
	};

public:
	EasyThread() = default;
	virtual ~EasyThread() = default;
};
