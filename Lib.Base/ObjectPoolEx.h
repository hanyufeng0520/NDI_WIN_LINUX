#pragma once
#include "SyncList.h"
#include <memory>

template <class T>
class ObjectPoolEx
{
	SyncList<T *> m_data_list;

	template<typename X, typename... Args>
	void addOneFree(Args&&... args)
	{
		T* p = new X(std::forward<Args>(args)...);
		if (p != nullptr)
		{
			m_data_list.emplace_back(p);
		}
	}

	template<typename X, typename... Args>
	void SetMaxSize(int _size, Args&&... args)
	{
		for (int i = 0; i < _size; i++)
			addOneFree<X>(std::forward<Args>(args)...);
	}
public:
	ObjectPoolEx() = default;
	~ObjectPoolEx() = default;

	template<typename X, typename... Args>
	void initialize(int _size, Args&&... args)
	{
		SetMaxSize<X>(_size, std::forward<Args>(args)...);
	}

	bool getNew(T* & _elem)
	{
		T* item = nullptr;
		if (m_data_list.pop_begin(item))
		{
			_elem = item;
			return true;
		}

		_elem = nullptr;
		return false;
	}

	void release(T* _elem)
	{
		if (_elem != nullptr)
			m_data_list.emplace_back(_elem);
	}

	uint32_t size()
	{
		return m_data_list.size();
	}

	//Managed API
	bool getNew(std::shared_ptr<T>& _elem)
	{
		T* frame = nullptr;
		getNew(frame);
		if (frame == nullptr)
		{
			_elem = nullptr;
			return false;
		}

		_elem = std::shared_ptr<T>(frame, [&](T*& ptr)
			{
				release(ptr);
			});
		return true;
	}
};
