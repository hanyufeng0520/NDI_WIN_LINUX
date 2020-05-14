#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <functional>
#include "Lib.Base/locker.h"

class DllParser
{
public:

	DllParser() :m_library(nullptr)
	{
	}

	~DllParser()
	{
		unload();
	}

	bool load(const wchar_t* dllPath)
	{
		if (m_library == nullptr)
			m_library = LoadLibraryW(dllPath);
		return nullptr != m_library;
	}

	bool unload()
	{
		if (m_library == nullptr)
			return true;

		if (!FreeLibrary(m_library))
			return false;

		m_library = nullptr;
		return true;
	}

	template <typename T>
	std::function<T> get_function(const char* func_name)
	{
		if (m_library == nullptr)
			return nullptr;

		LockHolder lock(m_locker);
		auto it = m_map.find(func_name);
		if (it == m_map.end())
		{
			auto addr = GetProcAddress(m_library, func_name);
			if (addr == nullptr)
				return nullptr;
			m_map.emplace(func_name, addr);
			it = m_map.find(func_name);
		}

		return std::function<T>((T*)(it->second));
	}

	template <typename T, typename... Args>
	auto invoke(const char* func_name, Args&&... args)
	{
		auto fun = get_function<T>(func_name);
		if (fun == nullptr)
			return false;
		return fun(std::forward<Args>(args)...);
	}

private:
	HMODULE							            m_library;
	std::map<std::string, FARPROC, std::less<>>	m_map;
	Locker							            m_locker;
};