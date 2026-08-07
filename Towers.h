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

/* 
 * Tower respond data example from tower controller:
 * frame start with [ and ends with ]
 * last 2 hex symbols is cdma2000 crc8
 * 
  USART_printf(0, "[%02x%04x%04x%03x%03x%x%03x%03x%03x%03x%02x]\r\n",
	(uint8_t)DEVICE_ID_NUMBER,
	(uint16_t)SensorData.HPElevation,
	(uint16_t)SensorData.HPAzimuth,
	(uint16_t)SensorData.PVU,
	(uint16_t)abs(SensorData.PVI),
	(uint8_t)SensorData.endSwitches,
	(uint16_t)StepperMotor.measuredVoltage,
	(uint16_t)abs(StepperMotor.measuredCurrent),
	(uint16_t)LinearMotor.measuredVoltage,
	(uint16_t)abs(LinearMotor.measuredCurrent),
	(uint8_t)crc8_cdma2000_id(DEVICE_ID_NUMBER)
	);
 */    
    
typedef struct{
    uint16_t voltage; //PVU ffff - Exp 1500.9V 15009 (0x3aa1), 250.5V 2505 (0x9c9)  , StepperMotor.measuredVoltage, LinearMotor.measuredVoltage
    uint16_t current; //PVI ffff - Exp 21.33A 2133 (0x855)                          , StepperMotor.measuredCurrent, LinearMotor.measuredCurrent
} electrical_t;

typedef struct{
    uint16_t azimuth; //HPAzimuth ffff- Exp 359.99 3599 (0x8c9f)
    uint16_t elevation; //HPElevation ffff- Exp 89.99 3599 (0x2327)
} position_t;

typedef struct{
    uint8_t id; // DEVICE_ID_NUMBER 1, 2... , 255 (0-ff)
    uint8_t es; // SensorData.endSwitches not used for now future feature
    position_t position;
    electrical_t panel;
    electrical_t az_motor;
    electrical_t el_motor;
} tower_t;

extern tower_t towers[];

#ifdef	__cplusplus
}
#endif

#endif	/* TOWERS_H */

