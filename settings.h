/* 
 * File:   settings.h
 * Author: Saulius
 *
 * Description:
 *   Global project configuration header.
 *   Contains CPU frequency definition, common includes,
 *   and forward declarations for low-level hardware modules:
 *   clock setup, GPIO, SERCOM (SPI), DMA, and ILI9341 LCD driver.
 *
 * Created on: Thursday, 29 January 2026, 14:24
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

/* --- CPU configuration --- */
// Core CPU frequency used for delays and timing calculations
#define F_CPU 128000000ULL //cpu clock

/* --- Standard and device includes --- */
#include <xc.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "CLK.h"
#include "GPIO.h"
#include "ILI9341.h"
#include "DMA.h"    
#include "SERCOM.h"
#include "A7672E.h"    
#include "XPT2046.h"
#include "keyboard.h"
#include "TC.h"
#include "image.h"
#include "RTC.h"
#include "windows.h"
#include "Cosmos.h"
#include "refraction.h"
#include "TCC.h"
#include "RS485.h"   
#include "USART.h"  
#include "Towers.h"
#include "crc8.h"
#include "eeprom.h"
#include "Sensors.h"

#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_H */
