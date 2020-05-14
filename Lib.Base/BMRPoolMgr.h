#pragma once
#include "gpuframe.h"
#include "ObjectPoolEx.h"
#include "Lib.channelCmd/cnlInfo.h"
#include "Lib.Config/IConfig.h"
#include "Codec/NvidiaVideoSDK/Lib.NvEncoder/NvEncoderDefine.h"
#include "Lib.Base/locker.h"

class BMRPoolMgr
{
	ObjectPoolEx<GpuFrame>                m_VideoPoolHD[NB_LOCAL_MAXCAM];

	BMRPoolMgr() = default;
	~BMRPoolMgr() = default;
public:

	static BMRPoolMgr* GetInstance() {
		static BMRPoolMgr* gSInstance = nullptr;
		static Locker g_lock;
		if (gSInstance != nullptr)return gSInstance;
		LockHolder lock(g_lock);
		if (gSInstance == nullptr)
			gSInstance = new BMRPoolMgr();
		return gSInstance;
	}

	void initialize(ChannelMask mask, Encoder264_Hardware encodeType, int _width, int _height, int _count) {
		for (int nCam = 0; nCam < std::size(m_VideoPoolHD); nCam++)
		{
			if (!mask[nCam])continue;
			m_VideoPoolHD[nCam].initialize<GpuFrame>(_count);
			for (int j = 0; j < _count; j++)
			{
				GpuFrame* frame = nullptr;
				m_VideoPoolHD[nCam].getNew(frame);
				frame->reset();
				frame->nCamID = nCam;
				frame->m_yuv420Index = nCam * NB_ENCODER_BUFFER_CNT + j;
				if (encodeType == Encoder264_Hardware::hardware_CPU)
				{
					switch (_height)
					{
					case 720: frame->init(VideoFrameSize::FS_720, VideoColorSpace::CC_420_NV12); break;
					case 1080: frame->init(VideoFrameSize::FS_1080p, VideoColorSpace::CC_420_NV12); break;
					case 2160: frame->init(VideoFrameSize::FS_4K, VideoColorSpace::CC_420_NV12); break;
					}
				}
				m_VideoPoolHD[nCam].release(frame);
			}
		}
	}

	void getNew(int nCam, GpuFrame*& _elem) {
		m_VideoPoolHD[nCam].getNew(_elem);
	}

	void release(GpuFrame* _elem) {
		if (_elem != nullptr)
			m_VideoPoolHD[_elem->nCamID].release(_elem);
	}

	uint32_t getVideoPoolSize(int nCam) {
		return m_VideoPoolHD[nCam].size();
	}

	//Managed API
	template <typename T>
	void getNew(int nCam, std::shared_ptr<T>& _elem)
	{
		T* frame = nullptr;
		getNew(nCam, frame);
		if (frame == nullptr)
		{
			_elem = nullptr;
			return;
		}

		_elem = std::shared_ptr<T>(frame, [](T*& ptr)
			{
				GetInstance()->release(ptr);
			});
	}
};
