#pragma once
#include <queue>
#include "locker.h"

template<class T>
class SyncQueue
{
	std::queue<T>	m_queue;
	Locker			m_lock;

public:
	SyncQueue() = default;

	~SyncQueue() = default;

	void pop()
	{
		LockHolder lock(m_lock);
		m_queue.pop();
	}

	bool pop_begin(T& _val)
	{
		LockHolder lock(m_lock);
		if (m_queue.empty())
			return false;
		_val = m_queue.front();
		m_queue.pop();
		return true;
	}

	template <class Function>
	auto with_lock(Function&& function)
	{
		LockHolder lock(m_lock);
		return function(m_queue);
	}

	template<class... _Valty>
	void emplace(_Valty&&... _Val)
	{
		LockHolder lock(m_lock);
		m_queue.emplace(std::forward<_Valty>(_Val)...);
	}

	uint32_t size() const
	{
		return m_queue.size();
	}

	bool empty() const
	{
		return m_queue.empty();
	}

	void clear()
	{
		LockHolder lock(m_lock);
		while (!m_queue.empty())
			m_queue.pop();
	}
};
