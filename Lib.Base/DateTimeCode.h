#pragma once
#include <cstdint>
#include "timecode.h"
#include "Date.h"
#include <cmath>

struct DateTimeCode
{
	Date date;
	Timecode tc;

	DateTimeCode() = default;

	DateTimeCode(int16_t _year, int8_t _month, int8_t _day, int8_t _hour, int8_t _minute, int8_t _second, int8_t _frame)
	{
		date.year = _year;
		date.month = _month;
		date.day = _day;
		tc.hour = _hour;
		tc.minute = _minute;
		tc.second = _second;
		tc.frame = _frame;
	}

	DateTimeCode(const Date& _date, int8_t _hour, int8_t _minute, int8_t _second, int8_t _frame)
	{
		date = _date;
		tc.hour = _hour;
		tc.minute = _minute;
		tc.second = _second;
		tc.frame = _frame;
	}

	static DateTimeCode make_dateTimeCode(int16_t _year, int8_t _month, int8_t _day, int8_t _hour, int8_t _minute, int8_t _second, int8_t _frame)
	{
		DateTimeCode dateTimecode;
		dateTimecode.date.year = _year;
		dateTimecode.date.month = _month;
		dateTimecode.date.day = _day;
		dateTimecode.tc.hour = _hour;
		dateTimecode.tc.minute = _minute;
		dateTimecode.tc.second = _second;
		dateTimecode.tc.frame = _frame;
		return dateTimecode;
	}

	static DateTimeCode make_dateTimeCode(const Date& _date, int8_t _hour, int8_t _minute, int8_t _second, int8_t _frame)
	{
		DateTimeCode dateTimecode;
		dateTimecode.date = _date;
		dateTimecode.tc.hour = _hour;
		dateTimecode.tc.minute = _minute;
		dateTimecode.tc.second = _second;
		dateTimecode.tc.frame = _frame;
		return dateTimecode;
	}

	uint64_t to_uint64() const
	{
		return uint64_t(date.year) << 48 |
			uint64_t(date.month) << 40 |
			uint64_t(date.day) << 32 |
			uint64_t(tc.hour) << 24 |
			uint64_t(tc.minute) << 16 |
			uint64_t(tc.second) << 8 |
			uint64_t(tc.frame);
	}

	bool operator<(const DateTimeCode& _rhs) const
	{
		return to_uint64() < _rhs.to_uint64();
	}

	bool operator<=(const DateTimeCode& _rhs) const
	{
		return to_uint64() <= _rhs.to_uint64();
	}

	bool operator>(const DateTimeCode& _rhs) const
	{
		return to_uint64() > _rhs.to_uint64();
	}

	bool operator>=(const DateTimeCode& _rhs) const
	{
		return to_uint64() > _rhs.to_uint64();
	}

	bool operator==(const DateTimeCode& _rhs) const
	{
		return to_uint64() == _rhs.to_uint64();
	}

	bool operator!=(const DateTimeCode& _rhs) const
	{
		return to_uint64() != _rhs.to_uint64();
	}

	std::string string(float _fps) const
	{
		const bool is_drop_ntsc =
			std::fabs(_fps - 23.98) < 0.01 ||
			std::fabs(_fps - 29.97) < 0.01 ||
			std::fabs(_fps - 59.94) < 0.01;
		char buffer[100];
		sprintf_s(buffer, "%s %s", date.string().c_str(), tc.string(is_drop_ntsc).c_str());
		return buffer;
	}
};
