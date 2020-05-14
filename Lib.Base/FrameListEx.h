#pragma once
#include "FrameList.h"

template <class T> class FrameListEx :public FrameList<T>
{
public:
	T getDataRef(int i)
	{
		LockHolder locker(this->m_dataListLock);
		T _elem = nullptr;
		auto iter = this->m_dataList.begin();
		auto end = this->m_dataList.end();
		int nSize = this->m_dataList.size();
		int nIndex = 0;
		for (iter; iter != end; ++iter)
		{
			if (nIndex++ == i)
			{
				_elem = *iter;
				break;
			}
		}
		return _elem;
	}
	void RemoveDataItem(T& _elem)
	{
		LockHolder locker(this->m_dataListLock);
		auto iter = find(this->m_dataList.begin(), this->m_dataList.end(), _elem);
		if (iter != this->m_dataList.end())
			this->m_dataList.erase(iter);
	}

	T getEmptyRef(int i)
	{
		LockHolder locker(this->m_emptyListLock);
		T _elem = nullptr;
		auto iter = this->m_emptyList.begin();
		auto end = this->m_emptyList.end();
		int nIndex = 0;
		for (iter; iter != end; ++iter)
		{
			if (nIndex++ == i)
			{
				_elem = *iter;
				break;
			}
		}
		return _elem;
	}
};