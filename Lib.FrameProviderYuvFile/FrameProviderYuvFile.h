#pragma once
#include <queue>
#include "../Lib.Base/locker.h"
#include "../Lib.Base/async_thread.h"
#include "IFrameProviderChannel.h"
#include "WaveReader.h"

class Build_Marker;
class CFrameProviderYuvFile :public IFrameProviderChannel
{
	int						 m_maxAudioSample = 0;
	sFrameProvider_Parameter m_stCnlParameter;
	FILE*                    m_fpVideo = nullptr;
	bool                     m_initDone;
	Locker                   m_locker;
	int32_t					 m_dwCnlID;
	IFPInputCallBack*        m_pGetFrameCB;
	std::queue<pVFrame>		 m_queueFrame;
	std::queue<pAframe>		 m_queueAudio;
	async_future             m_threadHandle;
	async_future             m_threadCallBack;
	CWaveReader				 m_audioReader;
	wchar_t					 m_szLogFile[MAX_PATH];
	std::atomic_long		 m_frameConsumed;
	const int*				 m_dwCycleValue;
	uint64_t				 m_dwTimes = 0;
	bool					 m_bStop = false;
	int		buildFrame(pVFrame& _uncompFrame, pAframe& _aFrame);
	int		loadAudioFrameFromDisk(pAframe& _aFrame);
	int		loadVideoFrameFromDisk(pVFrame& _uncompFrame);
	void	closeChannel();
	void	SendOneVideoFrm();
	void	callBack() ;
	void	CloseVideoFile();
	int		initAudio(const sFrameProvider_Parameter& pCnlParameter);
	int		initVideo(const sFrameProvider_Parameter& pCnlParameter);

public:
	CFrameProviderYuvFile();
	~CFrameProviderYuvFile();

	int		addChannel(uint32_t dwCnlID, const sFrameProvider_Parameter& pCnlParameter, IFPInputCallBack* _pGetFrameCB) override;
	int		removeChannel(uint32_t dwCnlID) override;
	int		startCapture() override;
	void	frameConsumed() override;
};
