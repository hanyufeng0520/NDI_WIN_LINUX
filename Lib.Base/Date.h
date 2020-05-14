#pragma once
#include <cstdint>
#include <string>

struct Date
{
	int16_t	year = 1970;
	int8_t	month = 1;
	int8_t	day = 1;

	Date() = default;

	Date(int16_t _year, int8_t _month, int8_t _day)
	{
		year = _year;
		month = _month;
		day = _day;
	}

	static Date make_date(int16_t _year, int8_t _month, int8_t _day)
	{
		Date date;
		date.year = _year;
		date.month = _month;
		date.day = _day;
		return date;
	}

	std::string string() const
	{
		char tc[50];
		sprintf_s(tc, "%04d/%02d/%02d", year, month, day);
		return tc;
	}
};
