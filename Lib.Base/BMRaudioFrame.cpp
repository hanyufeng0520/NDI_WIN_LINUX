#include "BMRaudioFrame.h"
#include <malloc.h>

BMRaudioFrame::BMRaudioFrame()
	:m_totalSize(1920) //16 audio channels , and simple bit depth is 4 BYTES
{
	m_raw = (short*)malloc(m_totalSize);
	m_dataSize = m_totalSize;
}
void BMRaudioFrame::reAlloc(uint32_t size)
{
	if (m_raw)
		free(m_raw);

	m_raw = (short*)malloc(size);
	m_totalSize = size;
}
BMRaudioFrame ::~BMRaudioFrame()
{
	free(m_raw);
}

short* BMRaudioFrame::getRaw() const
{
	return m_raw;
}

bool BMRaudioFrame::isMute() const
{
	for (unsigned long i = 0; i < m_dataSize; i++)
	{
		if (m_raw[i])
			return false;
	}
	return true;
}

unsigned long BMRaudioFrame::getDataSize() const
{
	return m_dataSize;
}

void BMRaudioFrame::setBufferSize(uint32_t size)
{
	m_dataSize = size;
	if (m_dataSize > m_totalSize)
	{
		reAlloc(size);
	}
}

void BMRaudioFrame::setTimeStamp(uint32_t lTimeStamp)
{
	m_nTimeStamp = lTimeStamp;
}

uint32_t BMRaudioFrame::getTimeStamp() const
{
	return m_nTimeStamp;
}

void BMRaudioFrame::setToMute() const
{
	memset(m_raw, 0, m_totalSize);
}

void BMRaudioFrame::SetFrameID(uint64_t  nFrameID)
{
	m_nFrameID = nFrameID;
}

uint64_t BMRaudioFrame::GetFrameID() const
{
	return m_nFrameID;
}

unsigned long BMRaudioFrame::getBufferTotalSize() const
{
	return m_totalSize;
}