#pragma once
#include <cstdint>
#include <memory>
#include "timecode.h"

class IBaseFrame
{
protected:
	uint64_t       m_frameID = 0;

public:
	IBaseFrame() = default;
	virtual ~IBaseFrame() = default;

	virtual unsigned char*	getRaw() const = 0;
	virtual int				getTotalSize() const = 0;
	virtual int				setBufferSize(uint32_t _rawSize) = 0;
	virtual uint32_t	getDataSize() const = 0;
	virtual void			setTC(const Timecode& _tc) = 0;
	virtual Timecode		getTC() const = 0;

	void	 SetFrameID(uint64_t nFrameID) { m_frameID = nFrameID; }
	uint64_t GetFrameID() const { return m_frameID; }
};

using pSharedIBaseFrame = std::shared_ptr<IBaseFrame>;
