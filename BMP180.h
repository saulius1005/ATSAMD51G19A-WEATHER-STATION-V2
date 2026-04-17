/* 
 * File:   BMP280.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, kovas 2, 20.07
 */

#ifndef BMP280_H
#define	BMP280_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define BMP180_ADD 0xEE //for writing

typedef enum {
    BMP_T = 0,
    BMP_P
} bmp180_parameters_t;    

typedef struct{
    uint8_t CONTROL;
    uint8_t DATA;
    uint8_t CALIB_START;
} bmp180_reg_t;

typedef struct{
 int16_t AC1;
 int16_t AC2;
 int16_t AC3;
 uint16_t AC4;
 uint16_t AC5;
 uint16_t AC6;
 int16_t B1;
 int16_t B2;
 int16_t MB;
 int16_t MC;
 int16_t MD;
 int32_t UT;
 int32_t UP;
} bmp180_calib_t;

typedef struct{
    uint8_t address;
    uint8_t oss;

    bmp180_reg_t reg;
    bmp180_calib_t calib;

    int32_t Temperature;
    int32_t Pressure;
    int32_t Altitude;
    uint8_t step;
    bool cycle;
    
    bool DataReady;

} bmp180_t;

typedef enum {
    BMP180_IDLE,
    BMP180_WAIT,
    BMP180_READ
} BMP180_STATE;

extern bmp180_t BMP180;

#ifdef	__cplusplus
}
#endif

#endif	/* BMP280_H */

