/* 
 * File:   ADC.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, liepa 6, 00.01
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    WIND_SPEED,
    WIND_DIR
} wind_measure_t;    
    
typedef struct {
    uint16_t speed;
    uint16_t direction;
} ADCParameters;

extern ADCParameters WIND;

void ADC0_init(); //initialization of ADC module
void GCLK4_SERCOM_ADC_core_init(); // initialization of clock engine for adc0
void ADC0_read(wind_measure_t wind_ch); //reads wind speed or direction 

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

