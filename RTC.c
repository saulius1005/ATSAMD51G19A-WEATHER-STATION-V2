#include "settings.h"
#include "RTCVar.h"

void RTC_init_calendar() {

    RTC_REGS->MODE2.RTC_CTRLA &= ~RTC_MODE2_CTRLA_ENABLE_Msk;
    while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_ENABLE_Msk); //turn off rtc
           
    OSC32KCTRL_REGS->OSC32KCTRL_RTCCTRL = OSC32KCTRL_RTCCTRL_RTCSEL_ULP1K;// Select RTC clock source
    
    RTC_REGS->MODE2.RTC_CLOCK = RTC_MODE2_CLOCK_YEAR(2) | RTC_MODE2_CLOCK_MONTH(4) | RTC_MODE2_CLOCK_DAY(15) | RTC_MODE2_CLOCK_HOUR(22) | RTC_MODE2_CLOCK_MINUTE(46) | RTC_MODE2_CLOCK_SECOND(15); //2026-2024(leap year) = 2
    while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_CLOCKSYNC_Msk);
    
    RTC_REGS->MODE2.RTC_CTRLA = RTC_MODE2_CTRLA_MODE_CLOCK | RTC_MODE2_CTRLA_CLKREP(0) |  RTC_MODE2_CTRLA_PRESCALER_DIV1024 | RTC_MODE2_CTRLA_CLOCKSYNC_Msk;//calendar mode, 24Hour mode, clear on match, 1.024khz/1024 = 1Hz
      
    RTC_REGS->MODE2.RTC_CTRLA |= RTC_MODE2_CTRLA_ENABLE_Msk;
    while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_ENABLE_Msk); //enable calendar
}

uint32_t RTC_read_sys_time(){ //one value
    return RTC_REGS->MODE2.RTC_CLOCK;
}

void RTC_read_date_and_time(){
    uint32_t clock = RTC_read_sys_time();
    RTC_Date_and_Time.RTC_year = STARTING_LEAP_YEAR + ((clock & RTC_MODE2_CLOCK_YEAR_Msk) >> RTC_MODE2_CLOCK_YEAR_Pos); //use 2024 leap years for base 
    RTC_Date_and_Time.RTC_month = (clock & RTC_MODE2_CLOCK_MONTH_Msk) >> RTC_MODE2_CLOCK_MONTH_Pos; 
    RTC_Date_and_Time.RTC_day = (clock & RTC_MODE2_CLOCK_DAY_Msk) >> RTC_MODE2_CLOCK_DAY_Pos;
    RTC_Date_and_Time.RTC_hour = (clock & RTC_MODE2_CLOCK_HOUR_Msk) >> RTC_MODE2_CLOCK_HOUR_Pos;
    RTC_Date_and_Time.RTC_minute = (clock & RTC_MODE2_CLOCK_MINUTE_Msk) >> RTC_MODE2_CLOCK_MINUTE_Pos;
    RTC_Date_and_Time.RTC_second = (clock & RTC_MODE2_CLOCK_SECOND_Msk) >> RTC_MODE2_CLOCK_SECOND_Pos;
}

uint32_t datetime_to_rtc_format(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t s){
    if(y < 26) //if year is less than 2026 it means gsm time is default or 0, same with gnss
        return 0;
    return (((uint32_t)(y - STARTING_LEAP_YEAR) & 0x3F) << 26) | ((uint32_t)(m   & 0x0F) << 22) | ((uint32_t)(d   & 0x1F) << 17) | ((uint32_t)(h   & 0x1F) << 12) | ((uint32_t)(min & 0x3F) << 6)  | ((uint32_t)(s   & 0x3F));
}

void RTC_date_and_time_update(){

    if(RTC_Date_and_Time.time_sync == GNSS_sync) //if rtc is synced with gnss skip further code
        return;
    
    if((A7672EGSM.GNSS_sys_time != RTC_Date_and_Time.RTC_sys_time) && (A7672EGNSS.mode != 0)){ //use gsm time sync only if gnss is not available
            RTC_REGS->MODE2.RTC_CTRLA &= ~RTC_MODE2_CTRLA_ENABLE_Msk; //turn off rtc
        while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_ENABLE_Msk); //wait sync
            RTC_REGS->MODE2.RTC_CLOCK = A7672EGSM.GNSS_sys_time; //write whole value (already fitted using datetime_to_rtc_format() function)
            //RTC_REGS->MODE2.RTC_CLOCK = RTC_MODE2_CLOCK_YEAR(A7672EGSM.GSM_year - STARTING_LEAP_YEAR) | RTC_MODE2_CLOCK_MONTH(A7672EGSM.GSM_month) | RTC_MODE2_CLOCK_DAY(A7672EGSM.GSM_day) | RTC_MODE2_CLOCK_HOUR(A7672EGSM.GSM_hour) | RTC_MODE2_CLOCK_MINUTE(A7672EGSM.GSM_minute) | RTC_MODE2_CLOCK_SECOND(A7672EGSM.GSM_second);
        while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_CLOCKSYNC_Msk);// wait sync
            RTC_REGS->MODE2.RTC_CTRLA |= RTC_MODE2_CTRLA_ENABLE_Msk;
        while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_ENABLE_Msk); //enable calendar
        RTC_Date_and_Time.time_sync = GNSS_sync; //change sync status
        return; //skip gsm time sync
    }
        
    if((A7672EGSM.GSM_sys_time > RTC_Date_and_Time.RTC_sys_time) && (RTC_Date_and_Time.time_sync == NONE)){ //use gsm time sync only if gnss is not available
            RTC_REGS->MODE2.RTC_CTRLA &= ~RTC_MODE2_CTRLA_ENABLE_Msk; //turn off rtc
        while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_ENABLE_Msk); //wait sync
            RTC_REGS->MODE2.RTC_CLOCK = A7672EGSM.GSM_sys_time; //write whole value (already fitted using datetime_to_rtc_format() function)
            //RTC_REGS->MODE2.RTC_CLOCK = RTC_MODE2_CLOCK_YEAR(A7672EGSM.GSM_year - STARTING_LEAP_YEAR) | RTC_MODE2_CLOCK_MONTH(A7672EGSM.GSM_month) | RTC_MODE2_CLOCK_DAY(A7672EGSM.GSM_day) | RTC_MODE2_CLOCK_HOUR(A7672EGSM.GSM_hour) | RTC_MODE2_CLOCK_MINUTE(A7672EGSM.GSM_minute) | RTC_MODE2_CLOCK_SECOND(A7672EGSM.GSM_second);
        while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_CLOCKSYNC_Msk);// wait sync
            RTC_REGS->MODE2.RTC_CTRLA |= RTC_MODE2_CTRLA_ENABLE_Msk;
        while (RTC_REGS->MODE2.RTC_SYNCBUSY & RTC_MODE2_SYNCBUSY_ENABLE_Msk); //enable calendar
        RTC_Date_and_Time.time_sync = GSM_sync; //change sync status
    }
}