#pragma once
#include <string>

struct Timecode
{
	char hour;
	char minute;
	char second;
	char frame;

	Timecode();
	Timecode(char _hour, char _minute, char _second, char _frame);

	void			increase(char _maxFrame, bool _isDrop);
	void			decrease(char _maxFrame, int _nOffset, bool _isDrop);
	void			decrease(char _maxFrame, bool _isDrop);
	int64_t			compare(const Timecode& _tc) const;
	bool			operator==(const Timecode& _tc) const;
	bool			operator!=(const Timecode& _tc) const;
	void			setTC(char _hour, char _minute, char _second, char _frame);
	void			setTC(const Timecode& _tc);
	std::string		string(bool _isDrop) const;
	static Timecode make_tc(char _hour, char _minute, char _second, char _frame);
	void			reset();
};
