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
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ILI9341.h"
#include "SERCOM.h"
#include "A7672E.h"    
#include "XPT2046.h"
#include "keyboard.h"
#include "TC.h"
#include "DMA.h"
#include "image.h"
#include "RTC.h"
#include "BME680.h"
#include "windows.h"
#include "Cosmos.h"
#include "refraction.h"
#include "ADC.h"
#include "TCC.h"
#include "Towers.h"

/* --- GPIO --- */
// Initialize all required GPIO pins (LCD, SPI, control lines)
void GPIO_init();


/* --- Clock configuration --- */
// Configure CPU clock using external TCXO (~2 MHz configuration)
void cpu_2MHz_TCXO_init();

// Configure CPU clock to ~128 MHz using DPLL0 with XOSC1 reference
void cpu_120Mhz_DPLL0_XOSC1_init();

// Simple blocking delay (approximate timing based on F_CPU)
void delay_ms(uint32_t ms);


/* --- SERCOM / SPI --- */

// Initialize SERCOM core clock
void GCLK1_SERCOM_SPIM_core_init();

void GCLK2_SERCOM_USARTM_core_init();
        
void GCLK3_SERCOM_TC_core_init();

// Initialize selected SERCOM interface (SPI/I2C/USART depending on enum)
void SERCOM_init(sercom_init_t interface);

// Blocking SPI transfer using 32-bit packed hardware mode
void SPI0_Transfer_32b_HW(uint32_t data, uint8_t length);

void SPI0_Baud_Switch(uint32_t baud);

/* --- DMA --- */
// Initialize DMA controller and descriptor base tables
void DMA_init();

// Configure DMA channel for SERCOM0 TX trigger
void DMA_SERCOM0_TX_init();

void SPI_DMA_LCD_send_area(const LCD_Transfer_t *transfer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

uint32_t swap_and_align(uint32_t data, uint8_t length);

void SPI0_Transfer_set_length(uint8_t length);

void SPI0_Transfer_32b_HW_cycle(uint32_t data);


uint32_t I2C_read(uint8_t addr, uint8_t readlen);

void I2C_write(uint8_t addr, uint32_t data, uint8_t length, i2c_cmd_t endaction);

uint32_t I2C_write_and_read(uint8_t addr, uint32_t reg, uint8_t writelen, uint8_t readlen);



void USART_write_str(char * str);

void USART_printf(const char *fmt, ...);

void USART_set_read_length(uint8_t length);

void USART_read_string(uint8_t total_length);

void USART_read_data_frame(uint8_t frame_length);

void USART_read_data_frame_by_one_byte(uint8_t frame_length);

void DMA_SERCOM3_RX_init();

void USART_set_read_length(uint8_t length);

void process_usart_data_frame();

bool process_usart_data_raw(uint8_t listIndex, uint16_t *y);

void USART_DMA_Circular_BYTE_Init();

uint16_t USART_DMA_read_progress();

void USART_DMA_Circular_BYTE_STOP();


void USART_DMA_Temp_Circular_BYTE_Init(char *RXBUF, uint16_t len);

void USART_DMA_Circular_BYTE_ENABLE(bool enable);




#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_H */
