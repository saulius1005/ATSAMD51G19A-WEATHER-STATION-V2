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
    uint8_t RTC_time_zone;
    uint8_t RTC_year;
    uint8_t RTC_month;
    uint8_t RTC_day;
    uint8_t RTC_hour;
    uint8_t RTC_minute;
    uint8_t RTC_second;
    
    uint32_t RTC_sys_time;
    
    RTC_time_update_status_t time_sync;
    
    uint8_t last_known_year; //last known date and time also need to be saved before turn off
    uint8_t last_known_month;
    uint8_t last_known_day;
    uint8_t last_known_hour;
    uint8_t last_known_minute;
    uint8_t last_known_second;
    
} RTC_calendar_t;



extern RTC_calendar_t RTC_Date_and_Time;


void RTC_init_calendar(); //initialization of RTC as calendar

uint32_t RTC_read_sys_time(); //reading calendar data as one 32bit value (formated)

void RTC_read_date_and_time(); //converts formated calendar data to readable date and time

uint32_t datetime_to_RTC_format(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t s); //converts readable date and time into formated 32 bit value

void RTC_date_and_time_update(); //update rtc time with GSM or GNSS data

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

