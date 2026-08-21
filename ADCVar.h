/* 
 * File:   ADCVar.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, liepa 6, 00.01
 */

#ifndef ADCVAR_H
#define	ADCVAR_H

#include "ADC.h"



#ifdef	__cplusplus
extern "C" {
#endif

ADCParameters_wind WIND = {
    .speed = 0,
    .direction = 0,
};

ADCParameters_sun SUN = {
    .level = 0,
};


#ifdef	__cplusplus
}
#endif

#endif	/* ADCVAR_H */

