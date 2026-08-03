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
    
#define Wind_Speed_koef 0.00732421875 //wind speed sensor max speed 30m/s divided by 12bit resolution 4096 or simpler: 30/4096 
#define Wind_Direction_step 512 //ADC full range / 8 to get 8 parts of wind direction: 4096/8

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
char * WindDirNames(); //return name of wind direction such as N, NE, E,....
void WIND_update();

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

