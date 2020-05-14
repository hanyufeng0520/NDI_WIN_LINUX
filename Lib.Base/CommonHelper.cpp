#include "CommonHelper.h"
#include <iostream>
#include "StringHelper.h"

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