#pragma once
#include <cstdint>
#include "../Lib.Base/locker.h"

class CommonSampleBuffer
{
	int                m_bufferSize = 1920*5 ;// 
	const int          m_sampleDepth;
	int   *            m_samplesTbl32 = nullptr;
	uint64_t	       m_nbSampleInTbl = 0;
	uint64_t	       m_nbSampleUsed = 0;
	unsigned char*	   m_pBuffer = nullptr;
	Locker			   m_criSection;
	uint32_t		   m_monoCnt;

	void _Init();

	CommonSampleBuffer();
public:
	CommonSampleBuffer(uint32_t sampleDepth/*in unsigned chars*/, uint32_t monoCnt);
	~CommonSampleBuffer();

	uint32_t getNbRemainingSamples();
	uint32_t pushSample32(int * sample, int nbSample);
	uint32_t getSample32(int * sample, int nbSample);
};
