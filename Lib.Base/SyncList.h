#pragma once
#include <list>
#include <functional>
#include "SRWLocker.h"

template<class T>
class SyncList
{
	std::list<T>	m_list;
	SRWLocker		m_lock;

public:
	SyncList() = default;

	~SyncList() = default;

	template<class... _Valty>
	void emplace_back(_Valty&&... _Val)
	{
		SRWLockHolder lock(m_lock, false);
		m_list.emplace_back(std::forward<_Valty>(_Val)...);
	}

	template<class... _Valty>
	void emplace_front(_Valty&&... _Val)
	{
		SRWLockHolder lock(m_lock, false);
		m_list.emplace_front(std::forward<_Valty>(_Val)...);
	}

	template <class Function>
	auto with_lock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_list);
	}

	template <class Function>
	auto wlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_list);
	}

	template <class Function>
	auto rlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, true);
		return function(m_list);
	}

	void pop_front()
	{
		SRWLockHolder lock(m_lock, false);
		m_list.pop_front();
	}

	bool pop_begin(T& _val)
	{
		SRWLockHolder lock(m_lock, false);
		if (m_list.empty())
			return false;
		_val = m_list.front();
		m_list.pop_front();
		return true;
	}

	void pop_back()
	{
		SRWLockHolder lock(m_lock, false);
		m_list.pop_back();
	}

	T front()
	{
		SRWLockHolder lock(m_lock, true);
		return m_list.front();
	}

	T back()
	{
		SRWLockHolder lock(m_lock, true);
		return m_list.back();
	}

	uint32_t size() const
	{
		return (uint32_t)m_list.size();
	}

	bool empty() const
	{
		return m_list.empty();
	}

	void clear()
	{
		SRWLockHolder lock(m_lock, false);
		return m_list.clear();
	}

	bool find(const T& item)
	{
		SRWLockHolder lock(m_lock, true);
		return any_of(begin(m_list), end(m_list), [&](const T& obj) { return item == obj; });
	}

	bool find(const std::function<bool(const T&)>& predicate)
	{
		SRWLockHolder lock(m_lock, true);
		return any_of(begin(m_list), end(m_list), [&](const T& obj) { return predicate(obj); });
	}

	bool remove(const T& item)
	{
		SRWLockHolder lock(m_lock, false);
		m_list.remove(item);
		return true;
	}
};
