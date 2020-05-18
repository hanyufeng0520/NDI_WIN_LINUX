#pragma once
#include "../Lib.Base/platform.h"
#include <memory>
#include "WaveHeader.h"
#include "../Lib.Base/audioFrame.h"

class CWaveReader
{
	wav_t m_wavInfo;
	unsigned char* m_pRawWave;
	wchar_t *m_szLogFile;
	void  initLog(wchar_t* szLogFile);
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
	bool open(const char *file_name, wchar_t*szLogFile);
	void close();
	bool getAudioFrame(pAframe& _aFrame);
};
