#include "GPUConfig.h"
#include <iterator>

GPUConfig::GPUConfig()
{
	defaultEncoder = 0;
	defaultPlayer = 0;
	for (int i = 0; i < std::size(m_GPUCnl); i++)
	{
		m_GPUCnl[i].maskPGM = 0;
		m_GPUCnl[i].maskPRV = 0;
		m_GPUCnl[i].maskRecoder = 0;
	}
}

GPUConfig::~GPUConfig()
{
}

void GPUConfig::setGPUforPGM(int _numPGM, int _numGPU)
{
	PGM[_numPGM] = _numGPU;
	m_GPUCnl[_numGPU].maskPGM |= 1 << _numPGM;
}

void GPUConfig::setGPUforPRV(int _numPRV, int _numGPU)
{
	PRV[_numPRV] = _numGPU;
	m_GPUCnl[_numGPU].maskPRV |= 1 << _numPRV;
}

int GPUConfig::getGPUidForEncoder(int _cnl)
{
	return Recoder[_cnl];
}

void GPUConfig::setGPUforEncoder(int _numEncoder, int _numGPU)
{
	Recoder[_numEncoder] = _numGPU;
	m_GPUCnl[_numGPU].maskRecoder |= 1 << _numEncoder;
}

void GPUConfig::cleanUnUsedCnl(int _activePlayer, int _activeEncoder)
{
	for (int i = 0; i < std::size(m_GPUCnl); i++)
	{
		m_GPUCnl[i].maskPGM &= _activePlayer;
		m_GPUCnl[i].maskPRV &= _activePlayer;
		m_GPUCnl[i].maskRecoder &= _activeEncoder;
	}
}