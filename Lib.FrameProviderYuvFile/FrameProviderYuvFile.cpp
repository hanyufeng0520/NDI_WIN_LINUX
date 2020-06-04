#include "FrameProviderYuvFile.h"
#include "../Lib.Base/CapturePoolMgr.h"
#include "../Lib.Base/platform.h"
#include "../Lib.Config/IConfig.h"
#include "../Lib.Base/AudioSampleHeader.h"
#ifndef _MSC_VER
#include <unistd.h>
#else
#include "../Lib.Logger/LogWriter.h"
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
#ifdef _MSC_VER
		WriteLogA(m_szLogFile, LOGLEVEL::Error, "CFrameProviderYuvFile::initVideo Failed.Open video file %s failed.", pCnlParameter.szFileName);
#endif
		printf("CFrameProviderYuvFile::initVideo Failed.Open video file %s failed.", pCnlParameter.szFileName);
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
	WriteLogA(m_szLogFile, LOGLEVEL::Info, "CFrameProviderYuvFile::initVideo Okay.Open video file %s .", pCnlParameter.szFileName);
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
	swprintf_s(m_szLogFile, MAX_PATH, L"C:\\Logs\\test_NDI\\ProviderYuvFile_%d.Log", dwCnlID);
#else 
	//
#endif
	if (initVideo(pCnlParameter) != 0)
	{
		printf("initVideo failed\n ");
#ifdef _MSC_VER
		WriteLogA(m_szLogFile, LOGLEVEL::Warn, "addChannel initVideo failed");
#endif
		return -1;
	}	

	initAudio(pCnlParameter);

	m_bStop = false;
	m_dwTimes = 0;
	m_threadReadCallBack = async_thread(thread_priority::normal, &CFrameProviderYuvFile::readThread, this);
	printf("CFrameProviderYuvFile::addChannel Okay\n ");
#ifdef _MSC_VER
	WriteLogA(m_szLogFile, LOGLEVEL::Info, "addChannel Okay");
#endif
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
	if (!m_initDone)
		return -1;
	m_threadSendFrames = async_thread(thread_priority::normal, &CFrameProviderYuvFile::SendOneVideoFrm, this);
#ifdef _MSC_VER
	WriteLogA(m_szLogFile, LOGLEVEL::Info, "startCapture Okay");
#endif
	return 0;
}

void CFrameProviderYuvFile::readThread()
{
	while (!m_bStop)
	{
		if (m_queueFrame.size() < 15)//because we only have 15 for each cam
		{
			if (m_queueFrame.size() < 5 || m_queueAudio.size() < 5)
#ifdef _MSC_VER
				WriteLogW(m_szLogFile, LOGLEVEL::Warn, L"readThread dwCnlID(%d) now Low level videoLIST %d audioList %d", m_dwCnlID, m_queueFrame.size(), m_queueAudio.size());
#else
				printf("readThread dwCnlID(%d) now Low level videoLIST %d audioList %d \n", m_dwCnlID, m_queueFrame.size(), m_queueAudio.size());
#endif


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
			usleep(1000);
#endif 		
		}

		else
#ifdef _MSC_VER
			Sleep(5);
#else
			usleep(5000);
#endif 
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
#ifdef _MSC_VER
		WriteLogW(m_szLogFile, LOGLEVEL::Warn, L"buildFrame dwCnlID(%d) getNew failed videoLIST %d audioList %d", m_dwCnlID, m_queueFrame.size(), m_queueAudio.size());
#else
		printf("buildFrame dwCnlID(%d) getNew failed videoLIST %d audioList %d \n", m_dwCnlID, m_queueFrame.size(), m_queueAudio.size());
#endif
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
	m_audioReader.getAudioFrame(_aFrame);
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
	if (m_threadReadCallBack.valid())
		m_threadReadCallBack.get();
	if (m_threadSendFrames.valid())
		m_threadSendFrames.get();
	if (m_fpVideo != nullptr)
		fclose(m_fpVideo);
	m_initDone = false;
	m_pGetFrameCB = nullptr;
	m_dwCnlID = 0xFFFFFFFF;
}

void CFrameProviderYuvFile::SendOneVideoFrm()
{
	uint64_t  sendLoop = 0;
	uint64_t  novCount = 0;
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
			else
			{
#ifdef _MSC_VER
				WriteLogW(m_szLogFile, LOGLEVEL::Warn, L"SendOneVideoFrm dwCnlID(%d) sendLoop(%I64d) novCount(%I64d) NO data videoLIST %d audioList %d", 
					m_dwCnlID, sendLoop,++novCount, m_queueFrame.size(), m_queueAudio.size());
				printf("SendOneVideoFrm dwCnlID(%d) sendLoop(%I64d) novCount(%I64d) NO data videoLIST %d audioList %d \n",
					m_dwCnlID, sendLoop, ++novCount, m_queueFrame.size(), m_queueAudio.size());

#else
				printf("SendOneVideoFrm dwCnlID(%d) sendLoop(%llu) novCount(%llu) NO data videoLIST %d audioList %d \n",
					m_dwCnlID, sendLoop, ++novCount, m_queueFrame.size(), m_queueAudio.size());
#endif
			}
			m_locker.unlock();

			if (m_pGetFrameCB != nullptr && _uncompFrame != nullptr && _aFrame != nullptr)
			{
				--m_frameConsumed;
				m_pGetFrameCB->cb(m_dwCnlID, _uncompFrame, nullptr, nullptr, _aFrame);
			}
#ifdef _MSC_VER
			Sleep(5);
#else
			usleep(5000);
#endif 
			if (sendLoop++ %1500 == 0)
			{
#ifdef _MSC_VER
				WriteLogW(m_szLogFile, LOGLEVEL::Info, L"SendOneVideoFrm dwCnlID(%d) sendLoop(%I64d) novCount(%I64d)", m_dwCnlID, sendLoop, novCount);
				printf("SendOneVideoFrm dwCnlID(%d) sendLoop(%I64d) novCount(%I64d)\n", m_dwCnlID, sendLoop, novCount);

#else
				printf("SendOneVideoFrm dwCnlID(%d) sendLoop(%llu) novCount(%llu)\n", m_dwCnlID, sendLoop, novCount);
#endif
			}
		}
		else
#ifdef _MSC_VER
			Sleep(5);
#else
			usleep(5000);
#endif 
	}
}