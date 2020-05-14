#include "affinily.h"
#include <Windows.h>
#include <psapi.h>

Affinily::Affinily()
{
}

Affinily::~Affinily()
{
}

void Affinily::start(int _nbCoreReserved)
{
	uint64_t processAffinityMask;
	GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &m_systemAffinityMask);

	int nbTotalCore = 0;
	for (int i = 1; (i&m_systemAffinityMask); i *= 2)
		nbTotalCore++;

	if (_nbCoreReserved >= nbTotalCore)
		_nbCoreReserved = 0;

	int nbCoreForOtherProcess = nbTotalCore - _nbCoreReserved;

	uint64_t newMask = 0;
	for (int i = 0; i < _nbCoreReserved; i++)
		newMask = (newMask << 1) | 1;

	newMask = newMask << nbCoreForOtherProcess;

	getToken();

	changeOtherProcess(m_systemAffinityMask & (~newMask));

	SetProcessAffinityMask(GetCurrentProcess(), newMask);
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
}

void Affinily::stop()
{
	SetPriorityClass(GetCurrentProcess(), THREAD_PRIORITY_NORMAL);
	SetProcessAffinityMask(GetCurrentProcess(), m_systemAffinityMask);
	changeOtherProcess(m_systemAffinityMask);
	setPrivilege(false);														// disable SeDebugPrivilege
	CloseHandle(m_token);
}

int Affinily::getToken()
{
	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &m_token))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			if (!ImpersonateSelf(SecurityImpersonation))
				return -1;

			if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &m_token))
				return -1;
		}
		else
			return -1;
	}

	if (!setPrivilege(true))
	{
		CloseHandle(m_token);										// close token handle
		return -1;
	}

	return 0;
}

bool Affinily::setPrivilege(bool _enable) const
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);

	if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) return false;

	// first pass.  get current privilege setting

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(m_token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);

	if (GetLastError() != ERROR_SUCCESS) return false;

	// second pass.  set privilege based on previous setting

	tpPrevious.PrivilegeCount = 1;
	tpPrevious.Privileges[0].Luid = luid;

	if (_enable)
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	else
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &tpPrevious.Privileges[0].Attributes);

	AdjustTokenPrivileges(m_token, FALSE, &tpPrevious, cbPrevious, nullptr, nullptr);

	if (GetLastError() != ERROR_SUCCESS) return false;

	return true;
}

void Affinily::changeOtherProcess(uint64_t _mask)
{
	DWORD aProcesses[1024], cbNeeded;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return;

	DWORD cProcesses = cbNeeded / sizeof(DWORD);																	// Calculate how many process identifiers were returned.
	for (i = 0; i < cProcesses; i++)																		// Print the name and process identifier for each process.
	{
		if (aProcesses[i] != 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i]);

			if (hProcess != nullptr)
			{
				SetProcessAffinityMask(hProcess, _mask);
				CloseHandle(hProcess);
			}
		}
	}
}