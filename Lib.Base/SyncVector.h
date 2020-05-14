#pragma once
#include <vector>
#include "Lib.Base/SRWLocker.h"

template<class T>
class SyncVector
{
	std::vector<T>	m_vector;
	SRWLocker		m_lock;

public:
	SyncVector() = default;

	~SyncVector() = default;

	template<class... _Valty>
	void emplace_back(_Valty&&... _Val)
	{
		SRWLockHolder lock(m_lock, false);
		m_vector.emplace_back(std::forward<_Valty>(_Val)...);
	}

	bool empty() const
	{
		return m_vector.empty();
	}

	T front()
	{
		SRWLockHolder lock(m_lock, true);
		return m_vector.front();
	}

	T back()
	{
		SRWLockHolder lock(m_lock, true);
		return m_vector.back();
	}

	template <class Function>
	auto with_lock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_vector);
	}

	template <class Function>
	auto wlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_vector);
	}

	template <class Function>
	auto rlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, true);
		return function(m_vector);
	}

	void clear()
	{
		SRWLockHolder lock(m_lock, false);
		m_vector.clear();
	}

	uint32_t size() const
	{
		return m_vector.size();
	}

	T operator[](const int32_t& _Pos)
	{
		SRWLockHolder lock(m_lock, true);
		return m_vector[_Pos];
	}
};