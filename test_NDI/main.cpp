#include <cstdint>
#include "../Lib.FrameProviderYuvFile/FrameProviderYuvFile.h"
#include "../Lib.FrameConsumerNDI/FrameConsumerNDI.h"
#include "../Lib.Base/CapturePoolMgr.h"
#include "../Lib.Config/IConfig.h"

#ifdef _MSC_VER
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swresample.lib")
#endif 

class Cnl : public IFPInputCallBack
{
	CFrameProviderYuvFile	*m_provider;
	CFrameConsumerNDI* m_consumer;
	int m_cnlId = -1;
	uint32_t				m_FramesDropped = 0;
	char					m_NDI_name[50];

public:
	Cnl();
	~Cnl();
	int init(int _cnlId, const char* _audioFileName, const char* _videoFileName, const char* _cnlName);
	int start();
	int stop();

	void cb(uint32_t _channelID, pVFrame pFrameVideo = nullptr, pCVframe pFrameVideo960 = nullptr, pCVframe pFrameVideo480 = nullptr, pAframe pFrameAudio = nullptr);
	void cb_FPS(uint32_t _channelID, int vFps = 0, int aFPS = 0);
};

Cnl::Cnl()
{
	m_provider = new CFrameProviderYuvFile;
	m_consumer = new CFrameConsumerNDI();
}
Cnl::~Cnl()
{
	delete m_consumer;
	delete m_provider;
}
int Cnl::init(int _cnlId, const char* _audioFileName, const char* _videoFileName, const char* _cnlName)
{
	printf("Init %s %s %s\n", _audioFileName, _videoFileName, _cnlName);
	sFrameProvider_Parameter paramProvid;
	paramProvid.fpVideoFormat = Config->getVideoFormat();
#ifdef _MSC_VER
	sprintf_s(paramProvid.szFileName, sizeof(paramProvid.szFileName), _videoFileName);
	sprintf_s(paramProvid.szFileNameAudio, sizeof(paramProvid.szFileNameAudio), _audioFileName);
	sprintf_s(m_NDI_name, sizeof(m_NDI_name), _cnlName);
#else
	sprintf(paramProvid.szFileName, _videoFileName);
	sprintf(paramProvid.szFileNameAudio, _audioFileName);
	sprintf(m_NDI_name, _cnlName);
#endif 

	m_cnlId = _cnlId;
	m_provider->addChannel(m_cnlId, paramProvid, this);
	sFrameConsumer_Parameter	paramConsum;
	paramConsum.fcType = FCT_NDI;
	paramConsum.fpVideoFormat = Config->getVideoFormat();
	paramConsum.name = _cnlName;
	m_consumer->addChannel(m_cnlId, paramConsum);

	return 0;
}

void Cnl::cb(uint32_t _channelID, pVFrame pFrameVideo, pCVframe pFrameVideo960, pCVframe pFrameVideo480, pAframe pFrameAudio)
{
	while (m_consumer->OutputFrames(_channelID, pFrameVideo, pFrameAudio, nullptr, m_FramesDropped))
#ifdef _MSC_VER
		Sleep(1);
#else
		usleep(1000);
#endif
	m_provider->frameConsumed();
}

void Cnl::cb_FPS(uint32_t _channelID, int vFps, int aFPS)
{
	printf("*");
}

int Cnl::start()
{
	return m_provider->startCapture();
}

int Cnl::stop()
{
	m_consumer->stopChannel(m_cnlId);
	return m_provider->removeChannel(m_cnlId);
}

int main()
{
	CapturePoolMgr* pCapturePoolMgr = CapturePoolMgr::GetInstance();
	pCapturePoolMgr->initialize(Config->getVideoFormat(), 200, 200, false);
	const Card_Config& pCardConfig = Config->getCardConfig();
	uint32_t nCnl = 8;
	if (Config->is4K())
		nCnl = 1;
	Cnl* cnl = new Cnl[nCnl];
	for (int i = 0; i < nCnl; i++)
	{
		if (pCardConfig.recorder[CamID(i)].providerType == FrameProviderType::FPT_YUV_FILE)
		{
			char NDI_name[50];
#ifdef _MSC_VER
			sprintf_s(NDI_name, sizeof(NDI_name), "Cam%c", 'A' + i);
#else
			sprintf(NDI_name, "Cam%c", 'A' + i);
#endif 
			cnl[i].init(i, pCardConfig.recorder[CamID(i)].szAudioName, pCardConfig.recorder[CamID(i)].szItemName, NDI_name);

			printf("index :%d name(%s) init done!\n", i, NDI_name);
		}
	}
#ifdef _MSC_VER
	Sleep(1);
#else
	usleep(1000);
#endif 	
	for (int i = 0; i < nCnl; i++)
		cnl[i].start();

	getchar();

	for (int i = 0; i < nCnl; i++)
		cnl[i].stop();

	delete[]cnl;
	return 0;
}