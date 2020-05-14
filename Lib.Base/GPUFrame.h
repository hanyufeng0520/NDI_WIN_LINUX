#pragma once
#include <cstdint>
#include "videoFrame.h"
#include <memory>

class GpuFrame :public VideoFrame
{
public:
	int nCamID = 0;
	int64_t  m_yuv420BufferAddress = 0;
	int64_t m_pitch;
	int32_t m_yuv420Index = 0;
	void reset()
	{
		m_yuv420BufferAddress = 0;
		m_pitch = 0;
		m_yuv420Index = 0;
	}
};

using pGPUFrame = std::shared_ptr<GpuFrame>;