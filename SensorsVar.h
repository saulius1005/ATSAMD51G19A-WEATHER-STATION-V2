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
    .state = SEND,
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

sensor_network_t modules = {
    .sensor_modules[T_P_RH_WS_WD] = { // T_P_RH_WS_WD == 1 // for now only one sensor module
        .name = T_P_RH_WS_WD, // for now not in use
        .idcall = "{01}\r\n"
    }
};


#ifdef	__cplusplus
}
#endif

#endif	/* SENSORSVAR_H */

