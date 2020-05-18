#pragma once
#include <string>
#include "CardDefine.h"
#include <bitset>
#include "../Lib.Base/VideoFormatDef.h"
#include <vector>

#define NB_MAX_USER	(1)
#define Config (IConfig::getInstance())

enum class LayerType
{
	LayerType_RTD,
	LayerType_Gfx1,
	LayerType_Gfx2,
	LayerType_Gfx3,
	LayerType_Max
};

enum class CardType
{
	CardType_AJA,
	CardType_LocalUYVYFile,
	CardType_LocalMJPEGFile,
	CardType_BlackMagic,
	CardType_DeltaCast,
	CardType_RTMPStream,
	CardType_TSUDPStream,
	CardType_MAGEWELL,
	CardType_NDI,
	CardType_Max,
};

enum class LiveTGAType
{
	LiveTGAType_disable,
	LiveTGAType_OnTCP,	//27102, CasperCG send ARGB without header but compressed by snappy
	LiveTGAType_NDI		//NDI send ARGB without header
};

enum class WorkingMode
{
	WorkingMode_Vibox,
	WorkingMode_BMR,
	WorkingMode_Max
};

enum class ServerType
{
	Server_8cnl,
	Server_6cnl,
	Server_DemoNUC,
	Server_16cnl,
	Server_4K,
	Server_0cnl,
};

struct tagServerTypeTable
{
	ServerType serverType;
	const char* TypeString;
};

static tagServerTypeTable g_tagServerTypeTable[] = {
	{ServerType::Server_0cnl, "0 channel"  },
	{ServerType::Server_8cnl, "8 channels" },
	{ServerType::Server_6cnl, "6 channels" },
	{ServerType::Server_DemoNUC, "Demo NUC"   },
	{ServerType::Server_16cnl, "16 channels"},
	{ServerType::Server_4K, "4K"		   }
};

inline const char* findServerTypeString(ServerType st)
{
	for (const auto& item : g_tagServerTypeTable)
	{
		if (item.serverType == st) return item.TypeString;
	}
	return "unknown";
}

enum class SlowMotionMode
{
	SlowMotionMode_parity_violation,
	SlowMotionMode_line_double,
};

enum class MemoryBaseOnServerType
{
	Memory_8GB = 8,
	Memory_32GB = 32,
	Memory_64GB = 64,
	Memory_128GB = 128,
};

enum class RGBColorModel
{
	BT709,
	BT601
};

enum class LTCType
{
	FromCardExternalTC,
	FromMCU,
	FromCardEmbeddedTC
};

enum class ThumbnailResolutionType
{
	thumbnail_480x270,
	thumbnail_960x540,
	thumbnail_full_size,
};

enum class Encoder264_Hardware
{
	hardware_GPU = 0,
	hardware_CPU = 1,
};

enum class EncoderOut_Conversion
{
	conversion_NO = 0,
	conversion_YES = 1,
};

struct ThumbnailResolution
{
	ThumbnailResolutionType type = ThumbnailResolutionType::thumbnail_480x270;
	uint32_t width = 480;
	uint32_t height = 270;

	void set_value(ThumbnailResolutionType _type, uint32_t _width, uint32_t _height)
	{
		type = _type;
		switch (_type)
		{
		case ThumbnailResolutionType::thumbnail_480x270:	  width = 480;  height = 270;  break;
		case ThumbnailResolutionType::thumbnail_960x540:   width = 960;  height = 540;  break;
		case ThumbnailResolutionType::thumbnail_full_size: width = _width; height = _height;  break;
		default: break;
		}
	}
};

using ChannelMask = std::bitset<EM_ConsumerChannel_MAX>;
class IConfig
{
protected:
	IConfig() = default;
	virtual ~IConfig() = default;
public:
	static IConfig* getInstance();
	static void		releaseInstance();
	virtual CamID   							getFirstLocalSDICam() const = 0;
	virtual const char*                         getGfxNDIChannelName(int _idx) const = 0;
	virtual std::string							getAudioMixerName() const = 0;
	virtual LayerType							getLayers(int _l) const = 0;
	virtual const bool&							is4K() const = 0;
	virtual const bool&							isDropNTSC() const = 0;
	virtual const bool&							is720p() const = 0;
	virtual const bool&							isHD() const = 0;
	virtual const bool&							isNTSC() const = 0;
	virtual const bool&							isFreeRun() const = 0;
	virtual const bool&							isRTDonClean() const = 0;
	virtual const bool&							isProgressive() const = 0;
	virtual const LiveTGAType&					getLiveTGAType() const = 0;
	virtual const CardType&						getCardType() const = 0;
	virtual const std::string&					getCardTypeString() const = 0;
	virtual const bool&							getNeedRecPGMDirty() const = 0;
	virtual const bool&							getNeedRecPGMClean() const = 0;
	virtual const ChannelMask&					getRecChannelMask() const = 0;
	virtual const ChannelMask&					getNetworkChannelMask() const = 0;
	virtual int									getNbRec() const = 0;
	virtual int									getNbRec_ALL() const = 0;
	virtual int									getNbRec_OnlyNetCams() const = 0;
	virtual int									getNbRec_OnlyLocalCams() const = 0;
	virtual const int&                          getSDINbRec() const = 0;
	virtual const int&                          getStreamNbRec() const = 0;
	virtual int									getNbPGM() const = 0;
	virtual const ChannelMask&                  getPGMChannelMask() const = 0;
	virtual const Card_Config&					getCardConfig() const = 0;
	virtual const float&                        getInterval() const = 0;
	virtual const bool&							isUsingCard() const = 0;
	virtual const FPTVideoFormat&               getVideoFormat() const = 0;
	virtual const std::string&                  getVideoFormatString() const = 0;
	virtual const int&                          getFramesPerSec() const = 0;
	virtual const int&                          getSendingThumbnailJump() const = 0;
	virtual const int&                          getFrameRateNum() const = 0;
	virtual const int&                          getFrameRateDen() const = 0;
	virtual const int&                          getVideoWidth() const = 0;
	virtual const int&                          getVideoHeight() const = 0;
	virtual const bool&							isHighQualityThumbnail() const = 0;
	virtual const uint32_t&                     getAudioSampleCount() const = 0;
	virtual const bool&							isDMAPlayout() const = 0;
	virtual bool								isBMR() const = 0;
	virtual bool								isVibox() const = 0;
	virtual const bool&							useExternalTimeCode() const = 0;
	virtual const WorkingMode&                  getWorkingMode() const = 0;
	virtual const std::string&                  getWorkingModeString() const = 0;
	virtual const bool&							alwaysDisplayUI() const = 0;
	virtual const uint32_t&                     getAjaOutBufferthreshold() const = 0;
	virtual const uint32_t&                     getMaxAudioLatency() const = 0;
	virtual const uint32_t&                     getAudioLatencySecurity() const = 0;
	virtual const int&                          getNbCoreReserved() const = 0;
	virtual const bool&							supportUHD_2SI() const = 0;
	virtual const bool&							isBuildMarker() const = 0;
	virtual const bool&							needReduceThumbnailSendingfps() const = 0;
	virtual const uint32_t&                     getRtmpOutChunkSize() const = 0;
	virtual const bool&                         getStreamInNeedSync() const = 0;
	virtual const ServerType&                   getServerType() const = 0;
	virtual const char*                         getServerTypeString() const = 0;
	virtual const MemoryBaseOnServerType&       getMaxMemoryGBBaseOnServerType() const = 0;
	virtual const int&                          getMaxUser() const = 0;
	virtual const int&                          getAJAInBufferLevel() const = 0;
	virtual const SlowMotionMode&               getSlowMotionMode() const = 0;
	virtual const bool&							isInLevelA() const = 0;
	virtual const bool&							isOutLevelA() const = 0;
	virtual const int&                          getSubstitutePlayer() const = 0;
	virtual const bool&							isPGMSubstitute() const = 0;
	virtual const uint32_t&                     getImportWaitTime() const = 0;
	virtual const uint32_t&                     getUdpBufSize() const = 0;
	virtual const uint32_t&                     getDefaultReplayJpegQuality() const = 0;
	virtual const uint32_t&                     getDefaultPGMPRVJpegQuality() const = 0;
	virtual const std::string&                  getPGM1NDIName() const = 0;
	virtual const bool&							isEnableStorageCRC() const = 0;
	virtual const int&                          getEncodeXDCamCpu() const = 0;
	virtual	const RGBColorModel&                getRGBColorModel() const = 0;
	virtual const bool&                         getDisableLiveToMixer() const = 0;
	virtual const bool&                         getDelayPGMVideo() const = 0;
	virtual const int&                          getLowResQ4Mbps() const = 0;
	virtual const int&                          getLowResQ16Mbps() const = 0;
	virtual const int&                          getLowResFullsizeMbps() const = 0;
	virtual const bool&							BlackMagicSwapSDI() const = 0;
	virtual const uint32_t&						GetMCUCOMPort() const = 0;
	virtual const uint32_t&						GetGPIType() const = 0;
	virtual const LTCType&						GetLTCType() const = 0;
	virtual const char*                         getNetworkServerMulticastBindIP() = 0;
	virtual const char*                         getNetworkServerMulticastIP() = 0;
	virtual const uint16_t&                     getNetworkServerMulticastPort() = 0;
	virtual uint16_t							getNetworkServerCmdPort() = 0;
	virtual bool								getFrameSenderIsUsingUdp() const = 0;
	virtual uint32_t                            getNetWorkLiveGap() = 0;
	virtual bool								isNetworkCam(CamID cam) = 0;
	virtual bool								isLocalCam(CamID cam) = 0;
	virtual bool								sendCmdToNetwork() = 0;
	virtual const uint32_t&						getFrameSenderUdpPacketSize() const = 0;
	virtual const ThumbnailResolution&			getReplayThumbnailResolution() const = 0;
	virtual const ThumbnailResolution&			getPGMPRVThumbnailResolution() const = 0;
	virtual const char*                         getudpStreamBindIP() = 0;
	virtual const Encoder264_Hardware&          getEncoder264HardwareType() const = 0;
	virtual const EncoderOut_Conversion&        getEncoder264Needconversion() const = 0;
	virtual const uint32_t&                     getUdpStreamPerWait() const = 0;
	virtual const uint32_t&                     getDnxExportPerWait() const = 0;
	virtual const uint32_t&						getCacheRangeHighRes() const = 0;
	virtual const uint32_t&						getCacheRangeLowRes() const = 0;
	virtual const uint32_t&						getNetworkCacheRangeHighRes() const = 0;
	virtual const uint32_t&						getNetworkCacheRangeLowRes() const = 0;
	virtual const char*							getBrocastDnxIP() = 0;
	virtual const float&						getVideoFps() const = 0;
	virtual const bool&							isAudioSlomo() const = 0;
	virtual const bool&							isAudioScrub() const = 0;
	virtual const char*							getMIDIKeyWords() const = 0;
};
