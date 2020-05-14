#pragma once
#include <cstdint>

class Affinily
{
	void	* m_token;
	unsigned long long  m_systemAffinityMask;

	int		getToken();
	bool	setPrivilege(bool _enable) const;
	void	changeOtherProcess(uint64_t _mask);

public:
	Affinily();
	~Affinily();

	void start(int _nbCoreReserved);
	void stop();
};
