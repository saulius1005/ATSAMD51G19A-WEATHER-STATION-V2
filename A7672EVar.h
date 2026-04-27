/* 
 * File:   A7672EVar.h
 * Author: Saulius
 *
 * Created on Ðeðtadienis, 2026, kovas 21, 16.13
 */

#ifndef A7672EVAR_H
#define	A7672EVAR_H

#include "A7672E.h"


#ifdef	__cplusplus
extern "C" {
#endif

CommandList_t A7672ESetGPSList[] = { //most command response time is max 9sec
    {"AT+CGNSSPWR=1,1,1\r", 9, 2}, //turn on gnss module,hot flash start, use dynamic flash
    {"AT+CGPSCOLD\r", 1, 1}, //cold start
    {"AT+CGNSSMODE=3\r", 1, 1}, //GPS+GLONASS+GALILEO+SBAS+QZSS (1,2,3,4)
    {"AT+CGPSNMEARATE=1\r", 1, 1}, //update rate 1 hz (1,2,5)
    {"AT+CAGPS\r", 5, 2}, //get data from assistant agnss server
};

CommandList_t A7672ESetInternetList[] = { //cmd list for http initialization
    {"AT\r", 1, 1},
    {"AT+CSQ\r", 1, 2},//check signal strength max response time 9000ms
    {"AT+CREG?\r", 1, 2},//check network registration
    {"AT+CPSI?\r", 1, 2}, //check network details
    {"AT+CGDCONT=1,\"IP\",\"internet.tele2.lt\"\r", 1, 1},//set apn
    {"AT+CGACT=1,1\r", 1, 1},    
    {"AT+HTTPINIT\r", 1, 1},
    {"AT+CTZU=1\r", 1, 1}, //Enable automatic time and time zone update via NITZ
    
};
CommandList_t A7672EHTTPGET[] = { //cmd list to receive GET request
    {"AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=8KEYNF1KHFESA11N&field1=181.8&field2=25.59&field3=14&field4=4&field5=518&field6=1025&field7=22.7&field8=61\"\r", 1, 1}, //test url
    {"AT+HTTPACTION=0\r", 1, 20},
   // {"AT+HTTPHEAD\r", 1, 1},
    {"AT+HTTPTERM\r", 1, 1},
};

GNSS_data_list_t A7672EGNSS = {
    .date = 0,
    .alt = 0,
    .UTC_time = 0, 
    .mode = 0,
};

A7672E_init_list_t A7672E_init = {
    .cycle = false,
    .enabled = false,
    .step = 0,
    .status = INIT,
    .state = SET,
};

A7672E_gnss_gsm_calendar_t A7672EGSM = {
    .GSM_year = 26,
    .GSM_month = 4,
    .GSM_day = 28,
    .GSM_hour = 0,
    .GSM_minute = 0,
    .GSM_second = 0,
    
    .GSM_sys_time = 0,
    .GSM_time_corect = false,
    
    .GNSS_year = 0,
    .GNSS_month = 0,
    .GNSS_day = 0,
    .GNSS_hour = 0,
    .GNSS_minute = 0,
    .GNSS_second = 0,    
    
    .GNSS_sys_time = 0,
    .GNSS_time_corect = 0,
    
};

A7672E_Config_t http_cfg = {
    .list = A7672ESetInternetList,
    .size = AT_COMMAND_COUNT(A7672ESetInternetList),
    .header = "SETING UP HTTP"
};

A7672E_Config_t gps_cfg = {
    .list = A7672ESetGPSList,
    .size = AT_COMMAND_COUNT(A7672ESetGPSList),
    .header = "SETING UP GPS"
};

A7672E_work_list_t A7672E_work = {
    .cycle = false,
    .state = SET,
    .source = GNSS,
 
};

#ifdef	__cplusplus
}
#endif

#endif	/* A7672EVAR_H */

