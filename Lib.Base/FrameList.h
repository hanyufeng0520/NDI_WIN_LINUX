#pragma once
#include "locker.h"
#include <list>
#include "SemaphoreClock.h"

template <class T>
class FrameList
{
protected:
	std::list<T>	m_emptyList;
	std::list<T>	m_dataList;
	Locker			m_emptyListLock;
	Locker			m_dataListLock;
	SemaphoreClock	m_emptyListSemaphore;
	SemaphoreClock	m_dataListSemaphore;
public:
	virtual ~FrameList() = default;

	bool pop_front_empty(T& _elem)
	{
		LockHolder locker(m_emptyListLock);
		if (!m_emptyList.empty())
		{
			_elem = m_emptyList.front();
			m_emptyList.pop_front();
			return true;
		}
		return false;
	}

	bool pop_front_data(T& _elem)
	{
		LockHolder locker(m_dataListLock);
		if (!m_dataList.empty())
		{
			_elem = m_dataList.front();
			m_dataList.pop_front();
			return true;
		}
		return false;
	}

	bool pop_end_data(T& _elem)
	{
		LockHolder locker(m_dataListLock);
		if (!m_dataList.empty())
		{
			_elem = m_dataList.back();
			m_dataList.pop_back();
			return true;
		}
		return false;
	}

	bool getHeadDataRef(T& _elem)
	{
		LockHolder locker(m_dataListLock);
		if (!m_dataList.empty())
		{
			_elem = m_dataList.front();
			return true;
		}
		return false;
	}

	void push_back_data(T& _elem)
	{
		LockHolder locker(m_dataListLock);
		m_dataList.emplace_back(_elem);
		m_dataListSemaphore.raiseEvent();
	}

	void push_back_data(T&& _elem)
	{
		LockHolder locker(m_dataListLock);
		m_dataList.emplace_back(std::forward<T>(_elem));
		m_dataListSemaphore.raiseEvent();
	}

	void push_front_data(T& _elem)
	{
		LockHolder locker(m_dataListLock);
		m_dataList.emplace_front(_elem);
		m_dataListSemaphore.raiseEvent();
	}

	void push_front_data(T&& _elem)
	{
		LockHolder locker(m_dataListLock);
		m_dataList.emplace_front(std::forward<T>(_elem));
		m_dataListSemaphore.raiseEvent();
	}

	bool wait_semaphore_data(int _time_out = 50)
	{
		return m_dataListSemaphore.waitEvent(_time_out);
	}

	bool wait_semaphore_empty(int _time_out = 50)
	{
		return m_emptyListSemaphore.waitEvent(_time_out);
	}

	void push_back_empty(T& _elem)
	{
		LockHolder locker(m_emptyListLock);
		m_emptyList.emplace_back(_elem);
		m_emptyListSemaphore.raiseEvent();
	}

	void push_back_empty(T&& _elem)
	{
		LockHolder locker(m_emptyListLock);
		m_emptyList.emplace_back(std::forward<T>(_elem));
		m_emptyListSemaphore.raiseEvent();
	}

	uint32_t get_size_empty()
	{
		return m_emptyList.size();
	}

	uint32_t get_size_data()
	{
		return m_dataList.size();
	}
};
