#pragma once
#include <cstdint>
#include <cmath>

struct ControlerTimeCode
{
	ControlerTimeCode()
	{
		Hour = 0;
		Minute = 0;
		Second = 0;
		Frame = 0;
	}
	void SetTimeCode(unsigned char _Hour, unsigned char _Minute, unsigned char _Second, unsigned char _Frame)
	{
		Hour = _Hour;
		Minute = _Minute;
		Second = _Second;
		Frame = _Frame;
	}
	void SetFps(float fps)
	{
		if (fabs(fps - 24) < 0.01)
		{
			s_UnitsCorrection = 0;
			s_UnitsPerSec = 24;
			s_UnitsPerMinute = s_UnitsPerSec * 60;
			s_UnitsPer10Minute = s_UnitsPerMinute * 10;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;

			s_UnitsPerSecDuration = 24;
		}
		else if (fabs(fps - 25) < 0.01)
		{
			s_UnitsCorrection = 0;
			s_UnitsPerSec = 25;
			s_UnitsPerMinute = s_UnitsPerSec * 60;
			s_UnitsPer10Minute = s_UnitsPerMinute * 10;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;

			s_UnitsPerSecDuration = 25;
		}
		else if (fabs(fps - 29.97) < 0.01)
		{
			s_UnitsCorrection = 2;

			s_UnitsPerSec = 30;
			s_UnitsPerMinute = (s_UnitsPerSec * 60) - s_UnitsCorrection;
			s_UnitsPer10Minute = (s_UnitsPerMinute * 10) + s_UnitsCorrection;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;

			s_UnitsPerSecDuration = 29.97;
		}
		else if (fabs(fps - 30) < 0.01)
		{
			s_UnitsCorrection = 0;
			s_UnitsPerSec = 30;
			s_UnitsPerMinute = s_UnitsPerSec * 60;
			s_UnitsPer10Minute = s_UnitsPerMinute * 10;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;
			s_UnitsPerSecDuration = 30;
		}
		else if (fabs(fps - 50) < 0.01)
		{
			s_UnitsCorrection = 0;
			s_UnitsPerSec = 25;
			s_UnitsPerMinute = s_UnitsPerSec * 60;
			s_UnitsPer10Minute = s_UnitsPerMinute * 10;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;
			s_UnitsPerSecDuration = 25;

			s_UnitsCorrection = s_UnitsCorrection * 2;
			s_UnitsPerSec = s_UnitsPerSec * 2;
			s_UnitsPerMinute = s_UnitsPerMinute * 2;
			s_UnitsPer10Minute = s_UnitsPer10Minute * 2;
			s_UnitsPerHour = s_UnitsPerHour * 2;
			s_UnitsPerDay = s_UnitsPerDay * 2;
			s_UnitsPerSecDuration = s_UnitsPerSecDuration * 2;
		}
		else if (fabs(fps - 59.94) < 0.01)
		{
			s_UnitsCorrection = 2;
			s_UnitsPerSec = 30;
			s_UnitsPerMinute = (s_UnitsPerSec * 60) - s_UnitsCorrection;
			s_UnitsPer10Minute = (s_UnitsPerMinute * 10) + s_UnitsCorrection;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;
			s_UnitsPerSecDuration = 29.97;

			s_UnitsCorrection = s_UnitsCorrection * 2;
			s_UnitsPerSec = s_UnitsPerSec * 2;
			s_UnitsPerMinute = s_UnitsPerMinute * 2;
			s_UnitsPer10Minute = s_UnitsPer10Minute * 2;
			s_UnitsPerHour = s_UnitsPerHour * 2;
			s_UnitsPerDay = s_UnitsPerDay * 2;

			s_UnitsPerSecDuration = s_UnitsPerSecDuration * 2;
		}
		else if (fabs(fps - 60) < 0.01)
		{
			s_UnitsCorrection = 0;
			s_UnitsPerSec = 30;
			s_UnitsPerMinute = s_UnitsPerSec * 60;
			s_UnitsPer10Minute = s_UnitsPerMinute * 10;
			s_UnitsPerHour = s_UnitsPer10Minute * 6;
			s_UnitsPerDay = s_UnitsPerHour * 24;
			s_UnitsPerSecDuration = 30;

			s_UnitsCorrection = s_UnitsCorrection * 2;
			s_UnitsPerSec = s_UnitsPerSec * 2;
			s_UnitsPerMinute = s_UnitsPerMinute * 2;
			s_UnitsPer10Minute = s_UnitsPer10Minute * 2;
			s_UnitsPerHour = s_UnitsPerHour * 2;
			s_UnitsPerDay = s_UnitsPerDay * 2;
			s_UnitsPerSecDuration = s_UnitsPerSecDuration * 2;
		}
	}
	uint64_t ToFrame() const
	{
		uint64_t nbr10Minute = (uint64_t)Minute / 10;
		uint64_t nbrMinute = (uint64_t)Minute % 10;

		uint64_t frame = (uint64_t)((Hour * s_UnitsPerHour)
			+ (nbr10Minute * s_UnitsPer10Minute)
			+ (nbrMinute * s_UnitsPerMinute)
			+ (Second * s_UnitsPerSec)
			+ Frame);

		return frame;
	}
	void FromFrame(int64_t _Frame)
	{
		HourDuration = (uint8_t)(int64_t(_Frame / (s_UnitsPerSecDuration * 60 * 60)) % (24));
		MinuteDuration = (uint8_t)(int64_t(_Frame / (s_UnitsPerSecDuration * 60)) % (60));
		SecondDuration = (uint8_t)(int64_t(_Frame / s_UnitsPerSecDuration) % 60);
		FrameDuration = (uint8_t)(_Frame % (int64_t)s_UnitsPerSecDuration);

		Day = (uint8_t)(_Frame / s_UnitsPerDay);
		LongHour = (uint8_t)(_Frame / s_UnitsPerDay);
		_Frame = _Frame % s_UnitsPerDay;

		Hour = (uint8_t)(_Frame / s_UnitsPerHour);
		_Frame = _Frame % s_UnitsPerHour;

		Minute = (uint8_t)((_Frame / (s_UnitsPer10Minute)) * 10);
		_Frame = _Frame % s_UnitsPer10Minute;

		if (s_UnitsCorrection != 0)
		{
			uint32_t nbrMinute = _Frame / s_UnitsPerMinute;

			_Frame = _Frame + (nbrMinute * s_UnitsCorrection);

			if ((nbrMinute > 0) && ((_Frame % (s_UnitsPerMinute + s_UnitsCorrection)) < s_UnitsCorrection))
			{
				_Frame = _Frame - s_UnitsCorrection;
			}
		}

		Minute += (uint8_t)(_Frame / (s_UnitsPerMinute + s_UnitsCorrection));
		_Frame = _Frame % (s_UnitsPerMinute + s_UnitsCorrection);

		Second = (uint8_t)(_Frame / s_UnitsPerSec);

		_Frame = _Frame % s_UnitsPerSec;
		Frame = (uint8_t)_Frame;
	}
	bool operator <= (ControlerTimeCode &rhs) const
	{
		return ToFrame() <= rhs.ToFrame();
	}
	bool operator < (ControlerTimeCode &rhs) const
	{
		return ToFrame() < rhs.ToFrame();
	}
	bool operator >= (ControlerTimeCode &rhs) const
	{
		return ToFrame() >= rhs.ToFrame();
	}
	bool operator > (ControlerTimeCode &rhs) const
	{
		return ToFrame() > rhs.ToFrame();
	}
	bool operator == (ControlerTimeCode &rhs) const
	{
		return ToFrame() == rhs.ToFrame();
	}
private:
	double s_UnitsPerSecDuration = 25;
	uint32_t s_UnitsPerSec = 25;
	uint32_t s_UnitsPerMinute = 25 * 60;
	uint32_t s_UnitsPer10Minute = 25 * 60 * 10;
	uint32_t s_UnitsPerHour = 25 * 60 * 60;
	uint32_t s_UnitsPerDay = 25 * 24 * 60 * 60;
	uint32_t s_UnitsCorrection = 0;

public:

	int LongHour;
	unsigned char Day;
	unsigned char HourDuration;
	unsigned char MinuteDuration;
	unsigned char SecondDuration;
	unsigned char FrameDuration;
	int Hour;
	int Minute;
	int Second;
	int Frame;
};
