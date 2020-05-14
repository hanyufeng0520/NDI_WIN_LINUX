#pragma once
#include <set>
#include <functional>
#include "Lib.Base/SRWLocker.h"

template<class _K, class _Pr = std::less<_K>>
class SyncSet
{
	std::set<_K, _Pr>	m_set;
	SRWLocker			m_lock;

public:
	template <class... _Valty>
	void emplace(_Valty&&... _Val)
	{
		SRWLockHolder lock(m_lock, false);
		m_set.emplace(std::forward<_Valty>(_Val)...);
	}

	bool empty()
	{
		return m_set.empty();
	}

	size_t size() const
	{
		return m_set.size();
	}

	bool find(const _K& key)
	{
		SRWLockHolder lock(m_lock, true);
		return m_set.find(key) != m_set.end();
	}

	void clear()
	{
		SRWLockHolder lock(m_lock, false);
		m_set.clear();
	}

	void erase(const _K& _Keyval)
	{
		SRWLockHolder lock(m_lock, false);
		m_set.erase(_Keyval);
	}

	template <class Function>
	auto rlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, true);
		return function(m_set);
	}

	template <class Function>
	auto wlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_set);
	}

	template <class Function>
	auto with_lock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_set);
	}

	_K front()
	{
		SRWLockHolder lock(m_lock, true);
		return *(m_set.begin());
	}

	_K back()
	{
		SRWLockHolder lock(m_lock, true);
		return *(m_set.rbegin());
	}
};
