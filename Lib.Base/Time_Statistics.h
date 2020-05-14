#pragma once
#include "ClockTimer.h"
struct Time_Statistics
{
	ClockTimer* pTimeCount = nullptr;
	float* fTimeArr = nullptr;
	void LogTime(int ndex) { if (this && this->fTimeArr && this->pTimeCount) { this->fTimeArr[ndex] = this->pTimeCount->elapse_ms(); } }
};