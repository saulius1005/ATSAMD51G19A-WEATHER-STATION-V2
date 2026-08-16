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
    
#define TOWER_COUNT 2 //for far far future may increase tower count. Maybe    
  
    
typedef enum{
    SEND = 0,
    WAIT_RESPOND,
    PROCESS,      
}tower_com_t;
    
typedef struct{
    uint16_t voltage; //PVU ffff - Exp 1500.9V 15009 (0x3aa1), 250.5V 2505 (0x9c9)  , StepperMotor.measuredVoltage, LinearMotor.measuredVoltage
    uint16_t current; //PVI ffff - Exp 21.33A 2133 (0x855)                          , StepperMotor.measuredCurrent, LinearMotor.measuredCurrent
} electrical_t;

typedef struct{
    uint16_t azimuth; //HPAzimuth ffff- Exp 359.99 3599 (0x8c9f)
    uint16_t elevation; //HPElevation ffff- Exp 89.99 3599 (0x2327)
} position_t;

typedef struct{
    tower_com_t state;
    uint8_t id; // DEVICE_ID_NUMBER 1, 2... , 255 (0-ff)
    uint8_t es; // SensorData.endSwitches not used for now future feature
    position_t position;
    electrical_t panel;
    electrical_t az_motor;
    electrical_t el_motor;
} tower_t;

extern tower_t towers[];

void Towers_init(); //create Towers list
void Tower_COM(); //main RS485 network for Towers function

#ifdef	__cplusplus
}
#endif

#endif	/* TOWERS_H */

