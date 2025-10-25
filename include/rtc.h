#ifndef RTC_H
#define RTC_H

#include <RtcDS1302.h>

void initRTC();
uint64_t getUnixTime();

#endif