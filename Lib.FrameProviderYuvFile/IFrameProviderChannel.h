#pragma once
#ifndef _MSC_VER
#define MAX_PATH 260
#endif 
#include "../Lib.Base/VideoFormatDef.h"
#include "IFPInputCallBack.h"


struct sFrameProvider_Parameter
{
	FPTVideoFormat fpVideoFormat;
	bool bAutoDetect;
	uint32_t dwBoardID;
	uint32_t dwSdiID;
	char szFileName[MAX_PATH];
	char szFileNameAudio[MAX_PATH];
	sFrameProvider_Parameter()
	{
		memset(this, 0, sizeof(*this));
	}

	sFrameProvider_Parameter& operator =(const sFrameProvider_Parameter& rhs)
	{
		if (this != &rhs)
		{
			fpVideoFormat = rhs.fpVideoFormat;
			bAutoDetect = rhs.bAutoDetect;
			dwBoardID = rhs.dwBoardID;
			dwSdiID = rhs.dwSdiID;
#ifdef _MSC_VER
			memcpy_s(szFileName, MAX_PATH, rhs.szFileName, MAX_PATH);
			memcpy_s(szFileNameAudio, MAX_PATH, rhs.szFileNameAudio, MAX_PATH);
#else
			memcpy(szFileName, rhs.szFileName, MAX_PATH);
			memcpy(szFileNameAudio, rhs.szFileNameAudio, MAX_PATH);
#endif _MSC_VER

		}
		return *this;
	}
};

class IFrameProviderChannel
{
public:
	virtual ~IFrameProviderChannel() = default;

	virtual int				addChannel(uint32_t dwCnlID, const sFrameProvider_Parameter& pCnlParameter, IFPInputCallBack* _pGetFrameCB) = 0;
	virtual int				startCapture() = 0;
	virtual int				removeChannel(uint32_t dwCnlID) = 0;
	virtual void			frameConsumed() = 0;
};
