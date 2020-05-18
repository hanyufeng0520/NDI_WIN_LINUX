#include "Configurator.h"
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#endif _MSC_VER
#include "cnlInfo.h"
#include <filesystem>
#include <algorithm>
#include "../Lib.Base/AudioSampleHeader.h"

static constexpr char const* BACKEND_CONFIG = "C:\\ProgramData\\SimplyLive.TV\\Vibox\\Backend\\Config.ini";

Configurator::Configurator()
{
	setDefault();
#ifdef _MSC_VER
	sprintf_s(m_strConfigPath, "%s", BACKEND_CONFIG);
#else
	char buf[MAX_PATH];
	memset(buf, 0, sizeof(buf));
	GetCurrentPath(buf, "Config.ini");
	strcpy(m_strConfigPath, buf);
#endif _MSC_VER
	load();
}

CamID Configurator::getFirstLocalSDICam() const
{
	return (CamID)m_firstLocalSDICam;
}

const char* Configurator::getGfxNDIChannelName(int _idx) const
{
	return _idx < std::size(m_GfxNDIChannelName) ? m_GfxNDIChannelName[_idx] : "";
}

std::string Configurator::getAudioMixerName() const
{
	return m_AudioMixerName;
}

LayerType Configurator::getLayers(int _l) const
{
	return _l < std::size(m_layers) ? m_layers[_l] : LayerType::LayerType_Gfx1;
}

const LiveTGAType& Configurator::getLiveTGAType() const
{
	return m_liveTGAType;
}

const bool& Configurator::is4K() const
{
	return m_is4K;
}

const bool& Configurator::isDropNTSC() const
{
	return m_isDropNTSC;
}

const bool& Configurator::is720p() const
{
	return m_is720p;
}

const bool& Configurator::isHD() const
{
	return m_isHD;
}

const bool& Configurator::isNTSC() const
{
	return m_isNTSC;
}

const bool& Configurator::isFreeRun() const
{
	return m_isFreeRun;
}

const bool& Configurator::isRTDonClean() const
{
	return m_isRTDonClean;
}

const bool& Configurator::isProgressive() const
{
	return m_isProgressive;
}

const CardType& Configurator::getCardType() const
{
	return m_cardType;
}

const std::string& Configurator::getCardTypeString() const
{
	return m_cardTypeString;
}

const bool& Configurator::getNeedRecPGMDirty() const
{
	return m_recPGMDirty;
}

const bool& Configurator::getNeedRecPGMClean() const
{
	return m_recPGMClean;
}

const ChannelMask& Configurator::getRecChannelMask() const
{
	return m_recChannelMask;
}

int Configurator::getNbRec() const
{
	return m_nbRecChannels;
}

const int& Configurator::getSDINbRec() const
{
	return m_nbSDIRec;
}

const int& Configurator::getStreamNbRec() const
{
	return m_nbStreamRec;
}

int Configurator::getNbPGM() const
{
	return (int)m_PGMChannelMask.count();
}

const ChannelMask& Configurator::getPGMChannelMask() const
{
	return m_PGMChannelMask;
}

const Card_Config& Configurator::getCardConfig() const
{
	return m_stConfig;
}

const float& Configurator::getInterval() const
{
	return m_interval;
}

const bool& Configurator::isUsingCard() const
{
	return m_isUsingCard;
}

const FPTVideoFormat& Configurator::getVideoFormat() const
{
	return m_videoFormat;
}

const std::string& Configurator::getVideoFormatString() const
{
	return m_videoFormatString;
}

const int& Configurator::getFramesPerSec() const
{
	return m_framesPerSec;
}

const int& Configurator::getSendingThumbnailJump() const
{
	return m_sendingThumbnailJump;
}

const bool& Configurator::isBuildMarker() const
{
	return m_isBuildMarker;
}

const bool& Configurator::needReduceThumbnailSendingfps() const
{
	return m_bReduceThumbnailSendingfps;
}

const uint32_t& Configurator::getRtmpOutChunkSize() const
{
	return m_rtmpOutChunkSize;
}

const bool& Configurator::getStreamInNeedSync() const
{
	return   m_bStreamInNeedSync;
}

const ServerType& Configurator::getServerType() const
{
	return m_hardwareType;
}

const char* Configurator::getServerTypeString() const
{
	return m_serverTypeString;
}

const MemoryBaseOnServerType& Configurator::getMaxMemoryGBBaseOnServerType() const
{
	return m_maxMemoryGBBaseOnServerType;
}

const int& Configurator::getMaxUser() const
{
	return m_nMaxUser;
}

const int& Configurator::getAJAInBufferLevel() const
{
	return m_nAJAInBufferLevel;
}

const SlowMotionMode& Configurator::getSlowMotionMode() const
{
	return m_SlowMotionMode;
}

const bool& Configurator::isInLevelA() const
{
	return m_isInLevelA;
}

const bool& Configurator::isOutLevelA() const
{
	return m_isOutLevelA;
}

const int& Configurator::getSubstitutePlayer() const
{
	return m_SubstitutePlayer;
}

const bool& Configurator::isPGMSubstitute() const
{
	return m_bPGMSubstitute;
}

const uint32_t& Configurator::getImportWaitTime() const
{
	return m_importWaitTime;
}

const uint32_t& Configurator::getUdpBufSize() const
{
	return m_udpBufSize;
}

const uint32_t& Configurator::getDefaultReplayJpegQuality() const
{
	return m_defaultReplayJpegQuality;
}

const uint32_t& Configurator::getDefaultPGMPRVJpegQuality() const
{
	return m_defaultPGMPRVJpegQuality;
}

const std::string& Configurator::getPGM1NDIName() const
{
	return m_strPGM1NDIName;
}

const bool& Configurator::isEnableStorageCRC() const
{
	return m_bStorageCRC;
}

const int& Configurator::getEncodeXDCamCpu() const
{
	return m_nEncodeXDCamCpu;
}

const RGBColorModel& Configurator::getRGBColorModel() const
{
	return m_RGBColorModel;
}

const int& Configurator::getFrameRateNum() const
{
	return m_frameRateNum;
}

const int& Configurator::getFrameRateDen() const
{
	return m_frameRateDen;
}

const int& Configurator::getVideoWidth() const
{
	return m_videoWidth;
}

const int& Configurator::getVideoHeight() const
{
	return m_videoHeight;
}

const bool& Configurator::isHighQualityThumbnail() const
{
	return m_isHighQualityThumbnail;
}

const uint32_t& Configurator::getAudioSampleCount() const
{
	return m_audioSampleCount;
}

const bool& Configurator::isDMAPlayout() const
{
	return m_isDMAPlayout;
}

const int& Configurator::getNbCoreReserved() const
{
	return m_nbCoreReserved;
}

bool Configurator::getFrameSenderIsUsingUdp() const
{
	return m_bFrameSenderIsUsingUdp;
}

const uint32_t& Configurator::getFrameSenderUdpPacketSize() const
{
	return m_nFrameSenderUdpPacketSize;
}

const uint32_t& Configurator::getUdpStreamPerWait() const
{
	return m_nUdpStreamPerWait;
}

const uint32_t& Configurator::getDnxExportPerWait() const
{
	return m_nDnxExportPerWait;
}

const uint32_t& Configurator::getCacheRangeHighRes() const
{
	return m_cacheProtectRange_HighRes;
}

const uint32_t& Configurator::getCacheRangeLowRes() const
{
	return m_cacheProtectRange_LowRes;
}

const uint32_t& Configurator::getNetworkCacheRangeHighRes() const
{
	return m_networkCacheProtectRange_HighRes;
}

const uint32_t& Configurator::getNetworkCacheRangeLowRes() const
{
	return m_networkCacheProtectRange_LowRes;
}

const float& Configurator::getVideoFps() const
{
	return m_video_fps;
}

const char* Configurator::getBrocastDnxIP()
{
	return	m_brocastDnxIP;
}

const bool& Configurator::isAudioSlomo() const
{
	return m_bAudioslomo;
}

const bool& Configurator::isAudioScrub() const
{
	return m_bAudioscrub;
}

const char* Configurator::getMIDIKeyWords() const
{
	return static_cast<const char*>(m_strMIDIKeyWords);
}

int Configurator::getNbRec_ALL() const
{
	return m_nbRecChannels;
}

int Configurator::getNbRec_OnlyNetCams() const
{
	return m_nbRecNetCams;
}

int Configurator::getNbRec_OnlyLocalCams() const
{
	return m_nbRecLocalCams;
}

const ThumbnailResolution& Configurator::getReplayThumbnailResolution() const
{
	return m_replayThumbnailResolution;
}

const ThumbnailResolution& Configurator::getPGMPRVThumbnailResolution() const
{
	return m_PGMPRVThumbnailResolution;
}

const char* Configurator::getudpStreamBindIP()
{
	return m_udpStreamBindIP;
}

const Encoder264_Hardware& Configurator::getEncoder264HardwareType() const
{
	return m_h264EncoderHardwareType;
}

const EncoderOut_Conversion& Configurator::getEncoder264Needconversion() const
{
	return m_h264OutConversion;
}

uint32_t Configurator::getNetWorkLiveGap()
{
	return m_NetWorkLiveGap;
}

const bool& Configurator::supportUHD_2SI() const
{
	return m_enableUHD2SI;
}


bool Configurator::isBMR() const
{
	return m_workingMode == WorkingMode::WorkingMode_BMR;
}

bool Configurator::isVibox() const
{
	return m_workingMode == WorkingMode::WorkingMode_Vibox;
}

const bool& Configurator::useExternalTimeCode() const
{
	return m_useExternalTCFlag;
}

const WorkingMode& Configurator::getWorkingMode() const
{
	return m_workingMode;
}

const std::string& Configurator::getWorkingModeString() const
{
	return m_workingModeString;
}

const bool& Configurator::alwaysDisplayUI() const
{
	return m_bAlwaysDisplayUI;
}

const uint32_t& Configurator::getAjaOutBufferthreshold() const
{
	return m_ajaOutBufferThreshold;
}

const uint32_t& Configurator::getMaxAudioLatency() const
{
	return  m_maxAudioLatency;
}

const uint32_t& Configurator::getAudioLatencySecurity() const
{
	return  m_audioLatencySecurity;
}

const bool& Configurator::getDisableLiveToMixer() const
{
	return m_disableLiveToMixer;
}

const bool& Configurator::getDelayPGMVideo() const
{
	return m_delayPGMVideo;
}

const int& Configurator::getLowResQ4Mbps() const
{
	return m_lowResQ4Mbps;
}

const int& Configurator::getLowResQ16Mbps() const
{
	return m_lowResQ16Mbps;
}

const int& Configurator::getLowResFullsizeMbps() const
{
	return m_lowResFullsizeMbps;
}

const bool& Configurator::BlackMagicSwapSDI() const
{
	return m_BlackMagicSwapSDI;
}

const uint32_t& Configurator::GetMCUCOMPort() const
{
	return m_mcuComPort;
}

const uint32_t& Configurator::GetGPIType() const
{
	return m_GPIType;
}

const char* Configurator::getNetworkServerMulticastIP()
{
	return m_networkServerMulticastIP;
}

const uint16_t& Configurator::getNetworkServerMulticastPort()
{
	return m_networkServerMulticastPort;
}

uint16_t Configurator::getNetworkServerCmdPort()
{
	return m_networkServerMulticastPort + 1;
}

const LTCType& Configurator::GetLTCType() const
{
	return m_LTCType;
}

const char* Configurator::getNetworkServerMulticastBindIP()
{
	return m_networkServerMulticastBindIP;
}


int Configurator::load()
{
	m_videoFormat = FPTVideoFormat(readIntValue("GENERAL_CONFIG", (char *)"Video_Format"));
	m_cardType = CardType::CardType_LocalUYVYFile;
	switch (m_cardType)
	{
	case CardType::CardType_LocalUYVYFile:	m_defaultProviderType = FrameProviderType::FPT_YUV_FILE; break;
	case CardType::CardType_LocalMJPEGFile:	m_defaultProviderType = FrameProviderType::FPT_MJPEG_FILE; break;
	case CardType::CardType_BlackMagic:		m_defaultProviderType = FrameProviderType::FPT_BLACKMAGIC; break;
	case CardType::CardType_DeltaCast:		m_defaultProviderType = FrameProviderType::FPT_DELTACAST; break;
	case CardType::CardType_RTMPStream:		m_defaultProviderType = FrameProviderType::FPT_RMTP_STREAM; break;
	case CardType::CardType_TSUDPStream:		m_defaultProviderType = FrameProviderType::FPT_TSUDP_STREAM; break;
	case CardType::CardType_MAGEWELL:			m_defaultProviderType = FrameProviderType::FPT_MAGEWELL; break;
	case CardType::CardType_NDI:				m_defaultProviderType = FrameProviderType::FPT_NDI; break;
	case CardType::CardType_AJA:				m_defaultProviderType = FrameProviderType::FPT_AJA; break;
	default:						m_defaultProviderType = FrameProviderType::FPT_YUV_FILE; break;
	}	
#ifdef _MSC_VER
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (m_nEncodeXDCamCpu > si.dwNumberOfProcessors)
		m_nEncodeXDCamCpu = si.dwNumberOfProcessors;
	if (si.dwNumberOfProcessors > 12)
		m_nbCoreReserved = 0;
#else
	m_nbCoreReserved = 0;
#endif _MSC_VER

	if (m_workingMode >= WorkingMode::WorkingMode_Max)
		m_workingMode = WorkingMode::WorkingMode_Vibox;

	char strValue[MAX_PATH];
	char strItemName[MAX_PATH];

	m_nbSDIRec = 0;
	m_nbStreamRec = 0;
	
	ChannelMask gpuRecorder;
	int nlocalCamIdx = 0;
	m_nbRecNetCams = 0;
	for (CamID nCamera = ID_Cam_0; nCamera < std::size(m_stConfig.recorder); nCamera = CamID(nCamera + 1))
	{
#ifdef _MSC_VER
		sprintf_s(strItemName, "Cam%d", nCamera + 1);
#else
		sprintf(strItemName, "Cam%d", nCamera + 1);
#endif 
		readStringValue("CAMERA_CONFIG", strItemName, strValue);
		std::string strTemp = strValue;

		EM_Channel_Detail recorder;
		recorder.nSDIID = nCamera;
		recorder.providerType = FrameProviderType::FPT_UNKNOWN;
		recorder.isNetworking = false;
		size_t sRet = LoadLSMConfigValue(recorder, nCamera, strValue);

		if (std::string::npos == sRet)
			break;// don't support config hole
		if (!recorder.isNetworking)
		{
			std::string strItemValue(strValue);
			size_t pos = strItemValue.find("\\");
			if (std::string::npos != pos && pos < sRet)
			{
				std::string serverName = strItemValue.substr(0, pos);
				std::transform(serverName.begin(), serverName.end(), serverName.begin(), ::tolower);				
			}
		}
		m_recChannelMask.set(nCamera);
		gpuRecorder.set(nCamera);
		recorder.chlType = EM_Channel_Type::EM_Channel_Type_Provider;
		recorder.nIndexID = nCamera;
		if (false)
		{
		}
		else {
			//localCam
			m_map_128_to_16[nCamera] = (CamID)nlocalCamIdx;
			m_map_16_to_128[nlocalCamIdx] = (CamID)nCamera;//localCamIDx will less than 16(NB_LOCAL_MAXCAM-3)
			nlocalCamIdx++;
		}

		if (FrameProviderType::FPT_AJA == recorder.providerType || FrameProviderType::FPT_BLACKMAGIC == recorder.providerType || FrameProviderType::FPT_DELTACAST == recorder.providerType || FrameProviderType::FPT_MAGEWELL == recorder.providerType)
		{
			++m_nbSDIRec;
		}
		else if (FrameProviderType::FPT_RMTP_STREAM == recorder.providerType || FrameProviderType::FPT_TSUDP_STREAM == recorder.providerType || FrameProviderType::FPT_NDI == recorder.providerType)
			++m_nbStreamRec;
		recorder.camID = nCamera;
		m_stConfig.recorder[nCamera] = recorder;
	}

	m_firstLocalSDICam = 0;
	int sncount = 0;	

	prepareData();

	if (is4K())
	{
		m_recPGMDirty = false;
		m_recPGMClean = false;
		if (WorkingMode::WorkingMode_BMR == m_workingMode)
		{
			m_recChannelMask = (m_nbSDIRec == 1) ? 1 : 3;
			gpuRecorder = (m_nbSDIRec == 1) ? 1 : 3;;
		}
	}

	m_recChannelMask.set(ID_Cam_Import);
	gpuRecorder.set(ID_Cam_Import);

	if (m_recPGMDirty)
	{
		m_recChannelMask.set(ID_Cam_PGMDirty);
		gpuRecorder.set(ID_Cam_PGMDirty);
	}

	if (m_recPGMClean)
	{
		m_recChannelMask.set(ID_Cam_PGMClean);
		gpuRecorder.set(ID_Cam_PGMClean);
	}

	m_nbRecChannels = m_recChannelMask.count();
	m_nbRecLocalCams = m_nbRecChannels - m_nbRecNetCams;
	int activePlayMask = 0;
	for (int nCamera = 0; nCamera < m_nMaxUser; nCamera++)
		activePlayMask |= 1 << nCamera;
	return 0;
}

void Configurator::prepareData()
{
	m_isDropNTSC = m_videoFormat == FPTVideoFormat::FP_FORMAT_1080i_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_720p_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_720p_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_4Kp_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_4Kp_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_2398 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_8kp_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_8kp_5994;

	m_isNTSC = m_videoFormat == FPTVideoFormat::FP_FORMAT_1080i_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080i_6000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_2398 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_3000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_1080p_6000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_720p_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_720p_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_720p_6000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_4Kp_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_4Kp_3000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_4Kp_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_4Kp_6000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_8kp_2997 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_8kp_3000 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_8kp_5994 ||
		m_videoFormat == FPTVideoFormat::FP_FORMAT_8kp_6000;

	m_isProgressive = !(m_videoFormat == FPTVideoFormat::FP_FORMAT_1080i_5994 || m_videoFormat == FPTVideoFormat::FP_FORMAT_1080i_6000 || m_videoFormat == FPTVideoFormat::FP_FORMAT_1080i_5000);
	m_isUsingCard = m_cardType == CardType::CardType_AJA || m_cardType == CardType::CardType_BlackMagic || m_cardType == CardType::CardType_DeltaCast || m_cardType == CardType::CardType_MAGEWELL;

	switch (m_cardType)
	{
	case CardType::CardType_AJA:            m_cardTypeString = "AJA";            break;
	case CardType::CardType_LocalUYVYFile:  m_cardTypeString = "LocalUYVYFile";  break;
	case CardType::CardType_LocalMJPEGFile: m_cardTypeString = "LocalMJPEGFile"; break;
	case CardType::CardType_BlackMagic:     m_cardTypeString = "BlackMagic";     break;
	case CardType::CardType_DeltaCast:      m_cardTypeString = "DeltaCast";      break;
	case CardType::CardType_RTMPStream:     m_cardTypeString = "RTMPStream";     break;
	case CardType::CardType_TSUDPStream:    m_cardTypeString = "TSUDPStream";    break;
	case CardType::CardType_MAGEWELL:		  m_cardTypeString = "Magewell";	   break;
	case CardType::CardType_NDI:		      m_cardTypeString = "NDI";			   break;
	case CardType::CardType_Max:
	default:                      m_cardTypeString = "Unknown Card";   break;
	}

	switch (m_videoFormat)
	{
	case FPTVideoFormat::FP_FORMAT_1080p_5000:
	case FPTVideoFormat::FP_FORMAT_1080p_5994:
	case FPTVideoFormat::FP_FORMAT_1080p_6000:
	case FPTVideoFormat::FP_FORMAT_720p_5000:
	case FPTVideoFormat::FP_FORMAT_720p_5994:
	case FPTVideoFormat::FP_FORMAT_720p_6000:
	case FPTVideoFormat::FP_FORMAT_4Kp_5000:
	case FPTVideoFormat::FP_FORMAT_4Kp_5994:
	case FPTVideoFormat::FP_FORMAT_SD_525_5994:
	case FPTVideoFormat::FP_FORMAT_SD_625_5000:
	case FPTVideoFormat::FP_FORMAT_8kp_5000:
	case FPTVideoFormat::FP_FORMAT_8kp_5994:
	case FPTVideoFormat::FP_FORMAT_8kp_6000:
	case FPTVideoFormat::FP_FORMAT_4Kp_6000: m_sendingThumbnailJump = m_bReduceThumbnailSendingfps ? 2 : 1; break;
	case FPTVideoFormat::FP_FORMAT_1080i_5000:
	case FPTVideoFormat::FP_FORMAT_1080i_5994:
	case FPTVideoFormat::FP_FORMAT_1080i_6000:
	case FPTVideoFormat::FP_FORMAT_1080p_2400:
	case FPTVideoFormat::FP_FORMAT_1080p_2500:
	case FPTVideoFormat::FP_FORMAT_1080p_2997:
	case FPTVideoFormat::FP_FORMAT_1080p_3000:
	case FPTVideoFormat::FP_FORMAT_720p_2500:
	case FPTVideoFormat::FP_FORMAT_720p_2997:
	case FPTVideoFormat::FP_FORMAT_4Kp_2400:
	case FPTVideoFormat::FP_FORMAT_4Kp_2500:
	case FPTVideoFormat::FP_FORMAT_4Kp_2997:
	case FPTVideoFormat::FP_FORMAT_4Kp_3000:
	case FPTVideoFormat::FP_FORMAT_UNKNOWN:
	case FPTVideoFormat::FP_FORMAT_1080p_2398:
	case FPTVideoFormat::FP_FORMAT_8kp_2400:
	case FPTVideoFormat::FP_FORMAT_8kp_2500:
	case FPTVideoFormat::FP_FORMAT_8kp_2997:
	case FPTVideoFormat::FP_FORMAT_8kp_3000:
	default: m_sendingThumbnailJump = 1; break;
	}

	m_videoWidth = FFMPEG_AUDIOSAMPLE::getFrameWidth(m_videoFormat);
	m_videoHeight = FFMPEG_AUDIOSAMPLE::getFrameHeight(m_videoFormat);
	m_isHD = (m_videoHeight == 1080) && m_videoWidth == 1920;
	m_is720p = (m_videoHeight == 720) && m_videoWidth == 1280;
	m_is4K = (m_videoHeight == 2160) && m_videoWidth == 3840;

	m_frameRateDen = FFMPEG_AUDIOSAMPLE::getFrameRateDen(m_videoFormat);
	m_frameRateNum = FFMPEG_AUDIOSAMPLE::getFrameRateNum(m_videoFormat);
	m_interval = float(m_frameRateDen) * 1000.0 / float(m_frameRateNum);
	m_video_fps = float(m_frameRateNum) / float(m_frameRateDen);
	m_videoFormatString = FFMPEG_AUDIOSAMPLE::getVideoFormatString(m_videoFormat);
	m_framesPerSec = m_frameRateNum / m_frameRateDen;
	if (m_frameRateNum % m_frameRateDen != 0)
		++m_framesPerSec;

	int len;
	const int* arr = FFMPEG_AUDIOSAMPLE::getFrameSamples(m_videoFormat, len);
	for (int i = 0; i < len; i++)
		m_audioSampleCount = m_audioSampleCount > arr[i] ? m_audioSampleCount:arr[i];

	if (m_audioSampleCount == 0)
		m_audioSampleCount = 1920;

	switch (m_workingMode)
	{
	case WorkingMode::WorkingMode_Vibox:  m_workingModeString = "Vibox";	 break;
	case WorkingMode::WorkingMode_BMR:    m_workingModeString = "BMR";    break;
	case WorkingMode::WorkingMode_Max:
	default:                 m_workingModeString = "Unknown mode"; break;
	}
}

int Configurator::setDefault()
{
	m_isDMAPlayout = false;
	m_recChannelMask.reset();
	m_PGMChannelMask.reset();
	m_workingMode = WorkingMode::WorkingMode_Vibox;
	m_isFreeRun = false;
	m_recPGMDirty = false;
	m_recPGMClean = false;
	m_liveTGAType = LiveTGAType::LiveTGAType_OnTCP;
	m_cardType = CardType::CardType_LocalMJPEGFile;
	m_bAlwaysDisplayUI = true;
	m_useExternalTCFlag = false;
	m_nbSDIRec = 0;
	m_nbStreamRec = 0;
	m_ajaOutBufferThreshold = 4;
	m_maxAudioLatency = 10;
	m_nbCoreReserved = 0;
	m_bFrameSenderIsUsingUdp = false;
	m_enableUHD2SI = false;
	m_bReduceThumbnailSendingfps = false;
	m_rtmpOutChunkSize = 128;
	m_bStreamInNeedSync = false;
	m_hardwareType = ServerType::Server_8cnl;
	m_serverTypeString = "8 channels";
	m_nAJAInBufferLevel = 5;
	m_isInLevelA = true;
	m_isOutLevelA = true;
	m_udpBufSize = 4096;
	m_replayThumbnailResolution.set_value(ThumbnailResolutionType::thumbnail_480x270, 480, 270);
	m_PGMPRVThumbnailResolution.set_value(ThumbnailResolutionType::thumbnail_960x540, 960, 540);
	for (int i = 0; i < std::size(m_map_128_to_16); i++)
		m_map_128_to_16[i] = ID_InValidCam;

	for (int i = 0; i < std::size(m_map_16_to_128); i++)
		m_map_16_to_128[i] = ID_InValidCam;
	return 0;
}

size_t Configurator::LoadLSMConfigValue(EM_Channel_Detail &recorder, CamID nCameraIndex, char* pValue)
{
	recorder.providerType = FrameProviderType::FPT_UNKNOWN;
	const std::string strItemValue = pValue;

	const char* pFind = "SDI:";
	size_t nFindSize = strlen(pFind);
	size_t nLeftPos = strItemValue.find(pFind);
	size_t nRightPos = strItemValue.size();
	if (std::string::npos != nLeftPos)//SDI input
	{
		if (FrameProviderType::FPT_AJA == m_defaultProviderType || FrameProviderType::FPT_BLACKMAGIC == m_defaultProviderType ||
			FrameProviderType::FPT_DELTACAST == m_defaultProviderType || FrameProviderType::FPT_MAGEWELL == m_defaultProviderType)
		{
			recorder.nSDIID = atoi(strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize)).c_str()) - 1;
			recorder.providerType = m_defaultProviderType;
			return nLeftPos;
		}
		return std::string::npos;
	}

	pFind = "RTMP";
	nFindSize = strlen(pFind);
	nLeftPos = strItemValue.find(pFind);
	if (std::string::npos != nLeftPos) //RTMP
	{
#ifdef _MSC_VER
		sprintf_s(recorder.szItemName, MAX_PATH, "Cam%c", 'A' + nCameraIndex);
#else
		sprintf(recorder.szItemName, "Cam%c", 'A' + nCameraIndex);
#endif 
		recorder.providerType = FrameProviderType::FPT_RMTP_STREAM;
		return nLeftPos;
	}

	pFind = "UDP:\"";
	nFindSize = strlen(pFind);
	nLeftPos = strItemValue.find(pFind);
	if (std::string::npos != nLeftPos) //UDP
	{
#ifdef _MSC_VER
		sprintf_s(recorder.szItemName, MAX_PATH, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize) - 1).c_str());
#else
		sprintf(recorder.szItemName, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize) - 1).c_str());
#endif 
		recorder.providerType = FrameProviderType::FPT_TSUDP_STREAM;
		return nLeftPos;
	}

	pFind = "FILE:\"";
	nFindSize = strlen(pFind);
	nLeftPos = strItemValue.find(pFind);
	if (std::string::npos != nLeftPos) //FILE
	{
		nRightPos = strItemValue.find('\"', nLeftPos + nFindSize);
		if (std::string::npos == nRightPos)
			return std::string::npos;
#ifdef _MSC_VER
		sprintf_s(recorder.szItemName, MAX_PATH, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize)).c_str());
#else
		sprintf(recorder.szItemName, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize)).c_str());
#endif 
		size_t nLeftPos_1 = nRightPos;
		nLeftPos_1 = strItemValue.find("|\"", nLeftPos_1);
		if (std::string::npos != nLeftPos_1)
		{
			nFindSize = 2;
			nRightPos = strItemValue.size();
			if (std::string::npos != nRightPos)
			{
#ifdef _MSC_VER
				sprintf_s(recorder.szAudioName, MAX_PATH, "%s",
					strItemValue.substr(nLeftPos_1 + nFindSize, nRightPos - (nLeftPos_1 + nFindSize) - 1).c_str());
#else
				sprintf(recorder.szAudioName, "%s",
					strItemValue.substr(nLeftPos_1 + nFindSize, nRightPos - (nLeftPos_1 + nFindSize) - 1).c_str());
#endif
			}
		}
		if (m_defaultProviderType == FrameProviderType::FPT_MJPEG_FILE)
			recorder.providerType = m_defaultProviderType;
		else
			recorder.providerType = FrameProviderType::FPT_YUV_FILE;
		return nLeftPos;
	}

	pFind = "NDI:\"";
	nFindSize = strlen(pFind);
	nLeftPos = strItemValue.find(pFind);
	if (std::string::npos != nLeftPos) //NDI
	{
#ifdef _MSC_VER
		sprintf_s(recorder.szItemName, MAX_PATH, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize) - 1).c_str());
#else
		sprintf(recorder.szItemName, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize) - 1).c_str());
#endif 
		recorder.providerType = FrameProviderType::FPT_NDI;
		return nLeftPos;
	}
	return std::string::npos;
}

bool Configurator::isNetworkCam(CamID cam)
{
	if (int(cam) >= std::size(m_stConfig.recorder))
		return false;
	return m_stConfig.recorder[cam].isNetworking;
}

bool Configurator::isLocalCam(CamID cam)
{
	if (int(cam) >= std::size(m_stConfig.recorder))
		return true;
	return !m_stConfig.recorder[cam].isNetworking;
}


const ChannelMask& Configurator::getNetworkChannelMask() const
{
	return m_networkChannelMask;
}

bool Configurator::sendCmdToNetwork()
{
	return m_sendCmdToNetwork;
}

void Configurator::GetCurrentPath(char buf[], char* pFileName)
{
#ifdef _MSC_VER
	GetModuleFileNameA(NULL, buf, MAX_PATH);
#else
	char pidfile[64];
	int bytes;
	int fd;
	sprintf(pidfile, "/proc/%d/cmdline", getpid());
	fd = open(pidfile, O_RDONLY, 0);
	bytes = read(fd, buf, MAX_PATH);
	close(fd);
	buf[MAX_PATH] = '\0';
#endif
	char* p = &buf[strlen(buf)];
	do
	{
		*p = '\0';
		p--;
#ifdef _MSC_VER
	} while ('\\' != *p);
#else
	} while ('/' != *p);
#endif
	p++;
	memcpy(p, pFileName, strlen(pFileName));
}

void Configurator::IniReadValue(char* section, char* key, char* val)
{
#ifndef _MSC_VER
	FILE* fp;
	int i = 0;
	int lineContentLen = 0;
	int position = 0;
	char lineContent[MAX_PATH];
	bool bFoundSection = false;
	bool bFoundKey = false;
	fp = fopen(m_strConfigPath, "r");
	if (fp == NULL)
	{
		printf("%s: Opent file %s failed.\n", __FILE__, m_strConfigPath);
		return;
	}
	while (feof(fp) == 0)
	{
		memset(lineContent, 0, MAX_PATH);
		fgets(lineContent, MAX_PATH, fp);
		if ((lineContent[0] == ';') || (lineContent[0] == '\0') || (lineContent[0] == '\r') || (lineContent[0] == '\n'))
		{
			continue;
		}

		//check section
		if (strncmp(lineContent, section, strlen(section)) == 0)
		{
			bFoundSection = true;
			//printf("Found section = %s\n", lineContent);
			while (feof(fp) == 0 && !bFoundKey)
			{
				memset(lineContent, 0, MAX_PATH);
				fgets(lineContent, MAX_PATH, fp);
				//check key
				if (strncmp(lineContent, key, strlen(key)) == 0)
				{
					bFoundKey = true;
					lineContentLen = strlen(lineContent);
					//find value
					for (i = strlen(key); i < lineContentLen; i++)
					{
						if (lineContent[i] == '=')
						{
							position = i + 1;
							break;
						}
					}
					if (i >= lineContentLen) 
						break;
					strncpy(val, lineContent + position, strlen(lineContent + position));
					lineContentLen = strlen(val);
					for (i = 0; i < lineContentLen; i++)
					{
						if ((lineContent[i] == '\0') || (lineContent[i] == '\r') || (lineContent[i] == '\n'))
						{
							val[i] = '\0';
							break;
						}
					}
				}
				else if (lineContent[0] == '[')
				{
					break;
				}
			}
			break;
		}
	}
	if (!bFoundSection) 
		printf("No section = %s\n", section); 
	else if (!bFoundKey) 
		printf("No key = %s\n", key);
	fclose(fp);
#endif
}

int Configurator::readStringValue(const char* section, char* key, char* val)
{	
	char sect[MAX_PATH];
	if (section == NULL || key == NULL || val == NULL)
	{
		printf("%s: input parameter(s) is NULL!\n", __func__);
		return -1;
	}
	memset(sect, 0, MAX_PATH);	
#ifdef _MSC_VER
	GetPrivateProfileStringA(section, key, "", val, MAX_PATH, m_strConfigPath);
#else
	sprintf(sect, "[%s]", section);
	IniReadValue(sect, key, val);
#endif _MSC_VER
	return 0;
}

int Configurator::readIntValue(const char* section, char* key)
{
	char strValue[MAX_PATH];
	memset(strValue, '\0', MAX_PATH);
#ifdef _MSC_VER
	return GetPrivateProfileIntA(section, key, static_cast<int>(FPTVideoFormat::FP_FORMAT_1080i_5000), m_strConfigPath);
#else
	if (readStringValue(section, key, strValue) != 0)
	{
		printf("%s: error", __func__);
		return 0;
	}
	return(atoi(strValue));
#endif	
}

