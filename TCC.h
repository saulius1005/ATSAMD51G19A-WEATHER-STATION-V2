/* 
 * File:   TCC.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, rugpjûtis 3, 09.14
 */

#ifndef TCC_H
#define	TCC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define GSM_GNSS_UPDATE_PERIOD 250//GSM and GNSS time sync checking interval
#define GSM_UPDATE_PERIOD 250 //rssi reg status check interval
#define TOWERS_UPDATE_PERIOD 5000 //towers data exchange interval
#define SERVER_UPDATE_PERIOD 8000 //data send to server interval

    
typedef enum {
    PREPARED = 0,
    UPDATING,
    UPDATED
}update_status_t;

typedef struct {
    uint16_t update_time;
    uint16_t respond_time;
    update_status_t update_stat;   
    uint16_t start_at;
}regular_update_param_t;

typedef struct {
    regular_update_param_t GSM;
    regular_update_param_t TIME;
    regular_update_param_t SERVER;
    regular_update_param_t TOWERS;
}regular_update_devices_t;


typedef struct {
    uint16_t period_counter;
    uint8_t GSM_GNSS_update;
    bool GSM_GNSS_update_flag;
    uint8_t GSM_update; //up to 4min15s
    bool GSM_update_flag;
    uint16_t TOWERS_update; //up to 4min15s
    bool TOWERS_update_flag;
    uint16_t SERVER_update; //up to 18h12min16sec
    bool SERVER_update_flag;
}regular_update_t;

extern volatile bool TCC0_timeout;
extern volatile bool TCC1_timeout;
extern regular_update_t Periodic_Checker;
extern regular_update_devices_t Periodic_Checker_Devices;

void TCC0_init(); //TCC0 used for GSM status update rssi, registration and so on
void TCC0_OFF();
void TCC0_ON(uint32_t period_us);
void TCC0_CHECKER();

void TCC1_init(); //TCC1 used for RS485 answer waiting timeout
void TCC1_OFF();
void TCC1_ON(uint32_t period_us);
void TCC1_CHECKER();


#ifdef	__cplusplus
}
#endif

#endif	/* TCC_H */

