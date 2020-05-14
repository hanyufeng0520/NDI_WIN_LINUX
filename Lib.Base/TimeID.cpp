#include "TimeID.h"
#include <cmath>
#include "DaysSince20190101.h"

TimeID::TimeID(uint32_t Days, uint32_t Hour, uint32_t Minute, uint32_t Second, uint32_t Decimal, uint8_t Session) :
	Days(Days), Hour(Hour), Minute(Minute), Second(Second), Decimal(Decimal), Session(Session)
{
	set_fps(25);
}

void TimeID::set(uint32_t Days, uint32_t Hour, uint32_t Minute, uint32_t Second, uint32_t Decimal, uint8_t Session)
{
	this->Days = Days;
	this->Hour = Hour;
	this->Minute = Minute;
	this->Second = Second;
	this->Decimal = Decimal;
	this->Session = Session;
}

void TimeID::set_fps(float _fps)
{
	m_fps = _fps;
	if (std::fabs(_fps - 24) < 0.01)
	{
		m_unitsCorrection = 0;
		m_unitsPerSec = 24;
		m_unitsPerMinute = m_unitsPerSec * 60;
		m_unitsPer10Minute = m_unitsPerMinute * 10;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;
	}
	else if (std::fabs(_fps - 25) < 0.01)
	{
		m_unitsCorrection = 0;
		m_unitsPerSec = 25;
		m_unitsPerMinute = m_unitsPerSec * 60;
		m_unitsPer10Minute = m_unitsPerMinute * 10;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;
	}
	else if (std::fabs(_fps - 29.97) < 0.01)
	{
		m_unitsCorrection = 2;
		m_unitsPerSec = 30;
		m_unitsPerMinute = m_unitsPerSec * 60 - m_unitsCorrection;
		m_unitsPer10Minute = m_unitsPerMinute * 10 + m_unitsCorrection;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;
		m_drop_ntsc = true;
	}
	else if (std::fabs(_fps - 30) < 0.01)
	{
		m_unitsCorrection = 0;
		m_unitsPerSec = 30;
		m_unitsPerMinute = m_unitsPerSec * 60;
		m_unitsPer10Minute = m_unitsPerMinute * 10;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;
	}
	else if (std::fabs(_fps - 50) < 0.01)
	{
		m_unitsCorrection = 0;
		m_unitsPerSec = 25;
		m_unitsPerMinute = m_unitsPerSec * 60;
		m_unitsPer10Minute = m_unitsPerMinute * 10;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;

		m_unitsCorrection = m_unitsCorrection * 2;
		m_unitsPerSec = m_unitsPerSec * 2;
		m_unitsPerMinute = m_unitsPerMinute * 2;
		m_unitsPer10Minute = m_unitsPer10Minute * 2;
		m_unitsPerHour = m_unitsPerHour * 2;
		m_unitsPerDay = m_unitsPerDay * 2;
	}
	else if (std::fabs(_fps - 59.94) < 0.01)
	{
		m_unitsCorrection = 2;
		m_unitsPerSec = 30;
		m_unitsPerMinute = m_unitsPerSec * 60 - m_unitsCorrection;
		m_unitsPer10Minute = m_unitsPerMinute * 10 + m_unitsCorrection;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;

		m_unitsCorrection = m_unitsCorrection * 2;
		m_unitsPerSec = m_unitsPerSec * 2;
		m_unitsPerMinute = m_unitsPerMinute * 2;
		m_unitsPer10Minute = m_unitsPer10Minute * 2;
		m_unitsPerHour = m_unitsPerHour * 2;
		m_unitsPerDay = m_unitsPerDay * 2;

		m_drop_ntsc = true;
	}
	else if (std::fabs(_fps - 60) < 0.01)
	{
		m_unitsCorrection = 0;
		m_unitsPerSec = 30;
		m_unitsPerMinute = m_unitsPerSec * 60;
		m_unitsPer10Minute = m_unitsPerMinute * 10;
		m_unitsPerHour = m_unitsPer10Minute * 6;
		m_unitsPerDay = m_unitsPerHour * 24;

		m_unitsCorrection = m_unitsCorrection * 2;
		m_unitsPerSec = m_unitsPerSec * 2;
		m_unitsPerMinute = m_unitsPerMinute * 2;
		m_unitsPer10Minute = m_unitsPer10Minute * 2;
		m_unitsPerHour = m_unitsPerHour * 2;
		m_unitsPerDay = m_unitsPerDay * 2;
	}
	m_decimalStep = TIC / m_unitsPerSec;
	m_TIC_per_day = int64_t(m_unitsPerDay) * m_decimalStep;
}

float TimeID::get_fps() const
{
	return m_fps;
}

bool TimeID::update_timecode_with_log(int camera, const Timecode& _tc, bool _isDrop)
{
	bool jump = false;
	bool cross_day = false;
	if (m_last_tc.hour == 23 && _tc.hour == 0)//over midnight
	{
		Session = 0;
		jump = true;
		cross_day = true;
	}
	else
	{
		m_last_tc.increase(m_unitsPerSec, m_drop_ntsc);
		const int64_t nret = m_last_tc.compare(_tc);
		if (nret < 0)
		{
			++Session;
			jump = true;
		}
		else if (nret > 0)
		{
			jump = true;
		}
	}
	m_last_tc = _tc;

	if (cross_day)
		++Days;
	Hour = _tc.hour;
	Minute = _tc.minute;
	Second = _tc.second;
	Decimal = _tc.frame * m_decimalStep;
	return jump;
}

TimeIDFrame TimeID::to_uint64() const
{
	//|63---------------48|47------40|39-----------------------0|
	//| DaysSince20190101 | session  |    TIC since 00:00:00.00 |

	uint32_t nbr10Minute = uint32_t(Minute) / 10;
	uint32_t nbrMinute = uint32_t(Minute) % 10;

	uint64_t frame = uint64_t(Hour) * m_unitsPerHour
		+ uint64_t(nbr10Minute) * m_unitsPer10Minute
		+ uint64_t(nbrMinute) * m_unitsPerMinute
		+ uint64_t(Second) * m_unitsPerSec
		+ uint64_t(Decimal) / m_decimalStep;

	frame *= m_decimalStep;//change to TIC since 00:00:00.00
	const uint64_t left_decimal = uint64_t(Decimal) % m_decimalStep;
	frame += left_decimal;

	frame |= uint64_t(Days) << 48;
	frame |= uint64_t(Session) << 40;
	return frame;
}

void TimeID::from_uint64(TimeIDFrame _Frame)
{
	Days = (_Frame >> 48) & 0xFFFF;
	Session = (_Frame >> 40) & 0xFF;

	_Frame = _Frame & 0xFFFFFFFFFF;
	const uint64_t left_decimal = _Frame % m_decimalStep;
	_Frame /= m_decimalStep;//change to frame since 00:00:00.00
	_Frame = _Frame % m_unitsPerDay;

	Hour = uint8_t(_Frame / m_unitsPerHour);
	_Frame = _Frame % m_unitsPerHour;

	Minute = uint8_t(_Frame / m_unitsPer10Minute * 10);
	_Frame = _Frame % m_unitsPer10Minute;

	if (m_unitsCorrection != 0)
	{
		uint64_t nbrMinute = _Frame / m_unitsPerMinute;
		_Frame = _Frame + nbrMinute * m_unitsCorrection;
		if (nbrMinute > 0 && _Frame % (m_unitsPerMinute + m_unitsCorrection) < m_unitsCorrection)
			_Frame = _Frame - m_unitsCorrection;
	}

	Minute += uint32_t(_Frame / (m_unitsPerMinute + m_unitsCorrection));
	_Frame = _Frame % (m_unitsPerMinute + m_unitsCorrection);

	Second = uint32_t(_Frame / m_unitsPerSec);

	_Frame = _Frame % m_unitsPerSec;
	Decimal = uint32_t(_Frame) * m_decimalStep + left_decimal;
}

std::string TimeID::string() const
{
	char str[100];
	sprintf_s(str, "%u %u %2.2u:%2.2u:%2.2u:%07u", Days, Session, Hour, Minute, Second, Decimal);
	return str;
}

std::wstring TimeID::wstring() const
{
	wchar_t str[100];
	swprintf_s(str, L"%u %u %2.2u:%2.2u:%2.2u:%07u", Days, Session, Hour, Minute, Second, Decimal);
	return str;
}

uint32_t TimeID::get_step() const
{
	return m_decimalStep;
}

uint32_t TimeID::get_units_per_day() const
{
	return m_unitsPerDay;
}

void TimeID::offset_frame(int64_t _frame_offset)
{
	if (_frame_offset == 0)
		return;
	int64_t day_offset = abs(_frame_offset) / m_unitsPerDay;
	int64_t left_tic = (_frame_offset % m_unitsPerDay) * m_decimalStep;

	TimeID timeID = *this;
	TimeIDFrame oldFrame = timeID.to_uint64();
	int64_t time_part = oldFrame & 0xFFFFFFFFFF;
	if (_frame_offset > 0)
	{
		time_part += left_tic;
		if (time_part >= m_TIC_per_day)
		{
			++day_offset;
			time_part -= m_TIC_per_day;
		}

		TimeIDFrame newID = 0;
		newID |= uint64_t(timeID.Days + day_offset) << 48;
		newID |= uint64_t(timeID.Session) << 40;
		newID |= time_part;

		TimeID time_id;
		time_id.set_fps(m_fps);
		time_id.from_uint64(newID);
		*this = time_id;
	}

	if (_frame_offset < 0)
	{
		time_part += left_tic;
		if (time_part < 0)
		{
			++day_offset;
			time_part += m_TIC_per_day;
		}

		TimeIDFrame newID = 0;
		newID |= uint64_t(timeID.Days - day_offset) << 48;
		newID |= uint64_t(timeID.Session) << 40;
		newID |= time_part;

		TimeID time_id;
		time_id.set_fps(m_fps);
		time_id.from_uint64(newID);
		*this = time_id;
	}
}

bool TimeID::operator<=(const TimeID& rhs) const
{
	return to_uint64() <= rhs.to_uint64();
}

bool TimeID::operator<(const TimeID& rhs) const
{
	return to_uint64() < rhs.to_uint64();
}

bool TimeID::operator>=(const TimeID& rhs) const
{
	return to_uint64() >= rhs.to_uint64();
}

bool TimeID::operator>(const TimeID& rhs) const
{
	return to_uint64() > rhs.to_uint64();
}

bool TimeID::operator==(const TimeID& rhs) const
{
	return to_uint64() == rhs.to_uint64();
}

bool TimeID::operator!=(const TimeID& rhs) const
{
	return to_uint64() != rhs.to_uint64();
}

TimeID TimeID::operator+(const int64_t& frame_offset) const
{
	int64_t day_offset = abs(frame_offset) / m_unitsPerDay;
	const int64_t left_tic = (frame_offset % m_unitsPerDay) * m_decimalStep;

	TimeID timeID = *this;
	const TimeIDFrame oldFrame = timeID.to_uint64();
	int64_t time_part = oldFrame & 0xFFFFFFFFFF;
	if (frame_offset > 0)
	{
		time_part += left_tic;
		while (time_part >= m_TIC_per_day)
		{
			++day_offset;
			time_part -= m_TIC_per_day;
		}

		TimeIDFrame newID = 0;
		newID |= uint64_t(timeID.Days + day_offset) << 48;
		newID |= uint64_t(timeID.Session) << 40;
		newID |= time_part;

		TimeID time_id;
		time_id.set_fps(m_fps);
		time_id.from_uint64(newID);
		return time_id;
	}

	if (frame_offset < 0)
	{
		time_part += left_tic;
		while (time_part < 0)
		{
			++day_offset;
			time_part += m_TIC_per_day;
		}

		TimeIDFrame newID = 0;
		newID |= uint64_t(timeID.Days - day_offset) << 48;
		newID |= uint64_t(timeID.Session) << 40;
		newID |= time_part;

		TimeID time_id;
		time_id.set_fps(m_fps);
		time_id.from_uint64(newID);
		return time_id;
	}

	return timeID;
}

bool TimeID::update_timecode(const Timecode& _tc)
{
	bool jump = false;
	bool cross_day = false;
	if (m_last_tc.hour == 23 && _tc.hour == 0)//over midnight
	{
		Session = 0;
		jump = true;
		cross_day = true;
	}
	else
	{
		m_last_tc.increase(m_unitsPerSec, m_drop_ntsc);
		const int64_t nret = m_last_tc.compare(_tc);
		if (nret < 0)
		{
			++Session;
			jump = true;
		}
		else if (nret > 0)
			jump = true;
	}
	m_last_tc = _tc;

	if (cross_day)
		++Days;
	Hour = _tc.hour;
	Minute = _tc.minute;
	Second = _tc.second;
	Decimal = _tc.frame * m_decimalStep;
	return jump;
}

void TimeID::update_last_timecode(const Timecode& _tc)
{
	m_last_tc = _tc;
}

int64_t operator-(const TimeID& lhs, const TimeID& rhs)
{
	int64_t day_gap = int64_t(lhs.Days) - rhs.Days;
	int64_t frame_gap = (lhs.to_uint64() & 0xFFFFFFFFFF) / lhs.get_step() - (rhs.to_uint64() & 0xFFFFFFFFFF) / rhs.get_step();
	return  frame_gap + day_gap * lhs.m_unitsPerDay;
}