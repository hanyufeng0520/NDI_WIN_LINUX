#pragma once
#include <Windows.h>
#include <cstdint>

class CPUUsageHelper
{
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;

	static int64_t CompareFileTime(FILETIME time1, FILETIME time2);
public:
	CPUUsageHelper();
	~CPUUsageHelper() = default;

	void get_cpu_usage(int64_t& _cpu, int64_t& _cpuidle);
};
