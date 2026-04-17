/* 
 * File:   BMP280Var.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, kovas 2, 20.07
 */

#ifndef BMP280VAR_H
#define	BMP280VAR_H

#include "BMP180.h"


#ifdef	__cplusplus
extern "C" {
#endif

bmp180_t BMP180 = {
    .address = 0xEE, //bmp180 write add
    .oss = 3, //pressure oss value
    .reg = {
        .CONTROL = 0xF4, //control register
        .DATA = 0xF6, //data register
        .CALIB_START = 0xAA //star calibration from
    },
    .step = 0,
    .cycle = false,
    .DataReady = false,
};

static BMP180_STATE bmp_state = BMP180_IDLE;
static bmp180_parameters_t current_param;


#ifdef	__cplusplus
}
#endif

#endif	/* BMP280VAR_H */

