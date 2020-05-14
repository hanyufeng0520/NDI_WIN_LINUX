#include "AsyncFileWriterImp.h"
#include <Windows.h>

CAsyncFileWriterImp::CAsyncFileWriterImp(const char * _pFilePath, int _bufferSize, int _bufferCnt)
{
	strcpy_s(m_filePath, 512, _pFilePath);
	m_bufferCnt = _bufferCnt;
	m_bufferSize = _bufferSize;
	m_hFile = nullptr;
	for (int i = 0; i < m_bufferCnt; i++)
	{
		void * pBuf = malloc(_bufferSize);
		m_buffEmptyaList.emplace_back(std::make_pair(pBuf, _bufferSize));
	}
}

void  CAsyncFileWriterImp::_Work()
{
	while (m_bRun)
	{
		if (m_buffDataList.empty())
		{
			Sleep(2);
			continue;
		}
		_Lock();
		std::pair<void*, int> buf = m_buffDataList.front();
		m_buffDataList.erase(m_buffDataList.begin());
		_UnLock();

		fwrite(buf.first, 1, buf.second, m_hFile);

		_Lock();
		m_buffEmptyaList.emplace_back(buf);
		_UnLock();
	}
}

void CAsyncFileWriterImp::_Lock()
{
	m_locker.lock();
}

void CAsyncFileWriterImp::_UnLock()
{
	m_locker.unlock();
}

CAsyncFileWriterImp::~CAsyncFileWriterImp()
{
	for (auto& item : m_buffEmptyaList)
	{
		free(item.first);
	}
	m_buffEmptyaList.clear();

	for (auto& item : m_buffDataList)
	{
		free(item.first);
	}
	m_buffDataList.clear();
}

bool CAsyncFileWriterImp::Start()
{
	int err = 0;
	if (!m_hFile)
	{
		err = fopen_s(&m_hFile, m_filePath, "wb");
	}

	m_bRun = true;
	m_hTask = async_thread(thread_priority::below_normal, &CAsyncFileWriterImp::_Work, this);

	return err == 0;
}

void CAsyncFileWriterImp::Stop()
{
	m_bRun = false;
	if (m_hTask.valid())
		m_hTask.get();
	if (m_hFile)
	{
		fclose(m_hFile);
	}
	m_hFile = nullptr;
}

int CAsyncFileWriterImp::PushData(char * _pData, int _dataSize)
{
	if (m_buffEmptyaList.empty())
		return -1;
	if (_dataSize > m_bufferSize)
		return -2;
	_Lock();
	std::pair<void*, int> buf = m_buffEmptyaList.front();
	m_buffEmptyaList.erase(m_buffEmptyaList.begin());
	_UnLock();

	memcpy_s(buf.first, m_bufferSize, _pData, _dataSize);
	buf.second = _dataSize;

	_Lock();
	m_buffDataList.emplace_back(buf);
	_UnLock();

	return 0;
}

bool CAsyncFileWriterImp::IsStarted()
{
	return m_bRun;
}