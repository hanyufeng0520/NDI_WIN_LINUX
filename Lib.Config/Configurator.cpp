#include "Configurator.h"
#include <tchar.h>
#include <Windows.h>
#include "cnlInfo.h"
#include <filesystem>
#include <io.h>
#include "../Lib.Base/StringHelper.h"
#include "../Lib.Base/CommonHelper.h"
#include "AudioSampleHeader.h"

Configurator::Configurator()
{
	setDefault();
	sprintf_s(m_strConfigPath, R"(%sConfig.ini)", GetAppPathA().c_str());
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
	m_sendCmdToNetwork = GetPrivateProfileIntA("NetworkFunction", "SendCmdToNetwork", false, m_strConfigPath);
	GetPrivateProfileStringA("NetworkFunction", "multicast_BindIP", "", m_networkServerMulticastBindIP, sizeof(m_networkServerMulticastBindIP), m_strConfigPath);
	GetPrivateProfileStringA("NetworkFunction", "multicast_ip", "", m_networkServerMulticastIP, sizeof(m_networkServerMulticastIP), m_strConfigPath);
	m_networkServerMulticastPort = uint16_t(GetPrivateProfileIntA("NetworkFunction", "multicast_port", 39100, m_strConfigPath));
	std::string_view multicast_ip(m_networkServerMulticastIP);
	if (!multicast_ip.empty())
		m_NetWorkLiveGap = GetPrivateProfileIntA("NetworkFunction", "networkLiveGap", 0, m_strConfigPath);
	
	GetPrivateProfileStringA("GENERAL_CONFIG", "broadcast_dnx_ip", "", m_brocastDnxIP, sizeof(m_brocastDnxIP), m_strConfigPath);
	
	m_lowResFullsizeMbps = GetPrivateProfileIntA("BandwidthRegulation", "FullSizeMbps", 0, m_strConfigPath);
	m_lowResQ4Mbps = GetPrivateProfileIntA("BandwidthRegulation", "Q4Mbps", 0, m_strConfigPath);
	m_lowResQ16Mbps = GetPrivateProfileIntA("BandwidthRegulation", "Q16Mbps", 0, m_strConfigPath);

	m_isHighQualityThumbnail = GetPrivateProfileIntA("GENERAL_CONFIG", "HighQualityThumbnail", false, m_strConfigPath);
	m_SlowMotionMode = SlowMotionMode(GetPrivateProfileIntA("GENERAL_CONFIG", "SlowMotionMode", static_cast<int>(SlowMotionMode::SlowMotionMode_parity_violation), m_strConfigPath));
	m_videoFormat = FPTVideoFormat(GetPrivateProfileIntA("GENERAL_CONFIG", "Video_Format", static_cast<int>(FPTVideoFormat::FP_FORMAT_1080i_5000), m_strConfigPath));
	m_isFreeRun = GetPrivateProfileIntA("GENERAL_CONFIG", "Free_Run", true, m_strConfigPath);
	m_recPGMDirty = GetPrivateProfileIntA("GENERAL_CONFIG", "WritePGMDirty", false, m_strConfigPath);
	m_recPGMClean = GetPrivateProfileIntA("GENERAL_CONFIG", "WritePGMClean", false, m_strConfigPath);
	m_workingMode = WorkingMode(GetPrivateProfileIntA("GENERAL_CONFIG", "Working_Mode", static_cast<int>(WorkingMode::WorkingMode_Vibox), m_strConfigPath));
	m_useExternalTCFlag = GetPrivateProfileIntA("GENERAL_CONFIG", "External_TimeCode", false, m_strConfigPath);
	m_liveTGAType = LiveTGAType(GetPrivateProfileIntA("GENERAL_CONFIG", "LiveTGA", static_cast<int>(LiveTGAType::LiveTGAType_OnTCP), m_strConfigPath));
	GetPrivateProfileStringA("GENERAL_CONFIG", "LiveTGA_NDIChannelName", "", m_GfxNDIChannelName[0], sizeof(m_GfxNDIChannelName[0]), m_strConfigPath);
	GetPrivateProfileStringA("GENERAL_CONFIG", "LiveTGA_NDIChannelName_2", "", m_GfxNDIChannelName[1], sizeof(m_GfxNDIChannelName[1]), m_strConfigPath);
	if (m_liveTGAType == LiveTGAType::LiveTGAType_NDI && _strnicmp(m_GfxNDIChannelName[0], m_GfxNDIChannelName[1], max(strlen(m_GfxNDIChannelName[0]), strlen(m_GfxNDIChannelName[1]))) == 0)
	{
		memset(&m_GfxNDIChannelName[1], 0, sizeof(m_GfxNDIChannelName[1]));
	}

	GetPrivateProfileStringA("GENERAL_CONFIG", "AudioMixerName", "", m_AudioMixerName, sizeof(m_AudioMixerName), m_strConfigPath);
	m_RGBColorModel = RGBColorModel(GetPrivateProfileIntA("GENERAL_CONFIG", "RGBColorModel", static_cast<int>(RGBColorModel::BT709), m_strConfigPath));

	m_bFrameSenderIsUsingUdp = GetPrivateProfileIntA("GENERAL_CONFIG", "FrameSenderIsUsingUdp", 0, m_strConfigPath);
	m_nFrameSenderUdpPacketSize = GetPrivateProfileIntA("GENERAL_CONFIG", "FrameSenderUdpPacketSize", 1400, m_strConfigPath);
	if (m_nFrameSenderUdpPacketSize < 100)
		m_nFrameSenderUdpPacketSize = 100;
	m_nUdpStreamPerWait = GetPrivateProfileIntA("GENERAL_CONFIG", "udpstreamper_wait", 2, m_strConfigPath);
	m_nDnxExportPerWait = GetPrivateProfileIntA("GENERAL_CONFIG", "dnxexportper_wait", 5, m_strConfigPath);

	m_disableLiveToMixer = GetPrivateProfileIntA("GENERAL_CONFIG", "DisableLiveToMixer", false, m_strConfigPath);
	m_delayPGMVideo = GetPrivateProfileIntA("GENERAL_CONFIG", "DelayPGMVideo", false, m_strConfigPath);

	if (is4K())
		m_enableUHD2SI = GetPrivateProfileIntA("GENERAL_CONFIG", "EnableUHD2SI", false, m_strConfigPath);

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
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (m_nEncodeXDCamCpu > si.dwNumberOfProcessors)
		m_nEncodeXDCamCpu = si.dwNumberOfProcessors;
	if (si.dwNumberOfProcessors > 12)
		m_nbCoreReserved = 0;

#ifdef NDEBUG
	m_bAlwaysDisplayUI = GetPrivateProfileIntA("GENERAL", "Always_Display_UI", false, HARDWARE_CONFIG);			//never hide UI in debug mode
#endif

	char value[MAX_PATH];
	GetPrivateProfileStringA("GENERAL_CONFIG", "4K_In_Level", "A", value, MAX_PATH, m_strConfigPath);
	m_isInLevelA = (_strnicmp(value, "A", strlen("A")) == 0);

	GetPrivateProfileStringA("GENERAL_CONFIG", "4K_Out_Level", "A", value, MAX_PATH, m_strConfigPath);
	m_isOutLevelA = (_strnicmp(value, "A", strlen("A")) == 0);

	if (m_workingMode >= WorkingMode::WorkingMode_Max)
		m_workingMode = WorkingMode::WorkingMode_Vibox;

	CHAR strValue[MAX_PATH];
	CHAR strItemName[MAX_PATH];

	m_nbSDIRec = 0;
	m_nbStreamRec = 0;
	m_layers[0] = LayerType::LayerType_Gfx1;
	m_layers[1] = LayerType::LayerType_Gfx2;
	m_layers[2] = LayerType::LayerType_Gfx3;
	m_layers[3] = LayerType::LayerType_RTD;

	for (int nCamera = 0; nCamera < static_cast<int>(LayerType::LayerType_Max); nCamera++)
	{
		sprintf_s(strItemName, "LAYER_%d", nCamera + 1);
		GetPrivateProfileStringA("LAYERS", strItemName, "", strValue, MAX_PATH, m_strConfigPath);
		if (_strnicmp(strValue, "RTD", 3) == 0)
			m_layers[nCamera] = LayerType::LayerType_RTD;
		else if (_strnicmp(strValue, "GFX1", 4) == 0)
			m_layers[nCamera] = LayerType::LayerType_Gfx1;
		else if (_strnicmp(strValue, "GFX2", 4) == 0)
			m_layers[nCamera] = LayerType::LayerType_Gfx2;
		else if (_strnicmp(strValue, "GFX3", 4) == 0)
			m_layers[nCamera] = LayerType::LayerType_Gfx3;
	}
	m_isRTDonClean = GetPrivateProfileIntA("LAYERS", "RTD_ON_CLEAN", true, m_strConfigPath);

	ChannelMask gpuRecorder;
	int nlocalCamIdx = 0;
	m_nbRecNetCams = 0;
	for (CamID nCamera = ID_Cam_0; nCamera < std::size(m_stConfig.recorder); nCamera = CamID(nCamera + 1))
	{
		sprintf_s(strItemName, "Cam%d", nCamera + 1);
		GetPrivateProfileStringA("CAMERA_CONFIG", strItemName, "", strValue, MAX_PATH, m_strConfigPath);
		std::string strTemp = strValue;

		EM_Channel_Detail recorder;
		recorder.nSDIID = nCamera;
		recorder.providerType = FrameProviderType::FPT_UNKNOWN;
		recorder.isNetworking = false;
		size_t sRet = LoadLSMConfigValue(recorder, nCamera, strValue);

		if (std::string::npos == sRet)break;// don't support config hole

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

	loadPGM1NDIConfig();
	prepareData();

	m_replayThumbnailResolution.set_value(ThumbnailResolutionType(GetPrivateProfileIntA("GENERAL_CONFIG", "ReplayThumbnailResolution", static_cast<int>(ThumbnailResolutionType::thumbnail_480x270), m_strConfigPath)),
		m_videoWidth, m_videoHeight);
	m_PGMPRVThumbnailResolution.set_value(ThumbnailResolutionType(GetPrivateProfileIntA("GENERAL_CONFIG", "PGMPRVThumbnailResolution", static_cast<int>(ThumbnailResolutionType::thumbnail_960x540), m_strConfigPath)),
		m_videoWidth, m_videoHeight);

	m_h264EncoderHardwareType = Encoder264_Hardware(GetPrivateProfileIntA("ENCODER_264", "encode_hardware", static_cast<int>(Encoder264_Hardware::hardware_GPU), m_strConfigPath));
	m_h264OutConversion = EncoderOut_Conversion(GetPrivateProfileIntA("ENCODER_264", "conversion", static_cast<int>(EncoderOut_Conversion::conversion_NO), m_strConfigPath));
	if (m_h264EncoderHardwareType == Encoder264_Hardware::hardware_CPU)
		m_h264OutConversion = EncoderOut_Conversion::conversion_NO;

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
	std::error_code error;
	if (!std::filesystem::exists(m_strConfigPath, error))
	{
		printf_s("  config file not found\n");
		return -1;
	}
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

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	uint64_t totalSystemMemGB = statex.ullTotalPhys / 1024 / 1024 / 1024;
	switch (m_hardwareType)
	{
	case ServerType::Server_0cnl:		m_serverTypeString = "0 channel";	m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_8GB;  break;
	case ServerType::Server_8cnl:		m_serverTypeString = "8 channels";	m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_64GB;  break;
	case ServerType::Server_6cnl:       m_serverTypeString = "6 channels";	m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_32GB;  break;
	case ServerType::Server_DemoNUC:    m_serverTypeString = "Demo NUC";	m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_64GB;  break;
	case ServerType::Server_16cnl:		m_serverTypeString = "16 channels";	m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_64GB;  break;
	case ServerType::Server_4K:         m_serverTypeString = "4K";			m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_128GB; break;
	default:			    m_serverTypeString = "unknown";		m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_64GB;  break;
	}

	if (totalSystemMemGB < 48)
	{
		if (totalSystemMemGB < 30)
			m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_8GB;
		else
			m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_32GB;
		//WriteLogA(LOGPATH, LOGLEVEL::Warn, "BE is %s but only have %uGB memory, so change to 32GB memory config", m_serverTypeString, totalSystemMemGB);
	}
	else if (totalSystemMemGB < 85 && m_maxMemoryGBBaseOnServerType == MemoryBaseOnServerType::Memory_128GB)
	{
		m_maxMemoryGBBaseOnServerType = MemoryBaseOnServerType::Memory_64GB;
		//WriteLogA(LOGPATH, LOGLEVEL::Warn, "BE is %s but only have %uGB memory, so change to 64GB memory config", m_serverTypeString, totalSystemMemGB);
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
		m_audioSampleCount = max(m_audioSampleCount, arr[i]);

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
		sprintf_s(recorder.szItemName, MAX_PATH, "Cam%c", 'A' + nCameraIndex);
		recorder.providerType = FrameProviderType::FPT_RMTP_STREAM;
		return nLeftPos;
	}

	pFind = "UDP:\"";
	nFindSize = strlen(pFind);
	nLeftPos = strItemValue.find(pFind);
	if (std::string::npos != nLeftPos) //UDP
	{
		sprintf_s(recorder.szItemName, MAX_PATH, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize) - 1).c_str());
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
		sprintf_s(recorder.szItemName, MAX_PATH, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize)).c_str());
		size_t nLeftPos_1 = nRightPos;
		nLeftPos_1 = strItemValue.find("|\"", nLeftPos_1);
		if (std::string::npos != nLeftPos_1)
		{
			nFindSize = 2;
			nRightPos = strItemValue.size();
			if (std::string::npos != nRightPos)
			{
				sprintf_s(recorder.szAudioName, MAX_PATH, "%s",
					strItemValue.substr(nLeftPos_1 + nFindSize, nRightPos - (nLeftPos_1 + nFindSize) - 1).c_str());
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
		sprintf_s(recorder.szItemName, MAX_PATH, "%s",
			strItemValue.substr(nLeftPos + nFindSize, nRightPos - (nLeftPos + nFindSize) - 1).c_str());
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

void Configurator::loadPGM1NDIConfig()
{
	CHAR strValue[MAX_PATH];
	GetPrivateProfileStringA("CAMERA_CONFIG", "PGM1_NDI", "", strValue, MAX_PATH, m_strConfigPath);
	m_strPGM1NDIName = strValue;
	if (!m_strPGM1NDIName.empty())
	{
		m_PGMChannelMask.set(EM_ConsumerChannel_PGM1_NDI);
		m_stConfig.player[EM_ConsumerChannel_PGM1_NDI].chlType = EM_Channel_Type::EM_Channel_Type_Consumer;
		m_stConfig.player[EM_ConsumerChannel_PGM1_NDI].nIndexID = EM_ConsumerChannel_PGM1_NDI;
		m_stConfig.player[EM_ConsumerChannel_PGM1_NDI].nSDIID = EM_ConsumerChannel_PGM1_NDI;
		sprintf_s(m_stConfig.player[EM_ConsumerChannel_PGM1_NDI].szItemName, MAX_PATH, "%s", strValue);
		++m_nbStreamRec;
	}
}
