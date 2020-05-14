#include "registryEdit.h"
#include <tchar.h>

CRegistryEdit::CRegistryEdit()
{
	m_hk = nullptr;
}

int CRegistryEdit::initialize(LPCTSTR _lpSubKey)
{
	if (m_hk != nullptr)
	{
		RegCloseKey(m_hk);
		m_hk = nullptr;
	}

	int ret = RegOpenKeyEx(HKEY_CURRENT_USER, _lpSubKey, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &m_hk);
	if (ret == ERROR_FILE_NOT_FOUND)
	{
		DWORD dwDisposition;
		ret = RegCreateKeyEx(HKEY_CURRENT_USER, _lpSubKey, 0, nullptr, 0, 0, nullptr, &m_hk, &dwDisposition);
		if (ret == ERROR_SUCCESS)
			ret = RegOpenKeyEx(HKEY_CURRENT_USER, _lpSubKey, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &m_hk);
	}

	return ret;
}

CRegistryEdit::~CRegistryEdit()
{
	RegCloseKey(m_hk);
}

int CRegistryEdit::getVal(LPCTSTR id, unsigned long *val, unsigned long defVal) const
{
	DWORD type = REG_DWORD;
	DWORD cbData = 1024;
	int ret = RegQueryValueEx(m_hk, id, 0, &type, LPBYTE(val), &cbData);
	if (ret != 0)
		*val = defVal;
	return ret;
}

int CRegistryEdit::setVal(LPCTSTR  id, unsigned long val) const
{
	int ret = RegSetValueEx(m_hk, id, 0, REG_DWORD, LPBYTE(&val), sizeof(DWORD));
	return ret;
}

int CRegistryEdit::setVal(LPCTSTR id, LPTSTR val) const
{
	int ret = RegSetValueEx(m_hk, id, 0, REG_SZ, LPBYTE(val), strlen((char*)val));
	return ret;
}

int CRegistryEdit::setVal(LPCSTR id, LPBYTE val) const
{
	int ret = RegSetValueExA(m_hk, id, 0, REG_SZ, LPBYTE(val), strlen((char*)val));
	return ret;
}

int CRegistryEdit::getVal(LPCTSTR id, LPBYTE val, DWORD len, LPCTSTR defVal) const
{
	DWORD type = REG_SZ;
	int ret = RegQueryValueEx(m_hk, id, nullptr, &type, val, &len);
	if (ret != 0 && defVal != nullptr)
		_tcscpy((TCHAR*)val, (TCHAR*)defVal);
	return ret;
}