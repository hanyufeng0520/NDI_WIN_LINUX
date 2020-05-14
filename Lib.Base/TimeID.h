#pragma once
#include <cstdint>
#include "timecode.h"

using TimeIDFrame = uint64_t;
static constexpr uint64_t TIC = 240000;
struct TimeID final
{
private:
	uint32_t m_unitsPerSec = 25;
	uint32_t m_unitsPerMinute = 25 * 60;
	uint32_t m_unitsPer10Minute = 25 * 60 * 10;
	uint32_t m_unitsPerHour = 25 * 60 * 60;
	uint32_t m_unitsPerDay = 25 * 24 * 60 * 60;
	uint32_t m_unitsCorrection = 0;
	uint32_t m_decimalStep = TIC / 25;
	bool     m_drop_ntsc = false;
	Timecode m_last_tc;
	float	 m_fps = 25;
	int64_t  m_TIC_per_day = 0;

public:
	uint32_t	Days = 0; //since 2019-01-01
	uint32_t	Hour = 0;
	uint32_t	Minute = 0;
	uint32_t	Second = 0;
	uint32_t	Decimal = 0;
	uint8_t		Session = 0;

	TimeID() = default;
	TimeID(uint32_t Days,
		uint32_t Hour,
		uint32_t Minute,
		uint32_t Second,
		uint32_t Decimal,
		uint8_t Session);
	~TimeID() = default;

	void set(uint32_t Days,
		uint32_t Hour,
		uint32_t Minute,
		uint32_t Second,
		uint32_t Decimal,
		uint8_t Session);
	void		set_fps(float _fps);
	float		get_fps() const;
	bool		update_timecode_with_log(int camera, const Timecode& _tc, bool _isDrop);
	bool		update_timecode(const Timecode& _tc);
	void		update_last_timecode(const Timecode& _tc);

	TimeIDFrame	to_uint64() const;
	void		from_uint64(TimeIDFrame _Frame);
	std::string string() const;
	std::wstring wstring() const;
	uint32_t	get_step() const;
	uint32_t	get_units_per_day() const;
	void		offset_frame(int64_t _frame_offset);

	bool		operator<=(const TimeID& rhs) const;
	bool		operator<(const TimeID& rhs) const;
	bool		operator>=(const TimeID& rhs) const;
	bool		operator>(const TimeID& rhs) const;
	bool		operator==(const TimeID& rhs) const;
	bool		operator!=(const TimeID& rhs) const;
	TimeID		operator+(const int64_t& frame_offset) const;
	friend int64_t operator-(const TimeID& lhs, const TimeID& rhs);
};
