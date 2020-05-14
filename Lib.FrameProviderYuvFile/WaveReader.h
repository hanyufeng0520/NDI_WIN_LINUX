#pragma once
#include <tchar.h>
#include <windows.h>
#include <memory>
#include "WaveHeader.h"

class AudioFrame;

class CWaveReader
{
	wav_t m_wavInfo;
	BYTE* m_pRawWave;
	TCHAR m_szLogFile[MAX_PATH];
	void  initLog(TCHAR* szLogFile);
	int   openFile(const char* file_name);
	int   checkFileHeader();
	int   getMarker(char(&_marker)[4], int& size);
	bool  isMarkerFMT(const char* _marker);
	bool  isMarkerDATA(const char * _marker);
	int   readFMT(const char* _marker, int size);
	int   readDATA(const char* _marker, int size, int & _offset);
	int   fillBufferFromDisk(int _sampleCount);

public:
	CWaveReader();
	~CWaveReader();
	bool open(const char *file_name, TCHAR *szLogFile);
	void close();
	bool getAudioFrame(std::shared_ptr<AudioFrame>& _aFrame);
};
