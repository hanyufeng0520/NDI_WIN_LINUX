#pragma once
#include "../Lib.Base/audioFrame.h"
#include "../Lib.Base/videoFrame.h"
#include <cstdint>

enum FrameConsumerType
{
	FCT_LOCAL,
	FCT_AJA,
	FCT_BLACKMAGIC,
	FCT_DELTACAST,
	FCT_STREAM,
	FCT_NDI,
};

struct sFrameConsumer_Parameter
{
	FrameConsumerType	fcType;
	FPTVideoFormat		fpVideoFormat;
	uint32_t			dwBoardID;
	uint32_t			dwSdiID;
	const char* name;

	sFrameConsumer_Parameter()
	{
		memset(this, 0, sizeof(*this));
	}

	sFrameConsumer_Parameter& operator =(const sFrameConsumer_Parameter& rhs)
	{
		if (this != &rhs)
		{
			fcType = rhs.fcType;
			fpVideoFormat = rhs.fpVideoFormat;
			dwBoardID = rhs.dwBoardID;
			dwSdiID = rhs.dwSdiID;
			name = rhs.name;
		}
		return *this;
	}
};

class IFrameConsumerChannel
{
public:
	virtual ~IFrameConsumerChannel() = default;
	virtual int				addChannel(uint32_t dwCnlID, const sFrameConsumer_Parameter& pCnlParameter) = 0;
	virtual int				OutputFrames(uint32_t nChannelID, pVFrame pVideo, pAframe pAudio, Timecode *tc, uint32_t &dwFramesDropped) = 0;
	virtual int				stopChannel(uint32_t nChannelID) = 0;
};
