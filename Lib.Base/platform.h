#pragma once
#include <cstdint>

using cpuFreq = uint64_t;
using cpuTick = uint64_t;

void getCPUfreq(cpuFreq& _freq);
void getTickCount(cpuTick& _tick);
cpuTick getTickCount();


