#include "timecode.h"

Timecode::Timecode()
{
	reset();
}

Timecode::Timecode(char _hour, char _minute, char _second, char _frame)
	:hour(_hour), minute(_minute), second(_second), frame(_frame)
{
}

void Timecode::increase(char _maxFrame, bool _isDrop)
{
	if (++frame == _maxFrame)
	{
		frame = 0;
		if (++second == 60)
		{
			second = 0;
			if (++minute == 60)
			{
				minute = 0;
				if (++hour == 24)
					hour = 0;
			}
			else if (_isDrop && (_maxFrame == 30 || _maxFrame == 60) && (minute % 10))
				frame = _maxFrame == 30 ? 2 : 4;
		}
	}
}

void Timecode::decrease(char _maxFrame, int _nOffset, bool _isDrop)
{
	for (int i = 0; i < _nOffset; i++)
		decrease(_maxFrame, _isDrop);
}

void Timecode::decrease(char _maxFrame, bool _isDrop)
{
	if ((_isDrop && ((_maxFrame == 30 && frame == 2) || (_maxFrame == 60 && frame == 4)) && minute % 10) || frame == 0)
	{
		frame = _maxFrame - 1;
		if (second-- == 0)
		{
			second = 59;
			if (minute-- == 0)
			{
				minute = 59;
				if (hour-- == 0)
					hour = 23;
			}
		}
	}
	else
		--frame;
}

int64_t Timecode::compare(const Timecode& _tc) const
{
	uint64_t  tempH = hour, tempM = minute, tempS = second;
	uint64_t  oldTime = (tempH << 24) | (tempM << 16) | (tempS << 8) | frame;

	tempH = _tc.hour; tempM = _tc.minute; tempS = _tc.second;
	uint64_t  newTime = (tempH << 24) | (tempM << 16) | (tempS << 8) | _tc.frame;

	return newTime - oldTime;
}

void Timecode::setTC(char _hour, char _minute, char _second, char _frame)
{
	hour = _hour;
	minute = _minute;
	second = _second;
	frame = _frame;
}

void Timecode::setTC(const Timecode& _tc)
{
	hour = _tc.hour;
	minute = _tc.minute;
	second = _tc.second;
	frame = _tc.frame;
}

std::string Timecode::string(bool _isDrop) const
{
	char tc[20];
	sprintf_s(tc, "%2.2d:%2.2d:%2.2d%s%2.2d ", hour, minute, second, _isDrop ? ";" : ":", frame);
	return tc;
}

Timecode Timecode::make_tc(char _hour, char _minute, char _second, char _frame)
{
	return Timecode(_hour, _minute, _second, _frame);
}

void Timecode::reset()
{
	memset(this, 0, sizeof(Timecode));
}

bool Timecode::operator!=(const Timecode& _tc) const
{
	return hour != _tc.hour || minute != _tc.minute || second != _tc.second || frame != _tc.frame;
}

bool Timecode::operator==(const Timecode& _tc) const
{
	return hour == _tc.hour && minute == _tc.minute && second == _tc.second && frame == _tc.frame;
}