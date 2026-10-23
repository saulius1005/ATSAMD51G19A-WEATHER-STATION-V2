/* 
 * File:   Sensors.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, rugsëjis 21, 21.21
 */

#ifndef SENSORS_H
#define	SENSORS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define S_TX_LED_ON()   (PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA22) // disable-low
#define S_TX_LED_OFF()   (PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA22) // enable-high
    
#define S_RX_LED_ON()   (PORT_REGS->GROUP[1].PORT_OUTCLR = PORT_PB23) // enable-low
#define S_RX_LED_OFF()   (PORT_REGS->GROUP[1].PORT_OUTSET = PORT_PB23) // disable-high
    
typedef struct{
    int8_t temperature;   
    uint16_t pressure;
    uint8_t humidity;  
}bme680_data_t;

typedef struct{
    uint8_t speed;
    uint8_t direction; 
}wind_data_t;

typedef struct{
    uint16_t level;  
}sunlight_data_t;

typedef struct{
    last_update_t update_time; //type from A7672E.h
    tower_com_t state; //use typedef enum from towers
    bme680_data_t BME680;
    wind_data_t WIND;
    sunlight_data_t SUN;
}sensors_data_t;

extern sensors_data_t sensors;

char * WindDirNames();
void Sensors_COM();

#ifdef	__cplusplus
}
#endif

#endif	/* SENSORS_H */

