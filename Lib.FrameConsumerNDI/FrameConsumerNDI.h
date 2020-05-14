#pragma once
#include "IFrameConsumerChannel.h"
#include "../Lib.Base/AutoLoopClock.h"
#include "../Lib.Base/SemaphoreClock.h"
#include "../Lib.Base/FrameList.h"
#include "../inc/NDI/Processing.NDI.Lib.h"
#include <future>
#include "../Lib.Base/async_thread.h"
#include "AudioProcess.h"
#include "../Lib.Base/SyncList.h"
#define FRAME_BUFFER_SIZE   3

class CFrameConsumerNDI :public IFrameConsumerChannel
{
	AutoLoopClock                           m_autoClock;
	SemaphoreClock                          m_SemaphoreClock;
	SemaphoreClock                          m_SemaphoreClockA;
	const static uint32_t                   m_dwMax = 3;
	uint64_t	                            m_outNum = 0;
	uint64_t	                            m_sendNum = 0;
	SyncList<pVFrame>                       m_listV;
	SyncList<pAframe>                       m_listA;
	NDIlib_video_frame_v2_t					NDI_video_frame;
	NDIlib_audio_frame_v2_t					NDI_audio_frame;
	async_future					        m_sendNDIThreadHandle;
	bool							        m_bExit = false;
	NDIlib_send_instance_t			        m_pNDI_send = nullptr;
	CAudioProcess						    m_stResample;

	void sendNDIThread();
	void sentToNDI();
public:
	CFrameConsumerNDI();
	virtual ~CFrameConsumerNDI();

	int			addChannel(uint32_t dwCnlID, const sFrameConsumer_Parameter& pCnlParameter) override;
	int			stopChannel(uint32_t nChannelID) override;
	int			OutputFrames(uint32_t nChannelID, pVFrame pVideo, pAframe pAudio, Timecode *tc, uint32_t &dwFramesDropped) override;
};
