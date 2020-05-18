#pragma once
#include "../Lib.Base/videoFrame.h"
#include "../Lib.Base/audioFrame.h"
#include "../Lib.Base/compVideoFrame.h"

class IFPInputCallBack
{
public:
	virtual void cb(uint32_t _channelID, pVFrame pFrameVideo = nullptr, pCVframe pFrameVideo960 = nullptr, pCVframe pFrameVideo480 = nullptr, pAframe pFrameAudio = nullptr) = 0;
	virtual void cb_FPS(uint32_t _channelID, int vFps = 0, int aFPS = 0) = 0;
};
