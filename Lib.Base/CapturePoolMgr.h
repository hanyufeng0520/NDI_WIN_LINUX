#pragma once
#include "audioFrame.h"
#include "videoFrame.h"
#include "compVideoFrame.h"
#include "ObjectPoolEx.h"

class CapturePoolMgr
{
	ObjectPoolEx<VideoFrame>                m_captureVideoPool;
	ObjectPoolEx<AudioFrame>                m_captureAudioPool;
	ObjectPoolEx<CompVideoFrame>            m_captureVideoComp;

	CapturePoolMgr() = default;
	~CapturePoolMgr() = default;
public:

	static CapturePoolMgr * GetInstance();
	void					initialize(FPTVideoFormat _fpFormat, int _vCount, int _aCount, bool _bLocalMJPEG);
	void					getNew(VideoFrame* & _elem);
	void					getNew(CompVideoFrame* & _elem);
	void					getNew(AudioFrame* & _elem);
	void					release(AudioFrame* _elem);
	void					release(VideoFrame* _elem);
	void					release(CompVideoFrame* _elem);
	uint32_t				getVideoPoolSize(bool _bLocalMJPEG);
	uint32_t				getAudioPoolSize();

	//Managed API
	template <typename T>
	void					getNew(std::shared_ptr<T> & _elem)
	{
		T* frame = nullptr;
		getNew(frame);
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
