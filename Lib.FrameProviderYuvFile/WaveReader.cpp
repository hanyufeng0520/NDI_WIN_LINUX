#include "WaveReader.h"
#include <stdio.h>
#ifdef _MSC_VER
#include <mbstring.h>
#else 
#endif 
#include <string.h>
#include <stdlib.h>
#include "../Lib.Base/audioFrame.h"

CWaveReader::CWaveReader()
{
	m_wavInfo.fp = nullptr;
	m_pRawWave = new unsigned char[16 * 4 * 2000];
	memset(&m_wavInfo, 0, sizeof(m_wavInfo));
}

CWaveReader::~CWaveReader()
{
	close();
}

void CWaveReader::initLog(wchar_t* szLogFile)
{
	m_szLogFile = szLogFile;	
}

int CWaveReader::openFile(const char* _file_name)
{
	if (nullptr == _file_name)
	{
		printf("CWaveReader::openFile Failed.File name is empty.\n");
		return -1;
	}
#ifndef _MSC_VER
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif 
	fopen_s(&m_wavInfo.fp, _file_name, "rb");
	if (nullptr == m_wavInfo.fp)
	{
		printf("CWaveReader::openFile Failed.Open file (%s) Failed.\n", _file_name);
		return -2;
	}
	return 0;
}

int CWaveReader::checkFileHeader()
{
	//handle RIFF WAVE chunk
	char buffer[12];
	int read_len = (int)fread(buffer, 1, 12, m_wavInfo.fp);
	if (read_len < 12)
	{
		printf("CWaveReader::openFile Failed.Read header 12 bytes.\n");
		close();
		return -1;
	}
#ifdef _MSC_VER
	if (_mbsnicmp((unsigned char*)buffer, (unsigned char*)"RIFF", 4))
#else 
	if (strncasecmp((const char*)buffer, (const char*)"RIFF", 4))
#endif	
	{
		printf("CWaveReader::openFile Failed.Read RIFF bytes.\n");
		close();
		return -2;
	}

	memcpy(m_wavInfo.riff.ChunkID, buffer, 4);
	m_wavInfo.riff.ChunkSize = *(int*)(buffer + 4);
#ifdef _MSC_VER
	if (_mbsnicmp((unsigned char*)buffer + 8, (unsigned char*)"WAVE", 4))
#else 
	if (strncasecmp((const char*)buffer + 8, (const char*)"WAVE", 4))
#endif	
	{
		printf("CWaveReader::openFile Failed.Read WAVE bytes.\n");
		close();
		return -3;
	}
	memcpy(m_wavInfo.riff.Format, buffer + 8, 4);
	m_wavInfo.file_size = m_wavInfo.riff.ChunkSize + 8;
	return 0;
}

int CWaveReader::getMarker(char(&_marker)[4], int& size)
{
	char buffer[8];

	int read_len = (int)fread(buffer, 1, 8, m_wavInfo.fp);
	if (read_len < 8)
	{
		printf("CWaveReader::openFile Failed.Read 8 bytes.\n");
		close();
		return -1;
	}

	memcpy(_marker, buffer, 4);
	size = *(int*)(buffer + 4);
	return 0;
}

bool CWaveReader::isMarkerFMT(const char* _marker)
{
#ifdef _MSC_VER
	if (0 == _mbsnicmp((unsigned char*)_marker, (unsigned char*)"FMT", 3))
#else 
	if (0 == strncasecmp((const char*)_marker, (const char*)"FMT", 3))
#endif	
		return true;
	return false;
}

bool CWaveReader::isMarkerDATA(const char* _marker)
{
#ifdef _MSC_VER
	if (0 == _mbsnicmp((unsigned char*)_marker, (unsigned char*)"DATA", 4))
#else 
	if (0 == strncasecmp((const char*)_marker, (const char*)"DATA", 4))
#endif	
		return true;
	return false;
}

int CWaveReader::readFMT(const char* _marker, int _markerSize)
{
	char buffer[256];

	memcpy(m_wavInfo.format.Subchunk1ID, _marker, 4);
	m_wavInfo.format.Subchunk1Size = _markerSize;

	int read_len = (int)fread(buffer, 1, _markerSize, m_wavInfo.fp);
	if (read_len < _markerSize)
	{
		printf("CWaveReader::openFile Failed.Read (%d) bytes.\n", _markerSize);
		close();
		return -1;
	}
	m_wavInfo.format.AudioFormat	= *(short*)buffer;
	m_wavInfo.format.NumChannels	= *(short*)(buffer + 2);
	m_wavInfo.format.SampleRate		= *(int*)(buffer + 4);
	m_wavInfo.format.ByteRate		= *(int*)(buffer + 8);
	m_wavInfo.format.BlockAlign		= *(short*)(buffer + 12);
	m_wavInfo.format.BitsPerSample	= *(short*)(buffer + 14);

	return 0;
}

int CWaveReader::readDATA(const char* _marker, int _markerSize, int& _offset)
{
	memcpy(m_wavInfo.data.Subchunk2ID, _marker, 4);
	m_wavInfo.data.Subchunk2Size = _markerSize;
	_offset += 8;
	m_wavInfo.data_offset = _offset;
	m_wavInfo.data_size = m_wavInfo.data.Subchunk2Size;
	return 0;
}

bool CWaveReader::open(const char * _file_name, wchar_t* _szLogFile)
{
	initLog(_szLogFile);
	if (openFile(_file_name))
		return false;

	if (checkFileHeader())
	{
		close(); return false;
	}

	int offset = 12;
	char marker[4];
	int markerSize;

	while (getMarker(marker, markerSize)==0)
	{
		if (isMarkerFMT(marker))
		{
			if (readFMT(marker, markerSize))
				break;
		}
		else if (isMarkerDATA(marker))
		{
			readDATA(marker, markerSize, offset);
			break;
		}
		else
#ifdef _MSC_VER
			_fseeki64(m_wavInfo.fp, markerSize, SEEK_CUR);
#else
			fseeko64(m_wavInfo.fp, markerSize, SEEK_CUR);
#endif 

		offset += 8 + markerSize;
		printf("markerSize :%d\n", markerSize);

	}

	if ((m_wavInfo.format.BitsPerSample == 16 || m_wavInfo.format.BitsPerSample == 24 || m_wavInfo.format.BitsPerSample == 32) && m_wavInfo.format.SampleRate == 48000)
	{
#ifdef _MSC_VER
		_fseeki64(m_wavInfo.fp, m_wavInfo.data_offset, SEEK_SET);
#else
		fseeko64(m_wavInfo.fp, m_wavInfo.data_offset, SEEK_SET);
#endif 
		return true;
	}

	printf("CWaveReader::openFile(%s) Failed.samples_per_sec: %d,bits_per_sample: %d \n",
		_file_name, m_wavInfo.format.SampleRate, m_wavInfo.format.BitsPerSample);
	close();
	return false;
}

void CWaveReader::close()
{
	if (nullptr != m_wavInfo.fp)
		fclose(m_wavInfo.fp);
	m_wavInfo.fp = nullptr;
	if (m_pRawWave != nullptr)
	{
		delete[]m_pRawWave;
		m_pRawWave = nullptr;
	}
}

int CWaveReader::fillBufferFromDisk(int _sampleCount)
{
	int nReadSize = _sampleCount * m_wavInfo.format.BlockAlign;

	int nRes = (int)fread(m_pRawWave, 1, nReadSize, m_wavInfo.fp);
	if (nRes != nReadSize)
	{
#ifdef _MSC_VER
		_fseeki64(m_wavInfo.fp, m_wavInfo.data_offset, SEEK_SET);
#else
		fseeko64(m_wavInfo.fp, m_wavInfo.data_offset, SEEK_SET);
#endif 
		nRes = (int)fread(m_pRawWave, 1, nReadSize, m_wavInfo.fp);
		if (nRes != nReadSize)
			return -1;
	}

	return 0;
}

bool CWaveReader::getAudioFrame(pAframe& _aFrame)
{
	//we have issue here.JKL_NEEDTOADD
	//return true;
	_aFrame->setToMute();

	if (nullptr == m_wavInfo.fp)
		return false;

	int sampleCount = _aFrame->getSampleCount();
	int nbChannel = _aFrame->GetMonoCnt();
	int nbBitPerSample = _aFrame->getNbBitPerSample();

	if (fillBufferFromDisk(sampleCount))
		return false;

	int32_t  * pDest32 = (int32_t*)_aFrame->getRaw();
	short * pDest16 = (short*)_aFrame->getRaw();
	int32_t* pSource32 = (int32_t*)m_pRawWave;
	short * pSource16 = (short*)m_pRawWave;
	unsigned char * pSource8 = (unsigned char*)m_pRawWave;
	for (int i = 0; i < sampleCount; i++)
	{
		for (int cnl = 0; cnl < nbChannel; cnl++)
		{
			if (cnl < m_wavInfo.format.NumChannels)
			{
				if (m_wavInfo.format.BitsPerSample == 16)
				{
					if (nbBitPerSample == 32)
						*pDest16++ = (*pSource32++) / (256 * 256);
					else
						*pDest16++ = *pSource16++;
				}
				else if (m_wavInfo.format.BitsPerSample == 24)
				{
					long* s = (long*)pSource8;
					long  d = (*s) << 8;

					if (d & 0x80000000)
						d |= 0x000000FF;
					else
						d &= 0xFFFFFF00;
					
					if (nbBitPerSample == 32)
						*pDest32++ = d;
					else
						*pDest16++ = (d/(256*256));
					pSource8 += 3;
				}
				else if (m_wavInfo.format.BitsPerSample == 32)
				{
					if (nbBitPerSample == 32)
						*pDest32++ = *pSource32++;				
					else
						*pDest16++ = (*pSource32++) / (256 * 256);
				}
			}
			else
				if (nbBitPerSample == 32)			// set to  mute
					*pDest32++ = 0;
				else
					*pDest16++ = 0;	
		}
	}
	return true;
}
