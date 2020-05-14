#pragma once
#include <ctime>
#include <cstdint>
#include <optional>
#include <string>
#include "DateTimeCode.h"

class DaysSince20190101
{
	__time64_t m_time_t_20190101;

protected:
	DaysSince20190101()
	{
		tm tm20190101 = { 0, 0, 0, 1, 0, 2019 - 1900 };
		m_time_t_20190101 = _mktime64(&tm20190101);
	}
	virtual ~DaysSince20190101() = default;

public:
	static DaysSince20190101& instance()
	{
		static DaysSince20190101 instance;
		return instance;
	}

	uint32_t days() const
	{
		__time64_t result = _time64(nullptr);
		double difference = _difftime64(result, m_time_t_20190101) / (60.0 * 60 * 24);
		return difference;
	}

	std::optional<std::string> local_time_string(uint32_t _days) const
	{
		__time64_t calendar_time = __time64_t(_days) * (60 * 60 * 24) + m_time_t_20190101;
		tm time;
		if (_localtime64_s(&time, &calendar_time) == 0)
		{
			char buffer[20];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d", &time);
			return buffer;
		}
		return std::nullopt;
	}

	Date get_date_from_days(uint32_t _days) const
	{
		__time64_t calendar_time = __time64_t(_days) * (60 * 60 * 24) + m_time_t_20190101;
		tm time;
		if (_localtime64_s(&time, &calendar_time) == 0)
			return Date::make_date(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
		return Date::make_date(2019, 1, 1);
	}

	uint32_t get_days_from_date(const Date& _date) const
	{
		tm time = { 0, 0, 0, _date.day, _date.month - 1, _date.year - 1900 };
		__time64_t time_int64 = _mktime64(&time);
		double difference = _difftime64(time_int64, m_time_t_20190101) / (60.0 * 60 * 24);
		return difference;
	}
};
