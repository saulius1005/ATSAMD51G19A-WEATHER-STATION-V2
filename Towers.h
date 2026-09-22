/* 
 * File:   Towers.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 7, 18.43
 */

#ifndef TOWERS_H
#define	TOWERS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//#define TOWER_COUNT 2 //for far far future may increase tower count. Maybe    
  
    
typedef enum{
    SEND = 0,
    WAIT_RESPOND,
    PROCESS,   
    COMPLETE,
}tower_com_t;
    
typedef struct{
    uint16_t voltage; //PVU ffff - Exp 1500.9V 15009 (0x3aa1), 250.5V 2505 (0x9c9)  , StepperMotor.measuredVoltage, LinearMotor.measuredVoltage
    uint16_t current; //PVI ffff - Exp 21.33A 2133 (0x855)                          , StepperMotor.measuredCurrent, LinearMotor.measuredCurrent
    uint32_t power; //calculated from received data
} electrical_t;

typedef struct{
    uint16_t azimuth; //HPAzimuth ffff- Exp 359.99 3599 (0x8c9f)
    uint16_t elevation; //HPElevation ffff- Exp 89.99 3599 (0x2327)
} position_t;

typedef struct{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} last_update_t;

typedef struct{
    tower_com_t state;
    uint8_t id; // DEVICE_ID_NUMBER 1, 2... , 255 (0-ff)
    uint8_t es; // SensorData.endSwitches not used for now future feature
    position_t position;
    electrical_t panel;
    electrical_t az_motor;
    electrical_t el_motor;
    last_update_t update_time;
    char prepared_to_server[30];//all data without crc
} tower_t;

extern tower_t towers[];

void Towers_init(); //create Towers list
void Tower_COM(); //main RS485 network for Towers function
uint16_t fast_atoi_hex(const char *p, uint8_t digits);// also used in SENSOR.c

#ifdef	__cplusplus
}
#endif

#endif	/* TOWERS_H */

