/* 
 * File:   RTCVar.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, kovas 26, 22.37
 */

#ifndef RTCVAR_H
#define	RTCVAR_H

#include "RTC.h"


#ifdef	__cplusplus
extern "C" {
#endif

RTC_calendar_t RTC_Date_and_Time = {
    .RTC_time_zone = TIME_ZONE,
    .RTC_year = 26,
    .RTC_month = 4,
    .RTC_day = 16,
    .RTC_hour = 14,
    .RTC_minute = 5,
    .RTC_second = 20,
    
    .RTC_sys_time = 0,
    
    .time_sync = NONE, //time is not syncronized
    .last_known_year = 26,
    .last_known_month = 4,
    .last_known_day = 28,
    .last_known_hour = 0,
    .last_known_minute = 7,
    .last_known_second = 30,
};


#ifdef	__cplusplus
}
#endif

#endif	/* RTCVAR_H */

