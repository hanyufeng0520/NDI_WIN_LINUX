#include "CommonHelper.h"
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include "StringHelper.h"


std::string ipToString(uint32_t _ip)
{
	char ip[30];
	sprintf_s(ip, "%03u.%03u.%03u.%03u", _ip & 0xff, (_ip >> 8) & 0xff, (_ip >> 16) & 0xff, (_ip >> 24) & 0xff);
	return ip;
}

std::wstring ipToWString(uint32_t _ip)
{
	wchar_t ip[30];
	swprintf_s(ip, L"%03u.%03u.%03u.%03u", _ip & 0xff, (_ip >> 8) & 0xff, (_ip >> 16) & 0xff, (_ip >> 24) & 0xff);
	return ip;
}

std::string GetAppPathA()
{
	char strModuleFileName[MAX_PATH];
	char strDriver[MAX_PATH];
	char strPath[MAX_PATH];
	GetModuleFileNameA(nullptr, strModuleFileName, MAX_PATH);
	_splitpath_s(strModuleFileName, strDriver, std::size(strDriver), strPath, std::size(strPath), nullptr, 0, nullptr, 0);
	strcat_s(strDriver, std::size(strDriver), strPath);
	return strDriver;
}

std::wstring GetAppPathW()
{
	wchar_t strModuleFileName[MAX_PATH];
	wchar_t strDriver[MAX_PATH];
	wchar_t strPath[MAX_PATH];
	GetModuleFileNameW(nullptr, strModuleFileName, MAX_PATH);
	_wsplitpath_s(strModuleFileName, strDriver, std::size(strDriver), strPath, std::size(strPath), nullptr, 0, nullptr, 0);
	wcscat_s(strDriver, std::size(strDriver), strPath);
	return strDriver;
}

bool isValidString(const unsigned char* _str, int _len)
{
	for (int i = 0; i < _len; i++)
	{
		if (_str[i] < 32)
			return false;
		if (_str[i] > 126)
			return false;
	}

	return true;
}

void buildStrforGUID(char* _dst, int _dstSize, unsigned char* _src, int _srcSize)
{
	if (_srcSize >= 16)
	{
		sprintf_s(_dst, _dstSize, "%2.2x%2.2x%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
			_src[3], _src[2], _src[1], _src[0], _src[4], _src[5], _src[6], _src[7], _src[8], _src[9], _src[10], _src[11], _src[12], _src[13], _src[14], _src[15]);
	}
	else
	{
		char temp[10];
		int size = 0;
		for (int i = 0; i < _srcSize; i++)
		{
			sprintf_s(temp, sizeof(temp), "%2.2x", _src[i]);
			size += strlen(temp);
			strcat_s(_dst, _dstSize - size, temp);
		}
	}
}

std::string getIDString(const char* id, unsigned long size)
{
	char idString[256] = { 0 };
	if (isValidString((unsigned char*)id, size))
		strncpy_s(idString, id, size);
	else
		buildStrforGUID(idString, sizeof(idString), (unsigned char*)id, size);
	return idString;
}

std::wstring getIDWString(const char* id, unsigned long size)
{
	return string_to_wtring(getIDString(id, size));
}

unsigned char asByte(int value)
{
	if (value > 255)
		return 255;
	if (value < 0)
		return 0;
	return (unsigned char)value;
}

void YUV2RGB(unsigned char* bufYUV, unsigned char* bufRGB, int nbElem)
{
	for (int i = 0; i < nbElem / 2; i++)
	{
		int C = (bufYUV[(i * 4) + 1] - 16) * 298;    //Y
		int D = bufYUV[(i * 4) + 0] - 128;           //u
		int E = bufYUV[(i * 4) + 2] - 128;		     //v

		bufRGB[(i * 6) + 2] = asByte((C + 409 * E + 128) >> 8);
		bufRGB[(i * 6) + 1] = asByte((C - 100 * D - 208 * E + 128) >> 8);
		bufRGB[(i * 6) + 0] = asByte((C + 516 * D + 128) >> 8);

		C = (bufYUV[(i * 4) + 3] - 16) * 298;
		D = bufYUV[(i * 4) + 0] - 128;
		E = bufYUV[(i * 4) + 2] - 128;

		bufRGB[(i * 6) + 5] = asByte((C + 409 * E + 128) >> 8);
		bufRGB[(i * 6) + 4] = asByte((C - 100 * D - 208 * E + 128) >> 8);
		bufRGB[(i * 6) + 3] = asByte((C + 516 * D + 128) >> 8);
	}
}

int YUV444ToRGB888(unsigned char* src_buffer, int w, int h, unsigned char* des_buffer, bool bBGR)
{
	unsigned char* yuv, * rgb;
	unsigned char U, V, Y;

	yuv = src_buffer;
	rgb = des_buffer;

	if (yuv == NULL || rgb == NULL)
	{
		printf("error: input data null!\n");
		return -1;
	}

	int size = w * h;
	int rOffset, bOffset;
	if (bBGR)
	{
		rOffset = 2;
		bOffset = 0;
	}
	else
	{
		rOffset = 0;
		bOffset = 2;
	}

	for (int i = 0; i < size; i++)
	{
		Y = yuv[3 * i + 0];
		U = yuv[3 * i + 1];
		V = yuv[3 * i + 2];

		unsigned char& B = rgb[3 * i + bOffset];
		unsigned char& G = rgb[3 * i + 1];
		unsigned char& R = rgb[3 * i + rOffset];

		B = asByte((298 * (Y - 16) + 516 * (U - 128) + 128) >> 8);
		G = asByte((298 * (Y - 16) - 100 * (U - 128) - 208 * (V - 128) + 128) >> 8);
		R = asByte((298 * (Y - 16) + 409 * (V - 128) + 128) >> 8);
	}

	return 0;
}

int RGB888ToYUV444(unsigned char* src_buffer, int w, int h, unsigned char* des_buffer, bool bBGR)
{
	unsigned char* yuv, * rgb;
	unsigned char u, v, y;

	yuv = des_buffer;
	rgb = src_buffer;

	if (yuv == NULL || rgb == NULL)
	{
		printf("error: input data null!\n");
		return -1;
	}

	int size = w * h;

	for (int i = 0; i < size; i++)
	{
		y = yuv[3 * i + 0];
		u = yuv[3 * i + 1];
		v = yuv[3 * i + 2];

#if 0
		rgb[3 * i + 2] = (unsigned char)(y1 + 1.402 * (u - 128));//B
		rgb[3 * i + 1] = (unsigned char)(y1 - 0.344 * (u - 128) - 0.714 * (v - 128));//G
		rgb[3 * i + 0] = (unsigned char)(y1 + 1.772 * (v - 128));//R

		rgb[3 * i + 5] = (unsigned char)(y2 + 1.375 * (u - 128));
		rgb[3 * i + 4] = (unsigned char)(y2 - 0.344 * (u - 128) - 0.714 * (v - 128));
		rgb[3 * i + 3] = (unsigned char)(y2 + 1.772 * (v - 128));
#endif
#if  1
		int rOffset, bOffset;
		if (bBGR)
		{
			rOffset = 2;
			bOffset = 0;
		}
		else
		{
			rOffset = 0;
			bOffset = 2;
		}
		float b = rgb[3 * i + bOffset];//B
		float g = rgb[3 * i + 1];//G
		float r = rgb[3 * i + rOffset];//R
		y = asByte(0.299 * r + 0.587 * g + 0.114 * b);
		u = asByte(-0.169 * r - 0.331 * g + 0.499 * b + 128);
		v = asByte(0.499 * r - 0.418 * g - 0.0813 * b + 128);
#endif
	}

	return 0;
}

void ReadSataInfo(IWbemClassObject* _pObj, std::wstring _key, void* _value)
{
	VARIANT vtProp;
	// Get the value of the Name property
	HRESULT hr = _pObj->Get((LPCWSTR)_key.c_str(), 0, &vtProp, 0, 0);

	if (wcscmp((const wchar_t*)_key.c_str(), L"Size") == 0)
	{
		LONGLONG size = _wtoi64(vtProp.bstrVal);
		*(LONGLONG*)_value = size;
	}
	else if (vtProp.bstrVal && vtProp.vt == VT_BSTR)
	{
		std::wstring  tem = vtProp.bstrVal;

		std::string str(tem.begin(), tem.end());//!!! only work for ascii code
		*(std::string*)_value = str;
	}
	else if (vtProp.vt == VT_UI4)
	{
		*(uint32_t*)_value = vtProp.ullVal;
	}
	else if (vtProp.vt == VT_I4)
	{
		*(int*)_value = vtProp.iVal;
	}
	VariantClear(&vtProp);
}
int  GetSATAMap(std::vector<SATAInfo>& _sataTable)
{
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	_sataTable.clear();
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		return 1;                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities
		NULL                         // Reserved
		);

	if (FAILED(hres))
	{
		CoUninitialize();
		return 1;                    // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator* pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc);

	if (FAILED(hres))
	{
		std::cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< std::hex << hres << std::endl;
		CoUninitialize();
		return 1;                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices* pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		std::cout << "Could not connect. Error code = 0x"
			<< std::hex << hres << std::endl;
		pLoc->Release();
		CoUninitialize();
		return 1;                // Program has failed.
	}

	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities
		);

	if (FAILED(hres))
	{
		std::cout << "Could not set proxy blanket. Error code = 0x"
			<< std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		//bstr_t("SELECT * FROM Win32_OperatingSystem"),
		bstr_t("SELECT * FROM Win32_DiskDrive"),
		//bstr_t("SELECT * FROM Win32_PnPSignedDriver WHERE DeviceName ='Disk drive'"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		std::cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;
	int count = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		SATAInfo sataInfo;
		std::string mediaType;
		ReadSataInfo(pclsObj, L"MediaType", &mediaType);

		if (mediaType != "Fixed hard disk media")
		{
			continue;
		}

		ReadSataInfo(pclsObj, L"DeviceID", (void*)&sataInfo.deviceID);
		if (sataInfo.deviceID.length())
		{
			std::string path = sataInfo.deviceID;
			sataInfo.windowsDiskID = (uint32_t)atoi(path.erase(0, path.find('E') + 1).c_str());
		}

		ReadSataInfo(pclsObj, L"Partitions", (void*)&sataInfo.partitions);
		ReadSataInfo(pclsObj, L"SCSIBus", (void*)&sataInfo.scsiBus);
		ReadSataInfo(pclsObj, L"SerialNumber", (void*)&sataInfo.serialNumber);
		ReadSataInfo(pclsObj, L"Size", (void*)&sataInfo.size);
		ReadSataInfo(pclsObj, L"Name", (void*)&sataInfo.name);
		sataInfo.index = count++;

		pclsObj->Release();
		_sataTable.emplace_back(sataInfo);
	}
	/*
	uint32_t goodDiskSizeInGB = 0;
	for (vector<SATAInfo>::iterator iter = _sataTable.begin(); iter != _sataTable.end(); iter++)
	{
		uint32_t diskSizeInGB = iter->size / (1024 * 1024 * 1024);
		goodDiskSizeInGB = max(goodDiskSizeInGB, diskSizeInGB);
	}
	*/

	for (std::vector<SATAInfo>::iterator iter = _sataTable.begin(); iter != _sataTable.end(); )
	{
		//uint32_t diskSizeInGB = iter->size / (1024 * 1024 * 1024);
		if (iter->scsiBus == 0 && iter->partitions > 0)
		{
			//not good
			iter = _sataTable.erase(iter);
		}
		else
			iter++;
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return 0;   // Program successfully completed.
}

std::pair<int, int>ParseVersion(std::string versionString)
{
	int majorVal = 0, minorVal = 0;
	char value[32];
	int posMajor = versionString.find('.', 0);
	if (posMajor >= 0)
	{
		versionString._Copy_s(value, 32, posMajor, 0);
		majorVal = atoi(value);
	}

	int posMinor = versionString.find('.', posMajor);

	if (posMinor >= 0)
	{
		versionString._Copy_s(value, 32, posMinor, posMajor + 1);
		minorVal = atoi(value);
	}

	return std::make_pair(majorVal, minorVal);
}

bool checkStroageCompatibility(std::string _newV, std::string _oldV)
{
	std::pair<int, int>newRevsion = ParseVersion(_newV);
	std::pair<int, int>oldRevsion = ParseVersion(_oldV);

	return (newRevsion.first == oldRevsion.first) && (newRevsion.second == oldRevsion.second);
}

void UYVY_To_YUV444(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height)
{
	unsigned char* pixel = _pDst;
	unsigned char* source = _pSrc;

	for (int l = 0; l < _height; l++)
	{
		for (int c = 0; c < _width / 2; c++)
		{
			*pixel++ = source[1];			// Y
			*pixel++ = source[0];			// U
			*pixel++ = source[2];			// V

			*pixel++ = source[3];			// Y
			*pixel++ = source[0];			// U
			*pixel++ = source[2];			// V
			source += 4;
		}
	}
}

void YUV422Planar_To_YUV444(unsigned char* _pDst, unsigned char* _pSrcY, unsigned char* _pSrcU, unsigned char* _pSrcV, int _width, int _height)
{
	unsigned char* pixel = _pDst;

	for (int l = 0; l < _height; l++)
	{
		for (int c = 0; c < _width / 2; c++)
		{
			*pixel++ = _pSrcY[0];			// Y
			*pixel++ = _pSrcU[0];			// U
			*pixel++ = _pSrcV[0];			// V

			*pixel++ = _pSrcY[1];			// Y
			*pixel++ = _pSrcU[0];			// U
			*pixel++ = _pSrcV[0];			// V
			_pSrcY += 2;
			_pSrcU++;
			_pSrcV++;
		}
	}
}

uint32_t round(uint32_t _value, int _base)
{
	return(_value + (_base - 1)) & (~(_base - 1));
}

void YUV444_To_UYVY(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height)
{
	unsigned char* pixel = _pDst;
	unsigned char* source = _pSrc;

	for (int l = 0; l < _height; l++)
	{
		for (int c = 0; c < _width / 2; c++)
		{
			*pixel++ = (int)(source[1] + source[4]) / 2;			// U
			*pixel++ = source[0];								// Y

			*pixel++ = (int)(source[2] + source[5]) / 2;			// V
			*pixel++ = source[3];								// Y
			source += 6;
		}
	}
}

void YUV444_To_YUV422Planner(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height)
{
	unsigned char* pDstY = _pDst;
	unsigned char* pDstU = _pDst + _width * _height;
	unsigned char* pDstV = _pDst + (_width * _height) * 3 / 2;

	unsigned char* source = _pSrc;

	for (int l = 0; l < _height; l++)
	{
		for (int c = 0; c < _width / 2; c++)
		{
			*pDstU++ = (int)(source[1] + source[4]) / 2;		// U
			*pDstY++ = source[0];								// Y

			*pDstV++ = (int)(source[2] + source[5]) / 2;		// V
			*pDstY++ = source[3];								// Y
			source += 6;
		}
	}
}

void UYVY_To_YUV422Planner(unsigned char* _pDst, unsigned char* _pSrc, int _width, int _height)
{
	unsigned char* pDstY = _pDst;
	unsigned char* pDstU = _pDst + 1920 * 1080;
	unsigned char* pDstV = _pDst + 1920 * 1080 * 3 / 2;

	unsigned char* source = _pSrc;

	for (int l = 0; l < _height; l++)
	{
		for (int c = 0; c < _width / 2; c++)
		{
			*pDstU++ = source[0];			// U
			*pDstY++ = source[1];			// Y

			*pDstV++ = source[2];			// V
			*pDstY++ = source[3];			// Y
			source += 4;
		}
	}
}

bool CopyToClipboard(const char* pszData, int nDataLen)
{
	if (OpenClipboard(nullptr))
	{
		EmptyClipboard();
		HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, nDataLen + 1);
		if (clipbuffer == nullptr)
			return false;
		char* buffer = (char*)GlobalLock(clipbuffer);
		if (buffer == nullptr)
			return false;
		strcpy_s(buffer, nDataLen + 1, pszData);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
		return true;
	}
	return false;
}

std::string generateGUID()
{
	GUID guid;
	CoCreateGuid(&guid);
	char id[70] = { 0 };
	sprintf_s(id, "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return id;
}