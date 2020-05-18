#pragma once
#include "VideoFormatDef.h"
namespace FFMPEG_AUDIOSAMPLE
{
	struct AVRational {
		int den; ///< denominator Duration
		int num; ///< numerator   timeScale
	};
	typedef struct {
		char describe[20];
		AVRational time_base;
		int samples_per_frame[6];
	} MXFSamplesPerFrame;

	typedef struct {
		int width;
		int height;
	}VIDEOWIDTH_HEIGHT;

	typedef struct {
		FPTVideoFormat		videoFormat;
		char				describe[24];
		VIDEOWIDTH_HEIGHT	width_height;
		AVRational time_base;
	} FPT_VIDEOFORMAT_INFO;

	static const FPT_VIDEOFORMAT_INFO mxf_spfx[] = {
		{FPTVideoFormat::FP_FORMAT_UNKNOWN   ,"UNKNOWN   ",	{    0,    0 },	{    0,     0 } },
		{FPTVideoFormat::FP_FORMAT_1080i_5000,"1080i50   ",	{ 1920, 1080 },	{ 1000, 25000 } },
		{FPTVideoFormat::FP_FORMAT_1080i_5994,"1080i59.94",	{ 1920, 1080 },	{ 1001, 30000 } },
		{FPTVideoFormat::FP_FORMAT_1080i_6000,"1080i60   ",	{ 1920, 1080 },	{ 1000, 30000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_2398,"1080p23.98",	{ 1920, 1080 },	{ 1001, 24000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_2400,"1080p24   ",	{ 1920, 1080 },	{ 1000, 24000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_2500,"1080p25   ",	{ 1920, 1080 },	{ 1000, 25000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_2997,"1080p29.97",	{ 1920, 1080 },	{ 1001, 30000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_3000,"1080p30   ",	{ 1920, 1080 },	{ 1000, 30000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_5000,"1080p50   ",	{ 1920, 1080 },	{ 1000, 50000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_5994,"1080p59.94",	{ 1920, 1080 },	{ 1001, 60000 } },
		{FPTVideoFormat::FP_FORMAT_1080p_6000,"1080p60   ",	{ 1920, 1080 },	{ 1000, 60000 } },
		{FPTVideoFormat::FP_FORMAT_720p_2500 ,"720p25    ",	{ 1280,  720 },	{ 1000, 25000 } },
		{FPTVideoFormat::FP_FORMAT_720p_2997 ,"720p29.97 ",	{ 1280,  720 },	{ 1001, 30000 } },
		{FPTVideoFormat::FP_FORMAT_720p_5000 ,"720p50    ",	{ 1280,  720 },	{ 1000, 50000 } },
		{FPTVideoFormat::FP_FORMAT_720p_5994 ,"720p59.94 ",	{ 1280,  720 },	{ 1001, 60000 } },
		{FPTVideoFormat::FP_FORMAT_720p_6000 ,"720p60    ",	{ 1280,  720 },	{ 1000, 60000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_2400  ,"4Kp24     ",	{ 3840, 2160 },	{ 1000, 24000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_2500  ,"4Kp25     ",	{ 3840, 2160 },	{ 1000, 25000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_2997  ,"4Kp29.97  ",	{ 3840, 2160 },	{ 1001, 30000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_3000  ,"4Kp30     ",	{ 3840, 2160 },	{ 1000, 30000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_5000  ,"4Kp50     ",	{ 3840, 2160 },	{ 1000, 50000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_5994  ,"4Kp59.94  ",	{ 3840, 2160 },	{ 1001, 60000 } },
		{FPTVideoFormat::FP_FORMAT_4Kp_6000  ,"4Kp60     ",	{ 3840, 2160 },	{ 1000, 60000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_2400  ,"8kp24     ",	{ 7680, 4320 },	{ 1000, 24000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_2500  ,"8kp25     ",	{ 7680, 4320 },	{ 1000, 25000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_2997  ,"8kp29.97  ",	{ 7680, 4320 },	{ 1001, 30000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_3000  ,"8kp30     ",	{ 7680, 4320 },	{ 1000, 30000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_5000  ,"8kp50     ",	{ 7680, 4320 },	{ 1000, 50000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_5994  ,"8kp59.94  ",	{ 7680, 4320 },	{ 1001, 60000 } },
		{FPTVideoFormat::FP_FORMAT_8kp_6000  ,"8kp60     ",	{ 7680, 4320 },	{ 1000, 60000 } },
	};

	static const MXFSamplesPerFrame mxf_spf[] = {
		{ "FILM 23.976",{ 1001, 24000 },{ 2002, 0,    0,    0,    0,    0 } },		// FILM 23.976
		{ "FILM 24    ",{ 1000, 24000 },{ 2000, 0,    0,    0,    0,    0 } },			// FILM 24
		{ "NTSC 29.97 ",{ 1001, 30000 },{ 1602, 1601, 1602, 1601, 1602, 0 } },		// NTSC 29.97
		{ "NTSC 59.94 ",{ 1001, 60000 },{ 801,  801,  801,  801,  800,  0 } },		// NTSC 59.94
		{ "PAL  25    ",{ 1000, 25000 },{ 1920, 0,    0,    0,    0,    0 } },			// PAL 25
		{ "PAL  50    ",{ 1000, 50000 },{ 960,  0,    0,    0,    0,    0 } },			// PAL 50
		{ "PAL  30    ",{ 1000, 30000 },{ 1600,  0,    0,    0,    0,    0 } },			// PAL 30
		{ "PAL  60    ",{ 1000, 60000 },{ 800,  0,    0,    0,    0,    0 } },			// PAL 60
	};

	inline const char*getVideoFormatString(FPTVideoFormat _fpvf)
	{
		if (_fpvf > FPTVideoFormat::FP_FORMAT_MAX)return "Unknown";
		for (int i = 0; i < static_cast<int>(FPTVideoFormat::FP_FORMAT_MAX); i++)
		{
			if (_fpvf == mxf_spfx[i].videoFormat)
				return mxf_spfx[i].describe;
		}
		return mxf_spfx[0].describe;
	}
	inline int getFrameRateNum(FPTVideoFormat _fpvf)
	{
		for (int i = 0; i < static_cast<int>(FPTVideoFormat::FP_FORMAT_MAX); i++)
		{
			if (_fpvf == mxf_spfx[i].videoFormat)
				return mxf_spfx[i].time_base.num;
		}
		return mxf_spfx[0].time_base.num;
	}
	inline int getFrameRateDen(FPTVideoFormat _fpvf)
	{
		for (int i = 0; i < static_cast<int>(FPTVideoFormat::FP_FORMAT_MAX); i++)
		{
			if (_fpvf == mxf_spfx[i].videoFormat)
				return mxf_spfx[i].time_base.den;
		}
		return mxf_spfx[0].time_base.den;
	}

	inline int getFrameWidth(FPTVideoFormat _fpvf)
	{
		for (int i = 0; i < static_cast<int>(FPTVideoFormat::FP_FORMAT_MAX); i++)
		{
			if (_fpvf == mxf_spfx[i].videoFormat)
				return mxf_spfx[i].width_height.width;
		}
		return mxf_spfx[0].width_height.width;
	}

	inline int getFrameHeight(FPTVideoFormat _fpvf)
	{
		for (int i = 0; i < static_cast<int>(FPTVideoFormat::FP_FORMAT_MAX); i++)
		{
			if (_fpvf == mxf_spfx[i].videoFormat)
				return mxf_spfx[i].width_height.height;
		}
		return mxf_spfx[0].width_height.height;
	}
	inline const int * getFrameSamples(int _den, int _num, int &arrLen)
	{
		for (int i = 0; i < sizeof(mxf_spf) / sizeof(MXFSamplesPerFrame); i++)
		{
			if (mxf_spf[i].time_base.den == _den && mxf_spf[i].time_base.num == _num)
			{
				const int* arr = mxf_spf[i].samples_per_frame;
				for (arrLen = 0; arrLen < 6; arrLen++)
				{
					if (arr[arrLen] == 0)
					{
						break;
					}
				}
				return arr;
			}
		}
		return mxf_spf[0].samples_per_frame;
	}
	inline const int *getFrameSamples(FPTVideoFormat _fpvf, int &arrLen)
	{
		return getFrameSamples(getFrameRateDen(_fpvf), getFrameRateNum(_fpvf), arrLen);
	}
};