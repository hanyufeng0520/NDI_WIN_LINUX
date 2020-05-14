#pragma once
#include "Lib.Base/Date.h"
#include "Lib.Base/TimeCodeWithDot.h"
#include <cmath>

struct DateTimeCodeWithDot
{
	Date			date;
	TimeCodeWithDot tc;

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

	static DateTimeCodeWithDot make_dateTimeCode(const Date& _date, int8_t _hour, int8_t _minute, int8_t _second, int8_t _frame, uint8_t _dot, int32_t _part_frame)
	{
		DateTimeCodeWithDot dateTimecode;
		dateTimecode.date = _date;
		dateTimecode.tc.hour = _hour;
		dateTimecode.tc.minute = _minute;
		dateTimecode.tc.second = _second;
		dateTimecode.tc.frame = _frame;
		dateTimecode.tc.dot = _dot;
		dateTimecode.tc.part_frame = _part_frame;
		return dateTimecode;
	}
};
