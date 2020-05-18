#include "FrameProviderYuvFile.h"
#include "../Lib.Base/CapturePoolMgr.h"
#include "../Lib.Base/platform.h"
#include "../Lib.Config/IConfig.h"
#include "../Lib.Base/AudioSampleHeader.h"
#ifndef _MSC_VER
#include <unistd.h>
#endif 

// ffmpeg - i source_file -vcodec rawvideo -pix_fmt uyvy422 -t 60 -vf scale=1920:1080 out.YUV


CFrameProviderYuvFile::CFrameProviderYuvFile()
{
	m_maxAudioSample = 0;
	m_initDone = false;
	m_pGetFrameCB = nullptr;
	m_dwCnlID = 0xFFFFFFFF;
	m_frameConsumed = 1;
}

CFrameProviderYuvFile::~CFrameProviderYuvFile()
{
	closeChannel();
	CloseVideoFile();
}

int CFrameProviderYuvFile::initAudio(const sFrameProvider_Parameter& pCnlParameter)
{
	char szAudioName[MAX_PATH];
#ifdef _MSC_VER
	memcpy_s(szAudioName, MAX_PATH, pCnlParameter.szFileNameAudio, MAX_PATH);
#else
	memcpy(szAudioName, pCnlParameter.szFileNameAudio, MAX_PATH);
#endif _MSC_VER
	
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
#ifndef _MSC_VER
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif 
	fopen_s(&m_fpVideo, pCnlParameter.szFileName, "rb");
	if (m_fpVideo == nullptr)
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
#ifdef _MSC_VER
	_fseeki64(m_fpVideo, 0, SEEK_SET);
#else
	fseeko64(m_fpVideo, 0, SEEK_SET);
#endif 	

	return 0;
}

int CFrameProviderYuvFile::addChannel(uint32_t dwCnlID, const sFrameProvider_Parameter& pCnlParameter, IFPInputCallBack* _pGetFrameCB)
{
	m_stCnlParameter = pCnlParameter;
	m_pGetFrameCB = _pGetFrameCB;
	m_dwCnlID = dwCnlID;
#ifdef _MSC_VER
	swprintf_s(m_szLogFile, MAX_PATH, L"C:\\Logs\\frame2TCP\\ProviderYuvFile_%d.Log", dwCnlID);
#else 
	//
#endif
	if (initVideo(pCnlParameter) != 0)
	{
		printf("initVideo failed\n ");
		return -1;
	}	

	initAudio(pCnlParameter);

	m_bStop = false;
	m_dwTimes = 0;
	m_threadHandle = async_thread(thread_priority::normal, &CFrameProviderYuvFile::SendOneVideoFrm, this);
	m_threadCallBack = async_thread(thread_priority::normal, &CFrameProviderYuvFile::callBack, this);
	printf("CFrameProviderYuvFile::addChannel Okay\n ");
	m_initDone = true;
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
	int loop = 0;
	while (!m_bStop)
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
#ifdef _MSC_VER
			Sleep(1);
#else
			sleep(1);
#endif 		
		}

		else
#ifdef _MSC_VER
			Sleep(5);
#else
			sleep(5);
#endif 
		++loop;
		printf("loop:%d \n", loop);
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

	CapturePoolMgr::GetInstance()->getNew(_uncompFrame);
	CapturePoolMgr::GetInstance()->getNew(_aFrame);

	if (!_uncompFrame || !_aFrame)
	{
		printf("buildFrame getNew failed--->\n");
		return -2;
	}
	
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
	m_audioReader.getAudioFrame(_aFrame);//JKL_NEEDTODO
	_aFrame->SetFrameID(m_dwTimes);

	return 0;
}

int	CFrameProviderYuvFile::loadVideoFrameFromDisk(pVFrame& _uncompFrame)
{
#ifdef _MSC_VER
	cpuFreq freq;
	getCPUfreq(freq);

	cpuTick tickNow = 0;
	getTickCount(tickNow);

	_uncompFrame->setFrameTime(tickNow / (freq / 1000));
#else
#endif
	if (_uncompFrame->loadFromFile(m_fpVideo) != 0)
	{
		printf("Failed to load frame. Loop\n");

#ifdef _MSC_VER
		_fseeki64(m_fpVideo, 0, SEEK_SET);
#else
		fseeko64(m_fpVideo, 0, SEEK_SET);
#endif 
		if (_uncompFrame->loadFromFile(m_fpVideo) != 0)
		{
			_uncompFrame->setToBlack();
			return -1;
		}
	}

	_uncompFrame->SetFrameID(m_dwTimes);
	return 0;
}

void CFrameProviderYuvFile::closeChannel()
{
	m_bStop = true;
	if (m_threadCallBack.valid())
		m_threadCallBack.get();
	if (m_threadHandle.valid())
		m_threadHandle.get();
	if (m_fpVideo != nullptr)
		fclose(m_fpVideo);
	m_initDone = false;
	m_pGetFrameCB = nullptr;
	m_dwCnlID = 0xFFFFFFFF;
}

void CFrameProviderYuvFile::SendOneVideoFrm()
{
	while (!m_bStop)
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
				printf("cb--->\n");
				m_pGetFrameCB->cb(m_dwCnlID, _uncompFrame, nullptr, nullptr, _aFrame);
			}
#ifdef _MSC_VER
			Sleep(5);
#else
			sleep(5);
#endif 
		}
		else
#ifdef _MSC_VER
			Sleep(5);
#else
			sleep(5);
#endif 
	}
}