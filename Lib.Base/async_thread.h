#pragma once
#include <thread>
#include <Windows.h>

struct async_future
{
	std::thread* handle = nullptr;

	bool valid() const { return handle != nullptr; }

	void get()
	{
		if (valid())
		{
			handle->join();
			delete handle;
			handle = nullptr;
		}
	}
};

enum class thread_priority
{
	lowest,
	below_normal,
	normal,
	above_normal,
	highest,
	real_time
};

template<class _Fty, class... _ArgTypes>
async_future async_thread(thread_priority _priority, _Fty&& _Fnarg, _ArgTypes&&... _Args)
{
	async_future future;
	future.handle = new std::thread(std::forward<_Fty>(_Fnarg), std::forward<_ArgTypes>(_Args)...);
	if (future.handle != nullptr)
	{
		int priority = THREAD_PRIORITY_NORMAL;
		switch (_priority)
		{
		case thread_priority::lowest:		priority = THREAD_PRIORITY_LOWEST;		  break;
		case thread_priority::below_normal: priority = THREAD_PRIORITY_BELOW_NORMAL;  break;
		case thread_priority::above_normal: priority = THREAD_PRIORITY_ABOVE_NORMAL;  break;
		case thread_priority::real_time:	priority = THREAD_PRIORITY_TIME_CRITICAL; break;
		case thread_priority::highest:		priority = THREAD_PRIORITY_HIGHEST;		  break;
		case thread_priority::normal:
		default:							priority = THREAD_PRIORITY_NORMAL;		  break;
		}
		SetThreadPriority(future.handle->native_handle(), priority);
	}
	return future;
}

template<class _Fty, class... _ArgTypes>
async_future async_thread(thread_priority _priority, const wchar_t* _thread_name, _Fty&& _Fnarg, _ArgTypes&&... _Args)
{
	async_future future = async_thread(_priority, std::forward<_Fty>(_Fnarg), std::forward<_ArgTypes>(_Args)...);
	if (future.handle != nullptr && _thread_name != nullptr)
		SetThreadDescription(future.handle->native_handle(), _thread_name);
	return future;
}