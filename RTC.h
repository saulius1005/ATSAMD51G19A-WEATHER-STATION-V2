/* 
 * File:   RTC.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, kovas 26, 20.11
 */

#ifndef RTC_H
#define	RTC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define STARTING_LEAP_YEAR 24 //2024 year if now it is 2026 so in rtc register need to write 2 (26-24 = 2)
    
#define RTC_GET_YEAR(x)   (((x) >> 26) & 0x3F)
#define RTC_GET_MONTH(x)  (((x) >> 22) & 0x0F)
#define RTC_GET_DAY(x)    (((x) >> 17) & 0x1F)
#define RTC_GET_HOUR(x)   (((x) >> 12) & 0x1F)
#define RTC_GET_MIN(x)    (((x) >> 6)  & 0x3F)
#define RTC_GET_SEC(x)    ((x) & 0x3F)

typedef enum {
    NONE = 0,
    GSM_sync,
    GNSS_sync
} RTC_time_update_status_t;    
    
typedef struct {
    volatile uint8_t RTC_year;
    volatile uint8_t RTC_month;
    volatile uint8_t RTC_day;
    volatile uint8_t RTC_hour;
    volatile uint8_t RTC_minute;
    volatile uint8_t RTC_second;
    
    volatile uint32_t RTC_sys_time;
    
    RTC_time_update_status_t time_sync;
    
} RTC_calendar_t;



extern RTC_calendar_t RTC_Date_and_Time;

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

