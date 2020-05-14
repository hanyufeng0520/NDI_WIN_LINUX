#pragma once
#include <cstdint>

extern "C"
{
#include "../inc/ffmpeg/libavutil/opt.h"
#include "../inc/ffmpeg/libavutil/channel_layout.h"
#include "../inc/ffmpeg/libavutil/samplefmt.h"
#include "../inc/ffmpeg/libswresample/swresample.h"
}
class CAudioProcess
{
public:
	CAudioProcess();
	~CAudioProcess();
	int Initialize();
	int ProcessAudioToFLT(int nSampleCount, unsigned char* pSrcAudio, int audioCount, unsigned char* pDstAudio);
private:
	struct SwrContext *swr_ctx;
	int64_t src_ch_layout = 1;
	int64_t dst_ch_layout = 1;
	int src_rate = 48000;
	int dst_rate = 48000;
	int src_nb_channels = 16;
	int dst_nb_channels = 16;
	int src_linesize;
	int dst_linesize;
	enum AVSampleFormat src_sample_fmt = AV_SAMPLE_FMT_S32;
	enum AVSampleFormat dst_sample_fmt = AV_SAMPLE_FMT_FLT;
	uint8_t **dst_data = nullptr;
	bool	m_bInit;
	unsigned char* m_pSrcAudioNew;
};
