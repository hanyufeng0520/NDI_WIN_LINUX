#pragma once
#include <memory>

class VEncodedFrame
{
	static const unsigned long m_width = 3840;
	static const unsigned long m_height = 2160;

	unsigned char * m_pFrame;
	unsigned long m_dataSize = 0;
	unsigned long m_totalSize = 2*1024 * 1024;
	__int64		  m_nFrameID;
	double m_nTimeStamp;
	BOOL m_bIsIDRFrame;
	void reAlloc(UINT32 size)
	{
		if (m_pFrame)
			delete[]m_pFrame;

		m_pFrame = new unsigned char[size];
		m_totalSize = size;
	}
public:
	VEncodedFrame()
	{
		m_bIsIDRFrame = FALSE;
		m_nFrameID = 0;
		m_nTimeStamp = 0;
		m_pFrame = new unsigned char[m_totalSize];
	};
	~VEncodedFrame()
	{
		delete []m_pFrame;
	};

	const unsigned long getWidth() 
	{
		return m_width;
	};

	const unsigned long getHeight()
	{
		return m_height;
	};

	unsigned char* getRaw()
	{
		return m_pFrame;
	};

	void setDataSize(unsigned long dataSize, __int64 nFrameID,BOOL bIsIDRFrame)
	{
		m_dataSize = dataSize;
		m_nFrameID = nFrameID;
		m_bIsIDRFrame = bIsIDRFrame;
		if (m_dataSize > m_totalSize)
		{
			reAlloc(dataSize);
		}		
	};
	unsigned long getTotalSize()
	{
		return m_totalSize;
	};

	unsigned long getDataSize()
	{
		return m_dataSize;
	};

	BOOL getIsIDRFrame()
	{
		return m_bIsIDRFrame;
	}
	void setTimeStamp(double lTimeStamp)
	{
		m_nTimeStamp = lTimeStamp;
	}
	double getTimeStamp()
	{
		return m_nTimeStamp;
	}
	__int64 GetFrameID(){ return m_nFrameID; }
};

typedef std::shared_ptr<VEncodedFrame> pVComFrame;
