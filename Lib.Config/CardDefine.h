#pragma once
#include <Windows.h>
#include <iterator>
#include "cnlInfo.h"
enum EM_ConsumerChannel
{
	EM_ConsumerChannel_PGM1_Dirty = 0,
	EM_ConsumerChannel_PGM1_Clean = 8,
	EM_ConsumerChannel_PGM2_Dirty = 16,
	EM_ConsumerChannel_PGM2_Clean = 24,
	EM_ConsumerChannel_PGM3_Dirty = 32,
	EM_ConsumerChannel_PGM3_Clean = 40,
	EM_ConsumerChannel_PGM4_Dirty = 48,
	EM_ConsumerChannel_PGM4_Clean = 56,
	EM_ConsumerChannel_PGM5_Dirty = 64,
	EM_ConsumerChannel_PGM5_Clean = 72,
	EM_ConsumerChannel_PGM6_Dirty = 80,
	EM_ConsumerChannel_PGM6_Clean = 88,
	EM_ConsumerChannel_PGM7_Dirty = 96,
	EM_ConsumerChannel_PGM7_Clean = 104,
	EM_ConsumerChannel_PGM8_Dirty = 112,
	EM_ConsumerChannel_PGM8_Clean = 120,
	EM_ConsumerChannel_VisionControl = 128,
	EM_ConsumerChannel_AUX1 = 136,
	EM_ConsumerChannel_AUX2 = 144,
	EM_ConsumerChannel_PGM1_NDI = 153,
	EM_ConsumerChannel_STREAM_0,
	EM_ConsumerChannel_STREAM_1,
	EM_ConsumerChannel_STREAM_2,
	EM_ConsumerChannel_STREAM_3,
	EM_ConsumerChannel_STREAM_4,
	EM_ConsumerChannel_STREAM_5,
	EM_ConsumerChannel_STREAM_6,
	EM_ConsumerChannel_STREAM_7,
	EM_ConsumerChannel_STREAM_8,
	EM_ConsumerChannel_STREAM_9,
	EM_ConsumerChannel_STREAM_10,
	EM_ConsumerChannel_STREAM_11,
	EM_ConsumerChannel_STREAM_12,
	EM_ConsumerChannel_STREAM_13,
	EM_ConsumerChannel_STREAM_14,
	EM_ConsumerChannel_STREAM_15,
	EM_ConsumerChannel_STREAM_16,
	EM_ConsumerChannel_STREAM_17,
	EM_ConsumerChannel_MAX
};

enum EM_ProviderChannel
{
	EM_ProviderChannel_0,
	EM_ProviderChannel_1,
	EM_ProviderChannel_2,
	EM_ProviderChannel_3,
	EM_ProviderChannel_4,
	EM_ProviderChannel_5,
	EM_ProviderChannel_6,
	EM_ProviderChannel_7,
	EM_ProviderChannel_8,
	EM_ProviderChannel_9,
	EM_ProviderChannel_10,
	EM_ProviderChannel_11,
	EM_ProviderChannel_12,
	EM_ProviderChannel_13,
	EM_ProviderChannel_14,
	EM_ProviderChannel_15,
	EM_ProviderChannel_Max/*=128*/ //DAILI
};

enum class EM_Channel_Type
{
	EM_Channel_Type_Unknown,
	EM_Channel_Type_Provider,
	EM_Channel_Type_Consumer
};

enum class FrameProviderType
{
	FPT_UNKNOWN = 0,
	FPT_YUV_FILE,
	FPT_MJPEG_FILE,
	FPT_AJA,
	FPT_BLACKMAGIC,
	FPT_DELTACAST,
	FPT_RMTP_STREAM,
	FPT_TSUDP_STREAM,
	FPT_MAGEWELL,
	FPT_NDI,
	FPT_ST2022,
	FPT_virtual_Import,
	FPT_virtual_PGMDirty,
	FPT_virtual_PGMClean,
	FPT_MAX_TYPE,
};
struct EM_Channel_Detail
{
	CamID               camID;
	uint32_t			nIndexID = 0;
	uint32_t			nSDIID = 0xFFFFFFFF;
	uint32_t			nGPUIndex = 0;
	FrameProviderType	providerType = FrameProviderType::FPT_UNKNOWN;
	char				szItemName[MAX_PATH] = {}; //Can be YUV file name,NDI name ,UDP address ,rtmp..
	char				szAudioName[MAX_PATH] = {};//Only for local YUV
	EM_Channel_Type		chlType = EM_Channel_Type::EM_Channel_Type_Unknown;
	//ServerSN			sn;
	bool				isNetworking = false;
};

struct Card_Config
{
	EM_Channel_Detail recorder[EM_ProviderChannel_Max];
	EM_Channel_Detail player[EM_ConsumerChannel_MAX];
};
