#pragma once
#include <functional>
#include <map>
#include "Lib.Base/SRWLocker.h"

template<class _K, class _V, class _Pr = std::less<_K>>
class SyncMap
{
	std::map<_K, _V, _Pr>	m_map;
	SRWLocker				m_lock;

public:
	using iterator = typename std::map<_K, _V, _Pr>::iterator;
	using const_iterator = typename std::map<_K, _V, _Pr>::const_iterator;
	using value_type = typename std::map<_K, _V, _Pr>::value_type;
	using key_type = typename std::map<_K, _V, _Pr>::key_type;
	using size_type = typename std::map<_K, _V, _Pr>::size_type;

	SyncMap() = default;
	~SyncMap() = default;

	bool pop_front()
	{
		SRWLockHolder lock(m_lock, false);
		if (m_map.empty())
			return false;
		m_map.erase(m_map.begin());
		return true;
	}

	bool pop_begin(_V& _val)
	{
		SRWLockHolder lock(m_lock, false);
		if (m_map.empty())
			return false;
		auto iter = m_map.begin();
		_val = iter->second;
		m_map.erase(iter);
		return true;
	}

	void emplace(const std::pair<_K, _V>& _Val)
	{
		SRWLockHolder lock(m_lock, false);
		m_map.emplace(_Val);
	}

	template<class... _Valty>
	void emplace(_Valty&&... _Val)
	{
		SRWLockHolder lock(m_lock, false);
		m_map.emplace(std::forward<_Valty>(_Val)...);
	}

	template<class... _Valty>
	bool try_emplace(_Valty&&... _Val)
	{
		SRWLockHolder lock(m_lock, false);
		return m_map.try_emplace(std::forward<_Valty>(_Val)...).second;
	}

	void insert(std::initializer_list<value_type> _Ilist)
	{
		SRWLockHolder lock(m_lock, false);
		m_map.emplace(_Ilist);
	}

	iterator erase(iterator pos)
	{
		SRWLockHolder lock(m_lock, false);
		return m_map.erase(pos);
	}

	size_type erase(const key_type& key)
	{
		SRWLockHolder lock(m_lock, false);
		return m_map.erase(key);
	}

	void insert_or_assign(const _K& key, const _V& value)
	{
		SRWLockHolder lock(m_lock, false);
		m_map.insert_or_assign(key, value);
	}

	bool empty() const
	{
		return m_map.empty();
	}

	size_type size() const
	{
		return m_map.size();
	}

	void clear()
	{
		SRWLockHolder lock(m_lock, false);
		m_map.clear();
	}

	bool find(const key_type& key)
	{
		SRWLockHolder lock(m_lock, true);
		return m_map.find(key) != m_map.end();
	}

	bool find(const key_type& key, _V& value)
	{
		SRWLockHolder lock(m_lock, true);
		auto item = m_map.find(key);
		if (item == m_map.end())
			return false;
		value = item->second;
		return true;
	}

	_V& operator[](const key_type& _Keyval)
	{
		SRWLockHolder lock(m_lock, false);
		return m_map[_Keyval];
	}

	template <class Function>
	auto with_lock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_map);
	}

	template <class Function>
	auto wlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, false);
		return function(m_map);
	}

	template <class Function>
	auto rlock(Function&& function)
	{
		SRWLockHolder lock(m_lock, true);
		return function(m_map);
	}

	void remove_if(const std::function<bool(const key_type & key, const _V & value)>& predicate)
	{
		SRWLockHolder lock(m_lock, false);
		for (auto iter = m_map.begin(); iter != m_map.end();)
		{
			if (predicate(iter->first, iter->second))
				iter = m_map.erase(iter);
			else
				++iter;
		}
	}
};
