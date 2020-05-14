#pragma once
#include <vector>
#include <future>
#include "async_thread.h"
#include <stdio.h>

class CAsyncFileWriterImp
{
public:
	CAsyncFileWriterImp(const char* _pFilePath, int _bufferSize, int _bufferCnt);
	~CAsyncFileWriterImp();
	bool 	Start();
	void    Stop();
	int     PushData(char* _pData, int _dataSize);
	bool	IsStarted();

private:
	void    _Work();
	void    _Lock();
	void    _UnLock();

	char                                 m_filePath[512];
	int                                  m_bufferSize;
	int                                  m_bufferCnt;
	FILE*                                m_hFile;
	async_future                         m_hTask;
	bool			                     m_bRun;
	std::vector<std::pair<void*, int>>   m_buffDataList;
	std::vector<std::pair<void*, int>>   m_buffEmptyaList;
	std::mutex					         m_locker;
};
