#include "audioFrame.h"
#include <malloc.h>

AudioFrame::AudioFrame(int  _monoCnt)
{
	init(_monoCnt);
}

AudioFrame::~AudioFrame()
{
	free(m_raw);
	m_raw = nullptr;
}

int AudioFrame::init(uint32_t _nbMonoCnl, uint32_t _nbBitPerSample, uint32_t _nbSample)
{
	m_monoCnt = _nbMonoCnl;
	m_nbBitPerSample = _nbBitPerSample;
	SetSampleCnt(_nbSample);
	return 0;
}

unsigned char * AudioFrame::getRaw() const
{
	return m_raw + sizeof(header);
}

bool AudioFrame::isMute() const
{
	if (m_raw == nullptr)
		return true;
	uint8_t* pBuf = m_raw + sizeof(header);
	header* s = (header *)m_raw;
	for (uint32_t i = 0; i < s->rawSize; i++)
	{
		if (pBuf[i])
			return false;
	}
	return true;
}

unsigned long AudioFrame::getDataSize() const
{
	if (m_raw)
	{
		header* s = (header *)m_raw;
		return s->rawSize;
	}
	return 0;
}

unsigned char* AudioFrame::getRawWithHeader() const
{
	return m_raw;
}

unsigned long AudioFrame::getRawWithHeaderSize() const
{
	if (m_raw)
	{
		header * s = (header *)m_raw;
		return s->rawSize + sizeof(header);
	}
	return 0;
}

int AudioFrame::setBufferSize(unsigned long _rawSize)
{
	if (m_raw && ((header*)m_raw)->rawSize == _rawSize)
		return 0;
	m_sampleCnt = _rawSize / m_monoCnt / (m_nbBitPerSample / 8);
	if (m_raw)
	{
		if (m_totalSize >= (_rawSize + sizeof(header)))
		{
			((header*)m_raw)->rawSize = _rawSize;
			return 0;
		}
		free(m_raw);
		m_raw = nullptr;
	}
	m_totalSize = _rawSize + sizeof(header);
	m_raw = (uint8_t*)malloc(m_totalSize);
	((header*)m_raw)->rawSize = _rawSize;

	return 0;
}

void AudioFrame::setPts(double pts)
{
	m_ptsFrame = pts;
}

double AudioFrame::getPts() const
{
	return m_ptsFrame;
}

void AudioFrame::setTC(const Timecode & _tc)
{
	m_tc = _tc;
}

Timecode AudioFrame::getTC() const
{
	return m_tc;
}

void AudioFrame::setTimeID(const TimeIDFrame& _timeID)
{
	m_timeID = _timeID;
}

TimeIDFrame AudioFrame::getTimeID() const
{
	return m_timeID;
}

int AudioFrame::getTotalSize() const
{
	return m_totalSize;
}

void AudioFrame::setToMute(uint32_t _size) const
{
	if (m_raw == nullptr)
		return;
	if (_size == 0 || _size > getDataSize())
		memset(getRaw(), 0, getDataSize());
	else
		memset(getRaw(), 0, _size);
}

void AudioFrame::SetMonoCnt(uint32_t cnt)
{
	if (cnt == 0)
		return;

	header* s = (header *)m_raw;
	m_monoCnt = cnt;
	uint32_t size = cnt * m_sampleCnt * (m_nbBitPerSample / 8);
	if (s == nullptr || size != s->rawSize)
		setBufferSize(size);
}

void AudioFrame::SetSampleCnt(uint32_t sampleCnt)
{
	m_sampleCnt = sampleCnt;
	uint32_t size = sampleCnt * (m_nbBitPerSample / 8) * m_monoCnt;
	setBufferSize(size);
}

unsigned long AudioFrame::getBufferTotalSize() const
{
	return m_totalSize - sizeof(header);
}

bool CAudioSampleNbCycle::SetFrameRate(uint32_t _frameRate)
{
	m_SampleNbPosInCycle = 0;
	if (_frameRate != 59 && _frameRate != 29)
	{
		m_pSampleNbData = nullptr;
		return false;
	}
	if (_frameRate == 59)
	{
		m_pSampleNbData = (uint32_t*)Audio59SampleTable;
		m_nbSampleOfCycle = sizeof(Audio59SampleTable) / sizeof(uint32_t);
	}
	else
	{
		m_pSampleNbData = (uint32_t*)Audio29SampleTable;
		m_nbSampleOfCycle = sizeof(Audio29SampleTable) / sizeof(uint32_t);
	}
	return  true;
}

uint32_t CAudioSampleNbCycle::GetSampleNb()
{
	if (!m_pSampleNbData)
		return 0;
	uint32_t nb = m_pSampleNbData[m_SampleNbPosInCycle];
	return nb;
}

void CAudioSampleNbCycle::MoveNext()
{
	if (!m_pSampleNbData || m_nbSampleOfCycle == 0)
		return;
	m_SampleNbPosInCycle = ++m_SampleNbPosInCycle % m_nbSampleOfCycle;
}