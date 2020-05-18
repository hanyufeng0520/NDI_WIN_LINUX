#include "compVideoFrame.h"


CompVideoFrame::CompVideoFrame()
{
	m_totalSize = sizeof(header);
	m_frame = nullptr;
	m_nTimeStamp = 0;
}

CompVideoFrame::~CompVideoFrame()
{
	delete[] m_frame;
}

unsigned char* CompVideoFrame::getRaw() const
{
	return &m_frame[sizeof(header)];
}

uint32_t CompVideoFrame::getRawSize() const
{
	return getDataSize();
}

uint32_t CompVideoFrame::getDataSize() const
{
	if (m_frame)
	{
		header * s = (header *)m_frame;
		return s->rawSize;
	}
	return 0;
}

int	CompVideoFrame::saveToFile(const char* _fileName) const
{
	/*
	FILE* fp = nullptr;

	fopen_s(&fp, _fileName, "wb");
	if (fp == nullptr)
		return -1;

	fwrite(getRaw(), getDataSize(), 1, fp);

	fclose(fp);
	*/
	return 0;
}

unsigned char* CompVideoFrame::getRawWithSize() const
{
	return &m_frame[sizeof(header) - sizeof(uint32_t)];
}

unsigned char* CompVideoFrame::getRawWithHeader() const
{
	return m_frame;
}

uint32_t CompVideoFrame::getRawWithHeaderSize() const
{
	if (m_frame)
	{
		header * s = (header *)m_frame;
		return s->rawSize + sizeof(header);
	}
	return 0;
}

int CompVideoFrame::getTotalSize() const
{
	return m_totalSize - sizeof(header);
}

int CompVideoFrame::setBufferSize(uint32_t _rawSize)
{
	if (m_totalSize - sizeof(header) < _rawSize)
	{
		delete[] m_frame;
		m_totalSize = _rawSize + sizeof(header);
		m_frame = new unsigned char[m_totalSize];
		if (m_frame == nullptr)
		{
			m_totalSize = sizeof(header);
			return -1;
		}
	}
	header * s = (header *)m_frame;
	if (s)
		s->rawSize = _rawSize;
	m_header = s;
	return 0;
}

void CompVideoFrame::setTC(const Timecode& _tc)
{
	if (m_frame)
	{
		header * s = (header *)m_frame;
		s->tc = _tc;
	}
}

Timecode CompVideoFrame::getTC() const
{
	if (m_frame)
	{
		header * s = (header *)m_frame;
		return s->tc;
	}
	return Timecode::make_tc(0, 0, 0, 0);
}

void CompVideoFrame::setTimeStamp(double lTimeStamp)
{
	m_nTimeStamp = lTimeStamp;
}

double CompVideoFrame::getTimeStamp() const
{
	return m_nTimeStamp;
}

void CompVideoFrame::setIsIDR(bool bIsIDRFrame) const
{
	if (m_frame)
	{
		header * s = (header *)m_frame;
		s->isIDRFrame = bIsIDRFrame;
	}
}

bool CompVideoFrame::getIsIDR() const
{
	if (m_frame)
	{
		header * s = (header *)m_frame;
		return s->isIDRFrame;
	}
	return false;
}

//void CompVideoFrame::setAncFieldInfo(uint8_t *srcFieldDataEven, uint32_t dwEvenSize, uint8_t *srcOddFieldData, uint32_t dwOddSize) const
//{
//	if (m_frame)
//	{
//		header * s = (header *)m_frame;
//		s->ancData.dataSize[0] = dwEvenSize;
//		s->ancData.dataSize[1] = dwOddSize;
//		if (srcFieldDataEven != nullptr && dwEvenSize != 0)
//			memcpy_s(s->ancData.data[0], NTV2_ANCSIZE_MAX, srcFieldDataEven, dwEvenSize);
//		if (srcOddFieldData != nullptr && dwOddSize != 0)
//			memcpy_s(s->ancData.data[1], NTV2_ANCSIZE_MAX, srcOddFieldData, dwOddSize);
//	}
//}
//
//void CompVideoFrame::setAncFieldInfo(VideoANCDataStruct & ancInfo)
//{
//	setAncFieldInfo(ancInfo.data[0], ancInfo.dataSize[0], ancInfo.data[1], ancInfo.dataSize[1]);
//}
//
//void CompVideoFrame::getAncFieldInfo(uint8_t * srcFieldDataEven, uint32_t & dwEvenSize, uint8_t * srcOddFieldData, uint32_t & dwOddSize) const
//{
//	if (m_frame)
//	{
//		header * s = (header *)m_frame;
//		dwEvenSize = s->ancData.dataSize[0];
//		dwOddSize = s->ancData.dataSize[1];
//		if (srcFieldDataEven != nullptr && dwEvenSize != 0)
//			memcpy(srcFieldDataEven, s->ancData.data[0], dwEvenSize);
//		if (srcOddFieldData != nullptr && dwOddSize != 0)
//			memcpy(srcOddFieldData, s->ancData.data[1], dwOddSize);
//	}
//}
//
//void CompVideoFrame::getAncFieldInfo(VideoANCDataStruct & ancData) const
//{
//	if (m_frame)
//	{
//		header * s = (header *)m_frame;
//		ancData = s->ancData;
//	}
//}