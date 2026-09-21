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


A7672E_network_settings_t A7672E_NET = {
    .etester = 0,
    .APN_USR = "internet.tele2.lt",
    .TRST_PHN = {0},
    .SERVER_URL = "https://script.google.com/macros/s/AKfycbxW9kfqiDz7xPEY7nezEQJfBDZv7C2zvVsNZpeZZZ7saAG8cegEyoPcK8lXMch-T0Zs/exec?data=",
    .towers_in_total = 2,
    .latitude = 551234,
    .longitude = 249876,
    .altitude = 112,
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
    .state = SET_DEVICE,
    .APN = {0},
};

A7672E_gnss_gsm_calendar_t A7672EGSM = {
    .GSM_year = 0,
    .GSM_month = 0,
    .GSM_day = 0,
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
    .GNSS_time_corect = false,
    
};

A7672E_work_list_t A7672E_work = {
    .state = SET_DEVICE,
 
};

A7672E_gsm_status_t A7672E_GSM_STATUS = {
    .rssi = 0,
    .ber = 0,
    .bad_signal = false,
    .reg_status = 0,
    .n = 0,
};

CommandList_t A7672ESetGPSList[] = { //most command response time is max 9sec
    {"AT+CGNSSPWR=1,1,1\r", 9, 2}, //turn on gnss module,hot flash start, use dynamic flash    
    {"AT+CGNSSPROD\r", 9, 2}, //get gnss module info only after power on module
    {"AT+CGPSCOLD\r", 1, 1}, //cold start
    //AT+CGNSSTST=1 //send data to nmea port
    {"AT+CGNSSMODE=3\r", 1, 1}, //GPS+GLONASS+GALILEO+SBAS+QZSS (1,2,3,4)
    {"AT+CGPSNMEARATE=1\r", 1, 1}, //update rate 1 hz (1,2,5)
    {"AT+CAGPS\r", 10, 2}, //get data from assistant agnss server
};

CommandList_t A7672ESetInternetList[] = { //cmd list for http initialization
    {"AT\r", 1, 1},
    {"AT+CSQ\r", 1, 2},//check signal strength max response time 9000ms
    {"AT+CREG?\r", 1, 2},//check network registration
    {"AT+CPSI?\r", 1, 2}, //check network details
    //{"AT+CMGF=1\r", 1, 1}, //turn on sms function
   // {"AT+CPMS?\r", 2, 1}, //check selected storages
    //{"AT+CMGD=1,4\r", 5, 1}, //delete all sms
    //{"AT+CMGS=\"+3.......213\"\r", 1, 10}, //laukiu < ar > þodþiu kaþkurio simbolio
    //{"testas ið gsm modulio\x1A\r", 1, 5},
    {A7672E_init.APN, 2, 1},//set apn //cid1, IP, apn: internet.tele2.lt
    {"AT+CGACT?\r",2,1},
    {"AT+CGACT=1,1\r", 2, 1},    //cid 2
       
    {"AT+CGPADDR=1\r",1,1},//check cid
    {"AT+HTTPINIT\r", 1, 1},
    {"AT+CTZU=1\r", 1, 1}, //Enable automatic time and time zone update via NITZ   
};
CommandList_t A7672EHTTPGET[] = { //cmd list to receive GET request
    {"AT+HTTPPARA=\"URL\",\"https://script.google.com/macros/s/AKfycbyps-b_D13lfETv7Xzy61zcjw9j0dr2iXX73-Ci4nFodykgA0Zumc_FTrwwkiHbnfF4/exec?\"\r", 1, 1}, //test url
    {"AT+HTTPACTION=0\r", 1, 20},
   // {"AT+HTTPHEAD\r", 1, 1},
    {"AT+HTTPTERM\r", 1, 1},
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

A7672E_message_request_t A7672E_ASKER = {
    .GSM_TIME_CMD = {
        .name = GSM_TIME,
        //.cmd = "AT+CCLK?\r",
        .cmd = "+CCLK?",
        .echo = "+CCLK: ",
        .sent = false,
    },
    .GNSS_TIME_CMD = {
        .name = GNSS_TIME,
        //.cmd = "AT+CGNSSINFO\r",
        .cmd = "+CGNSSINFO",
        .echo = "+CGNSSINFO: ",
        .sent = false,
    },
    .RSSI_CMD = {
        .name = RSSI_SIG,
        //.cmd = "AT+CSQ\r",
        .cmd = "+CSQ",
        .echo = "+CSQ: ",
        .sent = false,
    },
    .REGISTRATION_CMD = {
        .name = NET_REG,
        //.cmd = "AT+CREG?\r",
        .cmd = "+CREG?",
        .echo = "+CREG: ",
        .sent = false,
    },
    .SERVER_CMD = {
        .cmd = "AT+HTTPPARA=\"URL\",\"",
    },
};

#ifdef	__cplusplus
}
#endif

#endif	/* A7672EVAR_H */

