#include "stdafx.h"
#include "FrameConsumerNDI.h"
#include "../lib.Config/IConfig.h"
#include "../Lib.Base/ClockTimer.h"

CFrameConsumerNDI::CFrameConsumerNDI()
{
	m_pNDI_send = nullptr;
	m_stResample.Initialize();
	m_autoClock.start(Config->getFrameRateDen(), Config->getFrameRateNum());

	NDI_video_frame.FourCC = NDIlib_FourCC_type_UYVY;
	NDI_video_frame.picture_aspect_ratio = 16.0f / 9.0f;
	NDI_video_frame.timecode = NDIlib_send_timecode_synthesize;

	NDI_audio_frame.sample_rate = 48000;
	NDI_audio_frame.no_channels = 16;
	NDI_audio_frame.timecode = NDIlib_send_timecode_synthesize;
}

CFrameConsumerNDI::~CFrameConsumerNDI()
{
	m_autoClock.stop();
	stopChannel(0);
	if (m_pNDI_send)
		NDIlib_send_destroy(m_pNDI_send);
	if (NDI_audio_frame.p_data != nullptr)
		free(NDI_audio_frame.p_data);
	NDIlib_destroy();
}

int CFrameConsumerNDI::addChannel(uint32_t dwCnlID, const sFrameConsumer_Parameter& pCnlParameter)
{
	if (!NDIlib_initialize())
	{
		printf("CFrameConsumerNDI::addChannel NDIlib_initialize failed.\n");
		return -1;
	}
	FPTVideoFormat format = pCnlParameter.fpVideoFormat;

	NDI_video_frame.xres = Config->getVideoWidth();
	NDI_video_frame.yres = Config->getVideoHeight();
	NDI_video_frame.line_stride_in_bytes = Config->getVideoWidth() * 2;
	NDI_video_frame.frame_rate_N = Config->getFrameRateNum();
	NDI_video_frame.frame_rate_D = Config->getFrameRateDen();
	NDI_video_frame.frame_format_type = Config->isProgressive() ? NDIlib_frame_format_type_progressive : NDIlib_frame_format_type_interleaved,			// This is not a progressive frame
	NDI_audio_frame.no_samples = Config->getAudioSampleCount(); // Will be changed on the fly
	NDI_audio_frame.channel_stride_in_bytes = sizeof(float) * Config->getAudioSampleCount();
	NDI_audio_frame.p_data = (float*)malloc(NDI_audio_frame.channel_stride_in_bytes * NDI_audio_frame.no_channels);

	NDIlib_send_create_t NDI_send_create_desc;
	NDI_send_create_desc.clock_audio = false;
	NDI_send_create_desc.clock_video = true;
	if (pCnlParameter.name)
		NDI_send_create_desc.p_ndi_name = pCnlParameter.name;
	else
		NDI_send_create_desc.p_ndi_name = Config->getPGM1NDIName().c_str();
	m_pNDI_send = NDIlib_send_create(&NDI_send_create_desc);
	if (m_pNDI_send == nullptr)
	{
		printf("CFrameConsumerNDI::addChannel NDIlib_send_create failed.\n");
		return -1;
	}
	m_sendNDIThreadHandle = async_thread(thread_priority::normal, &CFrameConsumerNDI::sendNDIThread, this);
	printf("CFrameConsumerNDI::addChannel (%s) OK \n", pCnlParameter.name);
	return 0;
}

int CFrameConsumerNDI::OutputFrames(uint32_t nChannelID, pVFrame pVideo, pAframe pAudio, Timecode *tc, uint32_t &dwFramesDropped)
{
	if (m_listV.size() > FRAME_BUFFER_SIZE)
		return 1;
	m_listV.emplace_back(pVideo);
	m_listA.emplace_back(pAudio);
	m_SemaphoreClock.raiseEvent();
	if (m_outNum++ % 3000 == 0)
		printf("CFrameConsumerNDI::OutputFrames (%I64d) \n", m_outNum);
	return 0;
}

int CFrameConsumerNDI::stopChannel(uint32_t nChannelID)
{
	m_bExit = true;
	if (m_sendNDIThreadHandle.valid())
		m_sendNDIThreadHandle.get();
	return 0;
}

void CFrameConsumerNDI::sendNDIThread()
{
	while (!m_bExit)
	{
		if (!m_SemaphoreClock.waitEvent())
			continue;
		sentToNDI();
	}
}

void CFrameConsumerNDI::sentToNDI()
{
	static UINT64 nLoopTimes = 0;
	static ClockTimer timer;
	static float fInterval = Config->getInterval();
	static int nMaxSamples = Config->getAudioSampleCount();

	ClockTimer timerA, timerV;
	timer.reset();

	pAframe pAudio = nullptr;
	m_listA.pop_begin(pAudio);

	if (pAudio->getSampleCount() > nMaxSamples)
	{
		NDI_audio_frame.no_samples = nMaxSamples;
		memset(NDI_audio_frame.p_data, 0, nMaxSamples * NDI_audio_frame.no_channels * sizeof(float));
	}
	else
	{
		NDI_audio_frame.no_channels = pAudio->GetMonoCnt();
		NDI_audio_frame.no_samples = pAudio->getSampleCount();
		NDI_audio_frame.channel_stride_in_bytes = sizeof(float) * NDI_audio_frame.no_samples;

		int nRes = m_stResample.ProcessAudioToFLT(pAudio->getSampleCount(),
			(unsigned char*)pAudio->getRaw(),
			NDI_audio_frame.no_channels,
			(unsigned char*)NDI_audio_frame.p_data);
	}

	NDIlib_send_send_audio_v2(m_pNDI_send, &NDI_audio_frame);

	float timeCostA = timerA.elapse_ms();

	pVFrame pVideo = nullptr;
	m_listV.pop_begin(pVideo);
	NDI_video_frame.p_data = pVideo->getBuffer();

	timerV.reset();
	NDIlib_send_send_video_v2(m_pNDI_send, &NDI_video_frame);
	float timeCostV = timerV.elapse_ms();
	float timeCost = timer.elapse_ms();
	if (timeCost > (fInterval + 3))
		printf("sentToNDI nLoop(%I64d) timeout %.2f ms A(%.2f ms) V(%.2f ms) \n",nLoopTimes, timeCost, timeCostA, timeCostV);

	++nLoopTimes;

	if (m_sendNum++ % 3000 == 0)
		printf("CFrameConsumerNDI::sentToNDI (%I64d) \n", m_sendNum);
}
