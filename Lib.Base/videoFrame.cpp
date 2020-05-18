#include<string.h>
#include "videoFrame.h"

VideoFrame::VideoFrame()
{
}

VideoFrame::~VideoFrame()
{
	if (m_pBuffer)
		free(m_pBuffer);
}

int VideoFrame::setBufferSize(uint32_t _size)
{
	if (_size > m_totalSize)
	{
		if (m_pBuffer != nullptr)
			free(m_pBuffer);
		//https://msdn.microsoft.com/en-us/library/ycsb6wwf.aspx
		//In Visual C++, the fundamental alignment is the alignment that's required for a double, or 8 bytes. In code that targets 64-bit platforms, it¡¯s 16 bytes
		m_pBuffer = static_cast <unsigned char *> (malloc(_size));
		m_totalSize = _size;
	}
	m_usedSize = _size;
	return 0;
}

int VideoFrame::setToBlack(bool _flagOnly)
{
	if (!_flagOnly)
	{
		switch (m_colorSpace)
		{
		case VideoColorSpace::CC_422_UYVY:
		{
			uint32_t size = getBufferSize() / 4;
			uint32_t* dest = (uint32_t*)getBuffer();
			for (uint32_t i = 0; i < size; i++)
				*dest++ = 0x10801080;
			break;
		}
		case VideoColorSpace::CC_422_YUV:
		{
			memset(getBuffer(), 0x10, getBufferSize() / 2);
			memset(getBuffer() + (getBufferSize() / 2), 0x80, getBufferSize() / 2);
			break;
		}
		case VideoColorSpace::CC_420_NV12: break;
		case VideoColorSpace::CC_444_ARGB: break;
		default: break;
		}
	}
	m_isBlackFrame = true;
	return 0;
}

int	VideoFrame::saveToFile(const char* _fileName) const
{
	FILE* fp = nullptr;
#ifdef _MSC_VER
	fopen_s(&fp, _fileName, "wb");
#else
	#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif _MSC_VER
	fopen_s(&fp, _fileName, "wb");
	if (fp == nullptr)
		return -1;

	fwrite(m_pBuffer, m_usedSize, 1, fp);

	fclose(fp);
	return 0;
}

int VideoFrame::loadFromFile(FILE* _fp) const
{
	if (fread(m_pBuffer, m_usedSize, 1, _fp) != 1)
		return -2;
	return 0;
}

int	VideoFrame::loadFromFile(const char* _fileName) const
{
	FILE* fp = nullptr;

	fopen_s(&fp, _fileName, "rb");
	if (fp == nullptr)
		return -1;

	int ret = loadFromFile(fp);

	fclose(fp);
	return ret;
}

bool VideoFrame::compareWith(std::shared_ptr<VideoFrame> _frame) const
{
	if (m_usedSize != _frame->m_usedSize)
		return false;
	if (memcmp(m_pBuffer, _frame->m_pBuffer, m_usedSize) == 0)
		return true;
	return false;
}

int	VideoFrame::init(FPTVideoFormat _videoFormat, VideoColorSpace _colorSpace)
{
	const int height = videoFormatToHeight(_videoFormat);
	switch (height)
	{
	case 270:	return init(VideoFrameSize::FS_270, _colorSpace);
	case 540:	return init(VideoFrameSize::FS_540, _colorSpace);
	case 720:	return init(VideoFrameSize::FS_720, _colorSpace);
	case 1080:
	{
		if (isVideoFormatInterlaced(_videoFormat))
			return init(VideoFrameSize::FS_1080i, _colorSpace);
		return init(VideoFrameSize::FS_1080p, _colorSpace);
	}
	case 2160:	return init(VideoFrameSize::FS_4K, _colorSpace);
	case 4320:	return init(VideoFrameSize::FS_8K, _colorSpace);
	default:	return -1;
	}
}

int VideoFrame::init(VideoFrameSize _frameSize, VideoColorSpace _colorSpace)
{
	switch (_frameSize)
	{
	case VideoFrameSize::FS_270:	m_width = 480;  m_height = 270;  m_isInterlaced = false; break;
	case VideoFrameSize::FS_540:	m_width = 960;  m_height = 540;  m_isInterlaced = false; break;
	case VideoFrameSize::FS_720:	m_width = 1280; m_height = 720;  m_isInterlaced = false; break;
	case VideoFrameSize::FS_1080i:	m_width = 1920; m_height = 1080; m_isInterlaced = true;  break;
	case VideoFrameSize::FS_1080p:	m_width = 1920; m_height = 1080; m_isInterlaced = false; break;
	case VideoFrameSize::FS_4K:		m_width = 3840; m_height = 2160; m_isInterlaced = false; break;
	case VideoFrameSize::FS_8K:		m_width = 7680; m_height = 4320; m_isInterlaced = false; break;
	default: break;
	}

	m_colorSpace = _colorSpace;

	switch (m_colorSpace)
	{
	case VideoColorSpace::CC_422_UYVY: setBufferSize(m_width * m_height * 2); break;
	case VideoColorSpace::CC_422_YUV:  setBufferSize(m_width * m_height * 2); break;
	case VideoColorSpace::CC_420_I420: 
	case VideoColorSpace::CC_420_NV12: setBufferSize(m_width * m_height * 3 / 2); break;
	case VideoColorSpace::CC_444_ARGB: setBufferSize(m_width * m_height * 4); break;
	case VideoColorSpace::CC_422_UYVY_10bits: setBufferSize((m_width / 48 + (m_width % 48 ? 1 : 0)) * 128 * m_height); break;		
	case VideoColorSpace::CC_422_YUV_16bits: setBufferSize(m_width * m_height * 4); break;
	default: break;
	}
	return 0;
}

bool isVideoFormatInterlaced(FPTVideoFormat _fv)
{
	switch (_fv)
	{
	case FPTVideoFormat::FP_FORMAT_1080i_5000:
	case FPTVideoFormat::FP_FORMAT_1080i_5994:
	case FPTVideoFormat::FP_FORMAT_1080i_6000: return true;
	default: return false;
	}
}

int videoFormatToHeight(FPTVideoFormat _fv)
{
	switch (_fv)
	{
	case FPTVideoFormat::FP_FORMAT_1080i_5000:
	case FPTVideoFormat::FP_FORMAT_1080i_5994:
	case FPTVideoFormat::FP_FORMAT_1080i_6000:
	case FPTVideoFormat::FP_FORMAT_1080p_2500:
	case FPTVideoFormat::FP_FORMAT_1080p_2997:
	case FPTVideoFormat::FP_FORMAT_1080p_3000:
	case FPTVideoFormat::FP_FORMAT_1080p_5000:
	case FPTVideoFormat::FP_FORMAT_1080p_5994:
	case FPTVideoFormat::FP_FORMAT_1080p_6000:
	case FPTVideoFormat::FP_FORMAT_1080p_2400:
	case FPTVideoFormat::FP_FORMAT_1080p_2398: return 1080;
	case FPTVideoFormat::FP_FORMAT_720p_2500:
	case FPTVideoFormat::FP_FORMAT_720p_2997:
	case FPTVideoFormat::FP_FORMAT_720p_5000:
	case FPTVideoFormat::FP_FORMAT_720p_5994:
	case FPTVideoFormat::FP_FORMAT_720p_6000: return 720;
	case FPTVideoFormat::FP_FORMAT_4Kp_2500:
	case FPTVideoFormat::FP_FORMAT_4Kp_2997:
	case FPTVideoFormat::FP_FORMAT_4Kp_3000:
	case FPTVideoFormat::FP_FORMAT_4Kp_5000:
	case FPTVideoFormat::FP_FORMAT_4Kp_5994:
	case FPTVideoFormat::FP_FORMAT_4Kp_6000:
	case FPTVideoFormat::FP_FORMAT_4Kp_2400: return 2160;
	case FPTVideoFormat::FP_FORMAT_8kp_2400:
	case FPTVideoFormat::FP_FORMAT_8kp_2500:
	case FPTVideoFormat::FP_FORMAT_8kp_2997:
	case FPTVideoFormat::FP_FORMAT_8kp_3000:
	case FPTVideoFormat::FP_FORMAT_8kp_5000:
	case FPTVideoFormat::FP_FORMAT_8kp_5994:
	case FPTVideoFormat::FP_FORMAT_8kp_6000: return 4320;
	default: return 0;
	}
}