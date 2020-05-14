#include "FrameProviderYuvFile.h"
#include "../Lib.Base/CapturePoolMgr.h"
#include "../Lib.Base/platform.h"
#include <Shlwapi.h>
#include "MarkerBuilder.h"
#include "../Lib.Config/IConfig.h"
#include "../Lib.Config/AudioSampleHeader.h"

// ffmpeg - i source_file -vcodec rawvideo -pix_fmt uyvy422 -t 60 -vf scale=1920:1080 out.YUV


CFrameProviderYuvFile::CFrameProviderYuvFile()
{
	m_maxAudioSample = 0;
	m_initDone = false;
	m_hTimerRender = nullptr;
	m_pGetFrameCB = nullptr;
	m_dwCnlID = 0xFFFFFFFF;
	m_frameConsumed = 1;
	if (Config->isBuildMarker())
		m_pBuildMarker = new Build_Marker(Config->getVideoWidth(), Config->getVideoHeight());
}

CFrameProviderYuvFile::~CFrameProviderYuvFile()
{
	closeChannel();
	CloseVideoFile();
	if (m_pBuildMarker)
		delete m_pBuildMarker;
}

int CFrameProviderYuvFile::initAudio(const sFrameProvider_Parameter& pCnlParameter)
{
	char szAudioName[MAX_PATH];
	memcpy_s(szAudioName, MAX_PATH, pCnlParameter.szFileNameAudio, MAX_PATH);
	PathRenameExtensionA(szAudioName, ".wav");
	m_audioReader.open(szAudioName, m_szLogFile);

	m_maxAudioSample = 0;
	m_dwCycleValue = FFMPEG_AUDIOSAMPLE::getFrameSamples(m_stCnlParameter.fpVideoFormat, m_maxAudioSample);

	if (m_maxAudioSample == 0)
	{
		printf("CFrameProviderYuvFile::addChannel Failed.Format %s Not support.\n",
			Config->getVideoFormatString().c_str());
		return -3;
	}

	return 0;
}

int CFrameProviderYuvFile::initVideo(const sFrameProvider_Parameter& pCnlParameter)
{
	if (fopen_s(&m_fpVideo, pCnlParameter.szFileName, "rb") != 0)
	{
		printf("CFrameProviderYuvFile::addChannel Failed.Open video file %s failed.", pCnlParameter.szFileName);
		return -1;
	}

	unsigned char id[8];
	fread(id, 8, 1, m_fpVideo);

	if ((id[0] == 'I') && (id[1] == 'D') && (id[2] == '4') && (id[3] == '.'))
	{
		printf("CFrameProviderYuvFile::addChannel Failed.Not support OLD yuv files.");
		CloseVideoFile();
		return -1;
	}
	_fseeki64(m_fpVideo, 0, SEEK_SET);

	return 0;
}

int CFrameProviderYuvFile::addChannel(uint32_t dwCnlID, const sFrameProvider_Parameter& pCnlParameter, IFPInputCallBack* _pGetFrameCB)
{
	m_stCnlParameter = pCnlParameter;
	m_pGetFrameCB = _pGetFrameCB;
	m_dwCnlID = dwCnlID;
	swprintf_s(m_szLogFile, _T("C:\\Logs\\frame2TCP\\ProviderYuvFile_%d.Log"), dwCnlID);

	if (initVideo(pCnlParameter) != 0)
		return -1;

	initAudio(pCnlParameter);

	m_dwTimes = 0;
	m_threadHandle = async_thread(thread_priority::normal, &CFrameProviderYuvFile::SendOneVideoFrm, this);
	m_initDone = true;
	startThread();
	return 0;
}

int CFrameProviderYuvFile::removeChannel(uint32_t)
{
	closeChannel();
	return 0;
}

void CFrameProviderYuvFile::frameConsumed()
{
	++m_frameConsumed;
}

int CFrameProviderYuvFile::startCapture()
{
	return 0;
}

void CFrameProviderYuvFile::callBack()
{
	while (isRunning())
	{
		if (m_queueFrame.size() < 10)//because we only have 15 for each cam
		{
			if (m_queueFrame.size() < 5)
				printf("Low level %d for %d \n", m_queueFrame.size(), m_dwCnlID);

			pVFrame pFrame = nullptr;
			pAframe aFrame = nullptr;

			if (buildFrame(pFrame, aFrame) == 0)
			{
				m_locker.lock();
				m_queueFrame.push(pFrame);
				m_queueAudio.push(aFrame);
				m_locker.unlock();
			}
			Sleep(1);
		}
		else
			Sleep(5);
	}
}

void CFrameProviderYuvFile::CloseVideoFile()
{
	if (m_fpVideo != nullptr)
	{
		fclose(m_fpVideo);
		m_fpVideo = nullptr;
	}
}

int CFrameProviderYuvFile::buildFrame(pVFrame& _uncompFrame, pAframe& _aFrame)
{
	if (!m_initDone)
		return -1;

	_uncompFrame = nullptr;
	_aFrame = nullptr;

	CapturePoolMgr::GetInstance()->getNew(_uncompFrame);
	CapturePoolMgr::GetInstance()->getNew(_aFrame);

	if (!_uncompFrame || !_aFrame)
		return -2;

	loadAudioFrameFromDisk(_aFrame);
	int nRet = loadVideoFrameFromDisk(_uncompFrame);

	++m_dwTimes;

	return nRet;
}

int	CFrameProviderYuvFile::loadAudioFrameFromDisk(pAframe& _aFrame)
{
	int nCurSampleCnt = m_dwCycleValue[m_dwTimes % m_maxAudioSample];
	_aFrame->SetSampleCnt(nCurSampleCnt);
	_aFrame->SetMonoCnt(16);
	m_audioReader.getAudioFrame(_aFrame);

	_aFrame->SetFrameID(m_dwTimes);

	return 0;
}

int	CFrameProviderYuvFile::loadVideoFrameFromDisk(pVFrame& _uncompFrame)
{
	cpuFreq freq;
	getCPUfreq(freq);

	cpuTick tickNow = 0;
	getTickCount(tickNow);

	_uncompFrame->setFrameTime(tickNow / (freq / 1000));

	if (_uncompFrame->loadFromFile(m_fpVideo) != 0)
	{
		printf("Failed to load frame. Loop\n");

		_fseeki64(m_fpVideo, 0, SEEK_SET);	// try again from start

		if (_uncompFrame->loadFromFile(m_fpVideo) != 0)
		{
			_uncompFrame->setToBlack();
			return -1;
		}
	}

	_uncompFrame->SetFrameID(m_dwTimes);

	if (m_pBuildMarker)
		m_pBuildMarker->buildMarker((byte*)_uncompFrame->getBuffer(), m_dwTimes, m_dwCnlID);

	return 0;
}

void CFrameProviderYuvFile::closeChannel()
{
	stopThread(2000);

	//if (m_threadHandle.valid())
	//	m_threadHandle.get();
	if (m_fpVideo != nullptr)
		fclose(m_fpVideo);
	m_initDone = false;
	m_pGetFrameCB = nullptr;
	m_dwCnlID = 0xFFFFFFFF;
}

VOID CALLBACK CFrameProviderYuvFile::SendVideoCB(_In_ PVOID lpParameter, _In_ BOOLEAN)
{
	CFrameProviderYuvFile* pthis = (CFrameProviderYuvFile*)lpParameter;
	pthis->SendOneVideoFrm();
}

void CFrameProviderYuvFile::SendOneVideoFrm()
{
	while (isRunning())
	{
		if (m_frameConsumed)
		{
			pVFrame _uncompFrame = nullptr;
			pAframe _aFrame = nullptr;

			m_locker.lock();
			if (!m_queueFrame.empty() && !m_queueAudio.empty())
			{
				_uncompFrame = m_queueFrame.front();
				_aFrame = m_queueAudio.front();
				m_queueFrame.pop();
				m_queueAudio.pop();
			}
			m_locker.unlock();

			if (m_pGetFrameCB != nullptr && _uncompFrame != nullptr && _aFrame != nullptr)
			{
				--m_frameConsumed;
				m_pGetFrameCB->cb(m_dwCnlID, _uncompFrame, nullptr, nullptr, _aFrame);
			}
			else
				Sleep(5);
		}
		else
			Sleep(5);
	}
}