#include <cstdint>
#include "../Lib.FrameProviderYuvFile/FrameProviderYuvFile.h"
#include "../Lib.FrameConsumerNDI/FrameConsumerNDI.h"
#include "../Lib.Base/CapturePoolMgr.h"
#include "../lib.Config/IConfig.h"

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swresample.lib")

class Cnl : public IFPInputCallBack
{
	CFrameProviderYuvFile	m_provider;
	CFrameConsumerNDI		m_consumer;
	int m_cnlId = -1;
	uint32_t				m_FramesDropped = 0;
	char					m_NDI_name[50];

public:
	Cnl();
	int init(int _cnlId, const char* _audioFileName, const char* _videoFileName, const char* _cnlName);
	int start();
	int stop();

	void cb(uint32_t _channelID, pVFrame pFrameVideo = nullptr, pCVframe pFrameVideo960 = nullptr, pCVframe pFrameVideo480 = nullptr, pAframe pFrameAudio = nullptr);
	void cb_FPS(uint32_t _channelID, int vFps = 0, int aFPS = 0);
};

Cnl::Cnl()
{
}

int Cnl::init(int _cnlId, const char* _audioFileName, const char* _videoFileName, const char* _cnlName)
{
	printf("Init %s %s %s\n", _audioFileName, _videoFileName, _cnlName);
	sFrameProvider_Parameter paramProvid;
	paramProvid.fpVideoFormat = Config->getVideoFormat();
	sprintf_s(paramProvid.szFileName, sizeof(paramProvid.szFileName), _videoFileName);
	sprintf_s(paramProvid.szFileNameAudio, sizeof(paramProvid.szFileNameAudio), _audioFileName);
	sprintf_s(m_NDI_name, _cnlName);
	m_cnlId = _cnlId;
	m_provider.addChannel(m_cnlId, paramProvid, this);

	sFrameConsumer_Parameter	paramConsum;
	paramConsum.fcType = FCT_NDI;
	paramConsum.fpVideoFormat = Config->getVideoFormat();
	paramConsum.name = _cnlName;
	m_consumer.addChannel(m_cnlId, paramConsum);

	return 0;
}

void Cnl::cb(uint32_t _channelID, pVFrame pFrameVideo, pCVframe pFrameVideo960, pCVframe pFrameVideo480, pAframe pFrameAudio)
{
	while (m_consumer.OutputFrames(_channelID, pFrameVideo, pFrameAudio, nullptr, m_FramesDropped))
		Sleep(1);
	m_provider.frameConsumed();
}

void Cnl::cb_FPS(uint32_t _channelID, int vFps, int aFPS)
{
	printf("*");
}

int Cnl::start()
{
	return m_provider.startCapture();
}

int Cnl::stop()
{
	m_consumer.stopChannel(m_cnlId);
	return m_provider.removeChannel(m_cnlId);
}

int main()
{
	CapturePoolMgr * pCapturePoolMgr = CapturePoolMgr::GetInstance();
	pCapturePoolMgr->initialize(Config->getVideoFormat(), 200, 200, false);

	const Card_Config& pCardConfig = Config->getCardConfig();
	uint32_t nCnl = 8;
	if (Config->is4K())
		nCnl = 1;

	Cnl * cnl = new Cnl[nCnl];

	for (int i = 0; i < nCnl; i++)
	{
		if (pCardConfig.recorder[CamID(i)].providerType == FrameProviderType::FPT_YUV_FILE)
		{
			char NDI_name[50];
			sprintf_s(NDI_name, "Cam%c", 'A' + i);
			cnl[i].init(i, pCardConfig.recorder[CamID(i)].szAudioName, pCardConfig.recorder[CamID(i)].szItemName, NDI_name);
		}
	}

	for (int i = 0; i < nCnl; i++)
		cnl[i].start();

	getchar();

	for (int i = 0; i < nCnl; i++)
		cnl[i].stop();

	delete[]cnl;
	return 0;
}