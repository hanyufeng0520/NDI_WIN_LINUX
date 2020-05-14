#pragma once


// More info on http://soundfile.sapp.org/doc/WaveFormat/

//------------------------------------------------------------------------------------------------
//                                           RIFF

// Exemple
//                 R   I    F    F   |--     2084    --|  W    A    V    E
//               0x52 0x49 0x46 0x46 0x24 0x08 0x00 0x00 0x57 0x41 0x56 0x45 

typedef struct _wav_riff_t {
	char ChunkID[4];	// Contains the letters "RIFF" in ASCII form (0x52494646 big - endian form)
	int  ChunkSize;		// 36 + SubChunk2Size, or more precisely: 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
						// This is the size of the rest of the chunk following this number.
						// This is the size of the entire file in bytes minus 8 bytes for the
						// two fields not included in this count: ChunkID and ChunkSize.
	char Format[4];		// Contains the letters "WAVE" (0x57415645 big - endian form)
} wav_riff_t;

// The "WAVE" format consists of two subchunks: "fmt " and "data":

//------------------------------------------------------------------------------------------------
//                                            fmt
//
// The "fmt " subchunk describes the sound data's format:

// Exemple
//                f    m    t        |--      16     --| |- PCM -| |- 2cnl-|
//               0x66 0x6d 0x74 0x20 0x10 0x00 0x00 0x00 0x01 0x00 0x02 0x00 
//
//               |-- Sample Rate --| |--  Byte rate  --| |- B Al-| |- Bit -|
//				 0x20 0x56 0x00 0x00 0x88 0x58 0x01 0x00 0x04 0x00 0x10 0x00

typedef struct _wav_format_t {
	char  Subchunk1ID[4];	// Contains the letters "fmt " (0x666d7420 big - endian form).
	int   Subchunk1Size;	// 16 for PCM.  This is the size of the rest of the Subchunk which follows this number.
	short AudioFormat;		// PCM = 1 (i.e. Linear quantization)
							// Values other than 1 indicate some form of compression.
	short NumChannels;		// Mono = 1, Stereo = 2, etc.
	int   SampleRate;		// 8000, 44100, 48000, etc.
	int   ByteRate;			// SampleRate * NumChannels * BitsPerSample/8
	short BlockAlign;		// NumChannels * BitsPerSample/8
							// The number of bytes for one sample including all channels.
	short BitsPerSample;	// 8 bits = 8, 16 bits = 16, etc.
} wav_format_t;

//------------------------------------------------------------------------------------------------
//                                            data
//
// The "data" subchunk contains the size of the data and the actual sound:

// Exemple
//
//		 d    a    t    a   |--  Size 2048  --|
//      0x64 0x61 0x74 0x61 0x00 0x08 0x00 0x00  
//
//      |------  Sample  1  ----| |------  Sample  2  ----| |------  Sample  3  ----| |------  Sample  4  ----|
//      |- mono 1 -| |- mono 2 -| |- mono 1 -| |- mono 2 -| |- mono 1 -| |- mono 2 -| |- mono 1 -| |- mono 2 -|
//       0x00  0x00   0x00  0x00   0x24  0x17   0x1e  0xf3   0x3c  0x14   0x3c  0x14   0x16  0xf9   0x18  0xf9
//
//      |------  Sample  5  ----| |------  Sample  6  ----| |------  Sample  7  ----| |------  Sample  8  ----|
//      |- mono 1 -| |- mono 2 -| |- mono 1 -| |- mono 2 -| |- mono 1 -| |- mono 2 -| |- mono 1 -| |- mono 2 -|
//       0x34  0xe7   0x23  0xa6   0x3c  0xf2   0x24  0xf2   0x11  0xce   0x1a  0x0d   0x .......

typedef struct _wav_data_t {
	char Subchunk2ID[4];	// Contains the letters "data" (0x64617461 big - endian form).
	int  Subchunk2Size;		// NumSamples * NumChannels * BitsPerSample/8
							// This is the number of bytes in the data.
							// You can also think of this as the size of the read of the subchunk following this number.
} wav_data_t;



typedef struct _wav_t {
	FILE          * fp;
	wav_riff_t		riff;
	wav_format_t    format;
	wav_data_t      data;
	int             file_size;
	int             data_offset;
	int             data_size;
} wav_t; 


