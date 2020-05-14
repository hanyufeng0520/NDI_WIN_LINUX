#include "CPUUsageHelper.h"

int64_t CPUUsageHelper::CompareFileTime(FILETIME time1, FILETIME time2)
{
	int64_t a = int64_t(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	int64_t b = int64_t(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return (b - a);
}

CPUUsageHelper::CPUUsageHelper()
{
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
}

void CPUUsageHelper::get_cpu_usage(int64_t& _cpu, int64_t& _cpuidle)
{
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	int64_t idle = CompareFileTime(preidleTime, idleTime);
	int64_t kernel = CompareFileTime(prekernelTime, kernelTime);
	int64_t user = CompareFileTime(preuserTime, userTime);
	if (kernel + user == 0)
		return;

	_cpu = (kernel + user - idle) * 100 / (kernel + user);
	_cpuidle = (idle) * 100 / (kernel + user);

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
}