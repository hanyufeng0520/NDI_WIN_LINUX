#pragma once
#include "locker.h"

template<class T>
class DualObjectWrapper
{
	T		m_object[2];
	int		m_object_idx = 0;
	Locker	m_lock;

public:
	void	safe_set(const T& _object)
	{
		LockHolder lock(m_lock);
		unsafe_set(_object);
	}

	void	unsafe_set(const T& _object)
	{
		int idx = m_object_idx;
		m_object[idx] = _object;
		m_object_idx ^= 1;
	}

	T	get()
	{
		int idx = 1 - m_object_idx;
		return m_object[idx];
	}
};
