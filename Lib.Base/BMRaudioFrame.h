#pragma once
#include <cstdint>
#include <memory>

class BMRaudioFrame
{
	short*          m_raw;
	uint64_t        m_nFrameID;
	unsigned long   m_totalSize;
	unsigned long   m_dataSize;
	uint32_t		m_nTimeStamp;

	void reAlloc(uint32_t size);

public:
	BMRaudioFrame();
	~BMRaudioFrame();

	short*			getRaw() const;
	void			setToMute() const;
	void			SetFrameID(uint64_t  nFrameID);
	uint64_t		GetFrameID() const;
	unsigned long	getBufferTotalSize() const;
	bool			isMute() const;
	unsigned long	getDataSize() const;
	void			setBufferSize(uint32_t size);
	void			setTimeStamp(uint32_t lTimeStamp);
	uint32_t		getTimeStamp() const;
};

using pBMRAframe = std::shared_ptr<BMRaudioFrame >;
