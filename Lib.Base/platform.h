#pragma once
#include <cstdint>
#ifndef _MSC_VER
#define MAX_PATH 260
#endif 

using cpuFreq = uint64_t;
using cpuTick = uint64_t;

void getCPUfreq(cpuFreq& _freq);
void getTickCount(cpuTick& _tick);
cpuTick getTickCount();


