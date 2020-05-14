#include "AudioProcess.h"
#include "../lib.Config/IConfig.h"

CAudioProcess::CAudioProcess()
{
	m_bInit = false;
	dst_data = nullptr;
	swr_ctx = swr_alloc();
	m_pSrcAudioNew = new unsigned char[Config->getAudioSampleCount() * 16 * 4];
}

CAudioProcess::~CAudioProcess()
{
	if (dst_data)
		av_freep(&dst_data[0]);
	av_freep(&dst_data);
	swr_free(&swr_ctx);
	delete[] m_pSrcAudioNew;
}

int CAudioProcess::Initialize()
{
	//src_nb_samples = nSampleCount;
	av_opt_set_int(swr_ctx, "in_channel_layout", src_ch_layout, 0);
	av_opt_set_int(swr_ctx, "in_sample_rate", src_rate, 0);
	av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", src_sample_fmt, 0);

	av_opt_set_int(swr_ctx, "out_channel_layout", dst_ch_layout, 0);
	av_opt_set_int(swr_ctx, "out_sample_rate", dst_rate, 0);
	av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", dst_sample_fmt, 0);
	int ret;
	if ((ret = swr_init(swr_ctx)) < 0)
		return ret;
	dst_nb_channels = src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);
	m_bInit = true;
	return 0;
}

int CAudioProcess::ProcessAudioToFLT(int nSampleCount, unsigned char* pSrcAudio, int audioCount, unsigned char* pDstAudio)
{
	int ret = -1;
	if (!m_bInit)
		return ret;
	int nSampleCountNew = audioCount * nSampleCount;
	long *pAudioNew = (long *)m_pSrcAudioNew;
	for (int nIndex = 0; nIndex < audioCount; ++nIndex)
	{
		long *pSrc = (long *)pSrcAudio;
		for (int i = 0; i < nSampleCount; i++)
		{
			*pAudioNew++ = pSrc[nIndex];
			pSrc += 16;
		}
	}
	int dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, src_rate) + nSampleCountNew, dst_rate, src_rate, AV_ROUND_UP);
	// 	if (audioCount == 16)
	// 		ret = swr_convert(swr_ctx, (uint8_t **)&pDstAudio, dst_nb_samples, (const uint8_t **)&pSrcAudio, nSampleCount);
	// 	else
	ret = swr_convert(swr_ctx, (uint8_t * *)&pDstAudio, dst_nb_samples, (const uint8_t * *)&m_pSrcAudioNew, nSampleCountNew);
	if (ret < 0)
		return ret;
	int nDestSize = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels, ret, dst_sample_fmt, 1);
	return 0;
}