#pragma once
#include <vector>
#include <memory>
#include <string>

std::string		ipToString(uint32_t _ip);
std::wstring	ipToWString(uint32_t _ip);
std::string		GetAppPathA();
std::wstring	GetAppPathW();
bool			isValidString(const unsigned char* _str, int _len);
void			buildStrforGUID(char* _dst, int _dstSize, unsigned char* _src, int _srcSize);
std::string		getIDString(const char* id, unsigned long size);
std::wstring	getIDWString(const char* id, unsigned long size);
int				YUV444ToRGB888(unsigned char* src_buffer, int w, int h, unsigned char* des_buffer, bool bBGR /*true:BGR,false:RGB */);
int				RGB888ToYUV444(unsigned char* src_buffer, int w, int h, unsigned char* des_buffer, bool bBGR /*true:BGR,false:RGB */);
void			YUV2RGB(unsigned char* bufYUV, unsigned char* bufRGB, int nbElem);//UYVY2BGR24
std::pair<int, int>	ParseVersion(std::string versionString);
bool			checkStroageCompatibility(std::string _newV, std::string _oldV);
void			UYVY_To_YUV444(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height);
void			YUV422Planar_To_YUV444(unsigned char* _pDst, unsigned char* _pSrcY, unsigned char* _pSrcU, unsigned char* _pSrcV, int _width, int _height);
uint32_t		round(uint32_t _value, int _base = 4);
void		    YUV444_To_UYVY(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height);
void			YUV444_To_YUV422Planner(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height);
void			UYVY_To_YUV422Planner(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height);
bool			CopyToClipboard(const char* pszData, int nDataLen);
std::string		generateGUID();

struct  SATAInfo
{
	uint32_t index;
	std::string deviceID;
	uint32_t partitions;
	uint32_t scsiBus;
	std::string serialNumber;
	std::string name;
	uint64_t size;//in bytes
	uint32_t windowsDiskID;
	SATAInfo()
	{
		index = 0;
		deviceID = "";
		partitions = 0;
		scsiBus = 0;
		serialNumber = "";
		name = "";
		size = 0;
		windowsDiskID = 0;
	}
};
/*
return:
0: succeed , else failed
*/
int  GetSATAMap(std::vector<SATAInfo>& _sataTable);

//version 1
//http://www.fourcc.org/fccyvrgb.php
#define CLIP(X) ((X) > 255 ? 255 : (X) < 0 ? 0 : X)
#define RGB2Y(R, G, B) CLIP(0.299 * (R) + 0.587 * (G) + 0.114 * (B) )
#define RGB2U(R, G, B) CLIP(-0.169 * (R) - 0.331 * (G) + 0.499 * (B) + 128)
#define RGB2V(R, G, B) CLIP(0.499 * (R) - 0.418 * (G) - 0.0813 * (B) + 128)

// YUV -> RGB
#define YUV2R(Y, U, V) CLIP((298 * (Y - 16) + 516 * (U - 128) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP((298 * (Y - 16) - 100 * (U - 128) - 208 * (V - 128) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP((298 * (Y - 16) + 409 * (V - 128) + 128) >> 8)