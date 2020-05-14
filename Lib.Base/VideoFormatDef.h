#pragma once

enum class FPTVideoFormat
{
	FP_FORMAT_UNKNOWN = 0,
	FP_FORMAT_1080i_5000,
	FP_FORMAT_1080i_5994,
	FP_FORMAT_1080i_6000,
	FP_FORMAT_1080p_2500,
	FP_FORMAT_1080p_2997,
	FP_FORMAT_1080p_3000,
	FP_FORMAT_1080p_5000,
	FP_FORMAT_1080p_5994,
	FP_FORMAT_1080p_6000,
	FP_FORMAT_720p_2500,
	FP_FORMAT_720p_2997,
	FP_FORMAT_720p_5000,
	FP_FORMAT_720p_5994,
	FP_FORMAT_720p_6000,
	FP_FORMAT_4Kp_2500,
	FP_FORMAT_4Kp_2997,
	FP_FORMAT_4Kp_3000,
	FP_FORMAT_4Kp_5000,
	FP_FORMAT_4Kp_5994,
	FP_FORMAT_4Kp_6000,
	FP_FORMAT_1080p_2400,
	FP_FORMAT_4Kp_2400,
	FP_FORMAT_1080p_2398,
	FP_FORMAT_SD_525_5994,
	FP_FORMAT_SD_625_5000,
	FP_FORMAT_8kp_2400,
	FP_FORMAT_8kp_2500,
	FP_FORMAT_8kp_2997,
	FP_FORMAT_8kp_3000,
	FP_FORMAT_8kp_5000,
	FP_FORMAT_8kp_5994,
	FP_FORMAT_8kp_6000,
	FP_FORMAT_MAX
};

enum class VideoFrameSize
{
	FS_270,
	FS_540,
	FS_720,
	FS_1080i,
	FS_1080p,
	FS_4K,
	FS_8K
};

enum class VideoColorSpace
{
	// Packed  YUV 4:2:2 8 bits
	//
	CC_422_UYVY,

	// Planar YUV 4:2:2 8 bits
	//
	CC_422_YUV,

	// Planar Y and Packed UV 4:2:0 8 bits
	// Related to I420, NV12 has one luma "luminance" plane Y and one plane with U and V values interleaved.
	// In NV12, chroma planes(blueand red) are subsampled in both the horizontaland vertical dimensions by a factor of 2.
	// For a 2X2 group of pixels, you have 4 Y samples and 1 U and 1 V sample.
	//
	// For 1 NV12 pixel: YYYYYYYY UVUV
	// For a 2-pixel NV12 frame: YYYYYYYYYYYYYYYY UVUVUVUV
	//
	CC_420_NV12,

	// Planar YUV 4:2:0 8 bits
	// It has the luma "luminance" plane Y first, then the U chroma plane and last the V chroma plane.
	// The two chroma planes(blueand red projections) are sub - sampled in both the horizontal and vertical dimensions by a factor of 2.
	// That is to say, for a 2X2 square of pixels, there are 4 Y samples but only 1 U sample and 1 V sample.
	// This format requires 4X8 + 8 + 8 = 48 bits per 4 pixels, so its depth is 12 bits per pixel.
	//
	// For 1 pixel: YYYYYYYY UUVV
	// For 2-pixel: YYYYYYYYYYYYYYYY UUUU VVVV
	//
	CC_420_I420,

	// Planar Alpha RGB 4:4:4 8 bits
	//
	CC_444_ARGB,

	// Planar YUV 4:2:2 16 bits
	//
	CC_422_YUV_16bits,

	// Packed YUV 4:2:2 10 bits
	// Also called V210. Data is stored in blocks of 32 bit values in little-endian.
	// Each such block contains 3 components, one each in bits 0 - 9, 10 - 19 and 20 - 29, the remaining two bits are unused.
	// We have alligned with block of 48 pixels (128 bytes). Padding is 0.
	CC_422_UYVY_10bits
};
