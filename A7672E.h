/* 
 * File:   A7672E.h
 * Author: Saulius
 *
 * Created on Ðeðtadienis, 2026, kovas 21, 16.13
 */

#ifndef A7672E_H
#define	A7672E_H

#ifdef	__cplusplus
extern "C" {
#endif

#define AT_COMMAND_COUNT(arr) (sizeof(arr) / sizeof((arr)[0])) //calculate command set count
#define TIME_ZONE 2
    
typedef struct {
    char *ATTX; //send command array list      
    uint8_t WaitTimeInSeconds; //answer timeout period in seconds mostly A7672E recommendet to wait 9000ms = 9s
    uint8_t PCKCNT; //how many packets for answer exmpl AT have one- OK, AT+CGNSSPWR=1,1,1 have 2- ok and CGNNS ready
} CommandList_t;

typedef enum {
    GPSINFO = 0,
    GNSSINFO        
}GPSNames_t;

typedef enum {
    INIT = 0,
    HTTPSETUP,
    GNSSSETUP,
    SHOW_FIRST_WINDOW,
    WORK
}A7672Estatus_t;

typedef enum {
    GNSS = 0,
    GSM,
    SHOW_TIME,
}A7672Etime_source_t;

typedef enum {
    SET = 0,
    WAIT,
    DONE
}A7672states_t;

typedef struct {
    uint8_t mode; //Fix mode 2=2D fix 3=3D fix
    uint8_t GPS_SVs;//GPS satellite visible numbers
    uint8_t BEIDOU_SVs; // BEIDOU satellite visible numbers
    uint8_t GLONASS_SVs; // GLONASS satellite visible numbers
    uint8_t GALILEO_SVs; // GALILEO satellite visible numbers
    uint32_t lat; // Latitude of current position.Output format is dd.ddddddd instead of float use fixed point dddddddddd
    char N_S; // N/S Indicator, N=north or S=south.
    uint32_t log; // Longitude of current position. Output format is ddd.ddddddd instead of float use fixed point dddddddddd
    char E_W; // E/W Indicator, E=east or W=west.
    uint32_t date; // Date. Output format is ddmmyy.
    uint32_t UTC_time; // UTC Time. Output format is hhmmss.ss. Instead of float use fixed point hhmmssss
    uint16_t alt; // MSL Altitude. Unit is meters. 
    uint16_t speed; // Speed Over Ground. Unit is knots.
    uint16_t course; // Course. Degrees.
    uint32_t PDOP; // Position Dilution Of Precision
    uint32_t HDOP; // Horizontal Dilution Of Precision
    uint32_t VDOP; // Vertical Dilution Of Precision.
    uint8_t NoSV; // Number of satellites involved in positioning       
    
} GNSS_data_list_t;

typedef struct {
    uint8_t step;
    bool cycle;
    bool enabled;
    A7672Estatus_t status;
    A7672states_t state;
}A7672E_init_list_t;

typedef struct {
    bool cycle;
    A7672Etime_source_t source;
    A7672states_t state;
}A7672E_work_list_t;


typedef struct {
    uint8_t GNSS_year;
    uint8_t GNSS_month;
    uint8_t GNSS_day;
    uint8_t GNSS_hour;
    uint8_t GNSS_minute;
    uint8_t GNSS_second;
    
    uint32_t GNSS_sys_time; //for rtc update
    bool GNSS_time_corect;
    
    uint8_t GSM_year;
    uint8_t GSM_month;
    uint8_t GSM_day;
    uint8_t GSM_hour;
    uint8_t GSM_minute;
    uint8_t GSM_second;
    
    uint32_t GSM_sys_time; //for rtc update
    bool GSM_time_corect;
    
} A7672E_gnss_gsm_calendar_t;

typedef struct { // for cmd list at initialization of http or gnss
    CommandList_t *list; //array for cmd storage
    uint8_t size; // cmd count in total
    const char *header; //title which are showing at initialization
} A7672E_Config_t;

extern CommandList_t A7672ESetGPSList[];
extern CommandList_t A7672ESetInternetList[];
extern CommandList_t A7672EHTTPGET[];
extern GNSS_data_list_t A7672EGNSS;
extern A7672E_gnss_gsm_calendar_t A7672EGSM;
extern A7672E_init_list_t A7672E_init;
extern A7672E_work_list_t A7672E_work;


void A7672EInit(); //initialization of module HTTP, GPS

void A7672ReadNEMAGNSS(); //Reading of time and date data from GSM and from GNSS

void A7672EsendCommandsInit(); //sending at commands from list at a7672var.h

#ifdef	__cplusplus
}
#endif

#endif	/* A7672E_H */

