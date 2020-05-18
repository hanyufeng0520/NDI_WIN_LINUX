#pragma once
#include <cstdint>
#include <memory>
#include "timecode.h"
#include <atomic>
#include "VideoFormatDef.h"

int videoFormatToHeight(FPTVideoFormat _fv);
bool isVideoFormatInterlaced(FPTVideoFormat _fv);

class VideoFrame
{
	uint64_t		     m_nFrameID = 0;
	Timecode	         m_tc;
	bool		         m_validTC = false;
	double				 m_pts = 0;
	int32_t				 m_totalSize = 0;
	int32_t				 m_usedSize = 0;
	uint64_t             m_frameTime = 0;
	unsigned char*		 m_pBuffer = nullptr;
	uint32_t			 m_width = 0;
	uint32_t			 m_height = 0;
	bool				 m_isInterlaced = false;
	bool			     m_isBlackFrame = false;
	VideoColorSpace		 m_colorSpace = VideoColorSpace::CC_422_UYVY;

	int					 setBufferSize(uint32_t _size);

public:
	VideoFrame();
	virtual ~VideoFrame();

	int						init(FPTVideoFormat _videoFormat, VideoColorSpace _colorSpace);
	int						init(VideoFrameSize _frameSize, VideoColorSpace _colorSpace);
	uint32_t			getWidth() const { return m_width; }
	uint32_t			getHeight() const { return m_height; }
	unsigned char*			getBuffer() const { return m_pBuffer; }
	uint32_t			getBufferSize() const { return m_usedSize; }
	bool					isInterlaced() const { return m_isInterlaced; }
	int						setToBlack(bool _flagOnly = false);
	void					resetBlackFlag() { m_isBlackFrame = false; }
	bool					isBlackFrame() const { return m_isBlackFrame; }
	void					SetFrameID(uint64_t  nFrameID) { m_nFrameID = nFrameID; }
	uint64_t				GetFrameID() const { return m_nFrameID; }
	void					setTC(const Timecode& _tc) { m_tc = _tc; }
	Timecode				getTC() const { return m_tc; }
	void					setTCValid(bool _valid) { m_validTC = _valid; }
	bool					getTCValid() const { return m_validTC; }
	void					setPts(double _pts) { m_pts = _pts; }
	double					getPts() const { return m_pts; }
	void					setFrameTime(uint64_t _frameTime) { m_frameTime = _frameTime; }
	uint64_t				getFrameTime() const { return m_frameTime; }

	int						loadFromFile(FILE * _fp) const;
	int						loadFromFile(const char* _fileName) const;
	int						saveToFile(const char* _fileName) const;
	bool					compareWith(std::shared_ptr<VideoFrame> _frame) const;
	VideoColorSpace			getColorSpace() const { return m_colorSpace; }
};

using pVFrame = std::shared_ptr<VideoFrame>;
