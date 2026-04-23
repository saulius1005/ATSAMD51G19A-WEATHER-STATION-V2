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
#include "XPT2046.h"
#include "keyboard.h"
#include "BMP180.h"
#include "TC.h"
#include "DMA.h"
#include "image.h"
#include "A7672E.h"
#include "RTC.h"
#include "BME680.h"

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

void GCLK2_SERCOM_I2CM_core_init();

// Initialize selected SERCOM interface (SPI/I2C/USART depending on enum)
void SERCOM_init(sercom_init_t interface);

// Blocking SPI transfer using 32-bit packed hardware mode
void SPI0_Transfer_32b_HW(uint32_t data, uint8_t length);

void SPI0_Baud_Switch(uint32_t baud);


/* --- ILI9341 LCD driver (CPU mode) --- */
// Basic LCD initialization using SPI transfers
void ILI9341_init_simple_32b();

// Fill entire display with color using CPU-driven SPI transfers
void ILI9341_fill_color_CPU(uint16_t color);

// Draw framebuffer using CPU-only SPI transfers
void ILI9341_draw_image_CPU(const uint16_t *fb);


/* --- ILI9341 LCD driver (DMA mode) --- */
// Fill entire display with color using DMA-assisted SPI transfers
void ILI9341_fill_color_32b_DMA(uint16_t color);

// Draw framebuffer using DMA-assisted SPI transfers
void ILI9341_draw_framebuffer_DMA(const uint16_t *fb);

void ili9341_fill_screen(uint16_t color);

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

void ili9341_draw_char(int x, int y, char c, uint16_t fg, uint16_t bg);

void ili9341_draw_text(int x, int y, const char *str, uint16_t fg, uint16_t bg);

void draw_formatted_line(uint16_t x, uint16_t *y, uint16_t fg, uint16_t bg, const char *fmt, ...);

void draw_colored_line(uint16_t x, uint16_t *y, color_segment_t *segments, size_t count);

void ili9341_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint8_t filled);

void ILI9341_draw_keyboard(uint16_t *fb, uint16_t x0, uint16_t y0, uint8_t state, uint16_t color);

void ili9341_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void ili9341_CMD(uint32_t cmd, uint8_t length);

void ili9341_DATA(uint32_t data, uint8_t length);

void ILI9341_draw_image_DMA(const uint16_t *fb);

void ILI9341_fill_color_DMA(uint16_t color);

void source(Source_data *btn); //keyboard drawing and touch screen actions

/* --- DMA --- */
// Initialize DMA controller and descriptor base tables
void DMA_init();

// Configure DMA channel for SERCOM0 TX trigger
void DMA_SERCOM0_TX_init();

void SPI_DMA_LCD_send_area(const LCD_Transfer_t *transfer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

uint32_t swap_and_align(uint32_t data, uint8_t length);

void SPI0_Transfer_set_length(uint8_t length);

void SPI0_Transfer_32b_HW_cycle(uint32_t data);

uint16_t XPT2046_Read(uint32_t cmd);

uint32_t BME680_exchange_data(uint32_t cmd, uint8_t tx_length);

void BME680_write(uint32_t cmd, uint8_t length);

void BME680_change_page(BME680_page_no_t page);

void BME680_read_ID(); //read ID

void BME680_reset(); //reset sensor (same as power up reset)

void BME680_Config(BME680_filter_t filter, bool spi_3w_en); //modify Config register

void BME680_Ctrl_meas(BME680_meas_os_t os_t, BME680_meas_os_t os_p, BME680_mode_t mode); //modify Ctrl_meas register

void BME680_read_temp_calib();

void BME680_calculate_temperature();

void BME680_calculate_pressure();



uint32_t I2C_read(uint8_t addr, uint8_t readlen);

void I2C_write(uint8_t addr, uint32_t data, uint8_t length, i2c_cmd_t endaction);

uint32_t I2C_write_and_read(uint8_t addr, uint32_t reg, uint8_t writelen, uint8_t readlen);

void BMP180_ReadCalibration(bmp180_t *bmp);

void BMP180_ReadUTUP(bmp180_t *bmp, bmp180_parameters_t parameter); //polled

void BMP180_ReadUTUP_Task(bmp180_t *bmp, bmp180_parameters_t parameter); //state machine + interrupt

void BMP180_Task();

void BMP180_CalcTrueTP();

void GCLK3_SERCOM_TC_core_init();

void TC0_init();

void TC0_ON(uint32_t period_us);

void TC0_OFF();

void TC0_CHECKER();

void TC2_init();

void TC2_ON(uint32_t period_us);

void TC2_OFF();

void TC2_CHECKER();

void USART_write_str(char * str);

void USART_printf(const char *fmt, ...);

void USART_set_read_length(uint8_t length);

void USART_read_string(uint8_t total_length);

//void USART_read_data_frame();

void USART_read_data_frame(uint8_t frame_length);

void USART_read_data_frame_by_one_byte(uint8_t frame_length);

void DMA_SERCOM3_RX_init();

void USART_set_read_length(uint8_t length);

void process_usart_data_frame();

bool process_usart_data_raw(uint8_t listIndex, uint16_t *y);

void USART_DMA_Circular_BYTE_Init();

uint16_t USART_DMA_read_progress();

void USART_DMA_Circular_BYTE_STOP();

void A7672EsendCommandsInit(); //sending at commands from list at a7672var.h

void A7672ReadNEMAGNSS(); //gps data

void RTC_init_calendar();

uint32_t RTC_read_sys_time();

void RTC_read_date_and_time();

uint32_t datetime_to_rtc_format(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t s);

void RTC_date_and_time_update();

bool A7672Etestcycle(char *searchfor, uint8_t packs, uint16_t bufsize, char *answer, uint16_t write_index);

void USART_DMA_Temp_Circular_BYTE_Init(char *RXBUF, uint16_t len);

void USART_DMA_Circular_BYTE_ENABLE(bool enable);

void A7672EInit();

void A7672ReadNEMAGNSS();

#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_H */
