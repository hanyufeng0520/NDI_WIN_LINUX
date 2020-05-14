#include "CapturePoolMgr.h"
#include "locker.h"
#include "videoFrame.h"

CapturePoolMgr * CapturePoolMgr::GetInstance()
{
	static CapturePoolMgr* gSInstance = nullptr;
	static Locker g_lock;
	if (gSInstance != nullptr)
		return gSInstance;

	LockHolder lock(g_lock);
	if (gSInstance == nullptr)
		gSInstance = new CapturePoolMgr();

	return gSInstance;
}

void CapturePoolMgr::initialize(FPTVideoFormat _fpFormat, int _vCount, int _aCount, bool _bLocalMJPEG)
{
	if (!_bLocalMJPEG)
	{
		m_captureVideoPool.initialize<VideoFrame>(_vCount);
		for (int i = 0; i < _vCount; i++)
		{
			VideoFrame* frame = nullptr;
			m_captureVideoPool.getNew(frame);
			frame->init(_fpFormat, VideoColorSpace::CC_422_UYVY);
			m_captureVideoPool.release(frame);
		}
	}
	else
		m_captureVideoComp.initialize<CompVideoFrame>(_vCount);

	m_captureAudioPool.initialize<AudioFrame>(_aCount);
}

void CapturePoolMgr::getNew(VideoFrame*& _elem)
{
	m_captureVideoPool.getNew(_elem);
	if (_elem)
	{
		_elem->setTCValid(false);
		_elem->resetBlackFlag();
	}
}

void CapturePoolMgr::getNew(CompVideoFrame*& _elem)
{
	m_captureVideoComp.getNew(_elem);
}

void CapturePoolMgr::getNew(AudioFrame*& _elem)
{
	m_captureAudioPool.getNew(_elem);
}

void CapturePoolMgr::release(AudioFrame* _elem)
{
	if (_elem != nullptr)
		m_captureAudioPool.release(_elem);
}

void CapturePoolMgr::release(VideoFrame* _elem)
{
	if (_elem)
		m_captureVideoPool.release(_elem);
}

void CapturePoolMgr::release(CompVideoFrame* _elem)
{
	if (_elem)
		m_captureVideoComp.release(_elem);
}

uint32_t CapturePoolMgr::getVideoPoolSize(bool _bLocalMJPEG)
{
	return !_bLocalMJPEG ? m_captureVideoPool.size() : m_captureVideoComp.size();
}

uint32_t CapturePoolMgr::getAudioPoolSize()
{
	return m_captureAudioPool.size();
}