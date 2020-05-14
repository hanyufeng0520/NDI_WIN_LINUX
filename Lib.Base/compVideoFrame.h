#pragma once
#include <cstdint>
#include "timecode.h"
#include <memory>
#include "IBaseFrame.h"

//|-----------|
//|IDR 1 uint8_t |
//|-----------|
//|TC 4 uint8_t  |
//-------------
//|size 4 uint8_t|   <------ this rawSize is the rawSize of "Data" below
//|-----------|
//|           |
//|	Data      |
//|			  |
//|			  |
//|			  |
//|-----------|
class CompVideoFrame :public IBaseFrame
{
	unsigned char* m_frame;
	uint32_t	   m_totalSize;
	double		   m_nTimeStamp;

public:
	struct header
	{
		//VideoANCDataStruct	ancData;
		bool				isIDRFrame;
		Timecode			tc;
		uint32_t			rawSize;
	};
	header* m_header = nullptr;
	CompVideoFrame();
	virtual ~CompVideoFrame();

	unsigned char* getRaw() const override;
	unsigned char* getRawWithSize() const;
	unsigned char* getRawWithHeader() const;
	unsigned long  getRawWithHeaderSize() const;

	int			  getTotalSize() const override;
	int			  setBufferSize(unsigned long _rawSize)override;
	unsigned long getRawSize() const;

	void     setTC(const Timecode& _tc)override;
	Timecode getTC() const override;

	void   setTimeStamp(double lTimeStamp);
	double getTimeStamp() const;

	void setIsIDR(bool bIsIDRFrame) const;
	bool getIsIDR() const;

	/*void setAncFieldInfo(VideoANCDataStruct & ancInfo);
	void getAncFieldInfo(VideoANCDataStruct &ancData) const;
	void setAncFieldInfo(uint8_t *srcFieldDataEven, uint32_t dwEvenSize, uint8_t *srcOddFieldData, uint32_t dwOddSize) const;
	void getAncFieldInfo(uint8_t *srcFieldDataEven, uint32_t &dwEvenSize, uint8_t *srcOddFieldData, uint32_t &dwOddSize) const;*/

	unsigned long getDataSize() const override;
	void copyRawData(CompVideoFrame*pSrc)
	{
		this->setBufferSize(pSrc->getRawSize());
		memcpy(this->getRaw(), pSrc->getRaw(), pSrc->getRawSize());
	}
	int	saveToFile(const char* _fileName) const;
};

using pCVframe = std::shared_ptr<CompVideoFrame>;
