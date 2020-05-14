#pragma once
#include <cstdint>

struct TimeCodeWithDot
{
	int8_t	hour = 0;
	int8_t	minute = 0;
	int8_t	second = 0;
	int8_t	frame = 0;
	int8_t	dot = 0;
	int32_t	part_frame = 0;//[0, 600[

	std::string string(bool _isDrop) const
	{
		char tc[20];
		sprintf_s(tc, "%2.2d:%2.2d:%2.2d%s%2.2d%s", hour, minute, second, _isDrop ? ";" : ":", frame, dot == 0 ? " " : ".");
		return tc;
	}
};
