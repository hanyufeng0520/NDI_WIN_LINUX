#include "CommonSampleBuffer.h"
#include <cstdio>
#include "../Lib.Config/IConfig.h"

CommonSampleBuffer::CommonSampleBuffer(uint32_t  sampleDepth, uint32_t monoCnt) :m_sampleDepth(sampleDepth),
m_monoCnt(monoCnt)
{
	_Init();
}

CommonSampleBuffer::CommonSampleBuffer() : m_sampleDepth(4), m_monoCnt(16)
{
	_Init();
}

void CommonSampleBuffer::_Init()
{
	m_pBuffer = new unsigned char[m_bufferSize*m_sampleDepth*m_monoCnt];
	m_samplesTbl32 = (int*)m_pBuffer;
	m_nbSampleInTbl = 0;
	m_nbSampleUsed = 0;
}

CommonSampleBuffer::~CommonSampleBuffer()
{
	if (m_pBuffer)
		delete[] m_pBuffer;
}

uint32_t CommonSampleBuffer::pushSample32(int * sample, int nbSample)
{
	LockHolder lock(m_criSection);

	uint64_t remainingSampleInTable = getNbRemainingSamples();
	if (nbSample + remainingSampleInTable > m_bufferSize)
	{
		//no enough free space for new data
		nbSample = m_bufferSize - remainingSampleInTable;
		//log
	}

	if ((m_nbSampleInTbl + nbSample) < m_bufferSize)
	{
		memcpy(&m_samplesTbl32[m_nbSampleInTbl*m_monoCnt], sample, nbSample * m_sampleDepth*m_monoCnt);
		m_nbSampleInTbl += nbSample;
	}
	else
	{
		int nbSample1 = m_bufferSize - m_nbSampleInTbl;
		int nbSample2 = nbSample - nbSample1;

		memcpy(&m_samplesTbl32[m_nbSampleInTbl*m_monoCnt], sample, nbSample1 * m_sampleDepth*m_monoCnt);
		memcpy(&m_samplesTbl32[0], &sample[nbSample1*m_monoCnt], nbSample2 * m_sampleDepth*m_monoCnt);
		m_nbSampleInTbl = nbSample2;
	}

	remainingSampleInTable = getNbRemainingSamples();
	return nbSample;
}

uint32_t CommonSampleBuffer::getNbRemainingSamples()
{
	uint32_t remainingSampleInTable = m_nbSampleInTbl - m_nbSampleUsed;								// normal case
	if (m_nbSampleInTbl < m_nbSampleUsed)
		remainingSampleInTable = m_nbSampleInTbl + m_bufferSize - m_nbSampleUsed;						// not normal case , overlapped already
	return remainingSampleInTable;
}

uint32_t CommonSampleBuffer::getSample32(int * sample, int nbSample)
{
	LockHolder lock(m_criSection);

	uint32_t remainingSampleInTable = getNbRemainingSamples();

	if (nbSample > remainingSampleInTable)
		nbSample = remainingSampleInTable;

	if ((m_nbSampleUsed + nbSample) >= m_bufferSize)
	{
		int nbSample1 = m_bufferSize - m_nbSampleUsed;
		int nbSample2 = nbSample - nbSample1;

		memcpy(sample, &m_samplesTbl32[m_nbSampleUsed*m_monoCnt], nbSample1 * m_sampleDepth*m_monoCnt);
		memcpy(&sample[nbSample1*m_monoCnt], &m_samplesTbl32[0], nbSample2 * m_sampleDepth*m_monoCnt);
		m_nbSampleUsed = nbSample2;
	}
	else
	{
		memcpy(sample, &m_samplesTbl32[m_nbSampleUsed*m_monoCnt], nbSample * m_sampleDepth*m_monoCnt);
		m_nbSampleUsed += nbSample;
	}

	remainingSampleInTable = getNbRemainingSamples();
	return nbSample;
}