#pragma once
#include <windows.h>

#define BE_STORAGE_REG_SUB_KEY L"SOFTWARE\\Id4.tv\\ViBox\\Storage"
#define BE_LICENSE_REG_SUB_KEY L"SOFTWARE\\SimplyLive.TV\\Backend\\License"

class CRegistryEdit
{
	HKEY m_hk;

public:
	CRegistryEdit();
	~CRegistryEdit();

	int initialize(LPCTSTR _lpSubKey);
	int setVal(LPCTSTR id, unsigned long val) const;
	int setVal(LPCTSTR id, LPTSTR val) const;
	int setVal(LPCSTR id, LPBYTE val) const;
	int getVal(LPCTSTR id, unsigned long *val, unsigned long defVal) const;
	int getVal(LPCTSTR id, LPBYTE val, DWORD len, LPCTSTR defVal) const;
};
