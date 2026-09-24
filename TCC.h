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
    
void GCLK3_SERCOM_TC_core_init();    
    
typedef enum {
    PREPARED = 0,
    UPDATING,
    UPDATED
}update_status_t;

typedef struct {
    uint32_t update_time;
    uint16_t respond_time;
    update_status_t update_stat;   
    uint32_t start_at;
}regular_update_param_t;

typedef struct {
    uint32_t period_counter;
    regular_update_param_t GSM;
    regular_update_param_t TIME;
    regular_update_param_t SERVER;
    regular_update_param_t TOWERS;
    regular_update_param_t SENSORS;
    
}regular_update_devices_t;

extern volatile bool TCC0_timeout;
extern volatile bool TCC1_timeout;

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

