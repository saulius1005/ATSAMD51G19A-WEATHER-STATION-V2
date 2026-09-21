/* 
 * File:   SensorsVar.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, rugsëjis 21, 21.21
 */

#ifndef SENSORSVAR_H
#define	SENSORSVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

sensors_data_t sensors = {
    .BME680 = {
        -3, //in C
        1004, //in hPa
        59, //in %
    },
    .WIND = {
        5, // in m/s
        3, // one of 8 (0-7)
    },
    .SUN = {
        1236, // in mV
    },
};


#ifdef	__cplusplus
}
#endif

#endif	/* SENSORSVAR_H */

