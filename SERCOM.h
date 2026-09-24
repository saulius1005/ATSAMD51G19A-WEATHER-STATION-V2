/* 
 * File:   SERCOM.h
 * Author: Saulius
 *
 * Description:
 *   SERCOM interface configuration definitions.
 *   Contains initialization type enumeration and helper macros
 *   for peripheral configuration (USART / SPI / I2C).
 *
 * Created on: Friday, 6 February 2026, 20:45
 */

#ifndef SERCOM_H
#define	SERCOM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define F_S_SPIM_G1 60000000ULL //SPI core clock 
#define F_S_USART_G3 24000000UL//USART/RS485 core clock

#define SERCOM_SPI_BAUD(Fsck) ((uint8_t)((F_S_SPIM_G1 / (2UL * (Fsck))) - 1UL))//SPI
#define SERCOM_USART_ASYNC_ARITH_BAUD(f_baud) (uint16_t)(65536ULL - ((65536ULL * 16ULL * (f_baud)) / F_S_USART_G3))//USART

#define UART_RX_BUFFER_SIZE 256 //cgnssinfo one sentance is about ~110symbols including echo of command. need to increase

typedef enum {
    USART_GSM,   // Universal Synchronous/Asynchronous Receiver/Transmitter
    SPI_SCREEN, //SPI for LCD and Touch screen
    RS485_SENSOR, //SPI for sensors
    RS485_TOWER      // Inter-Integrated Circuit
} sercom_init_t;

void GCLK1_SERCOM_SPIM_core_init(); //Initialize SPIM core clock

void GCLK2_SERCOM_USARTM_core_init(); //Intialize USART/RS485 core clock

void SERCOM_init(sercom_init_t interface);// Initialize selected SERCOM interface (USART, SPI or RS485)

void SPI0_Transfer_32b_HW(uint32_t data, uint8_t length);// Blocking SPI transfer using 32-bit packed hardware mode

void SPI0_Baud_Switch(uint32_t baud); //used for lcd and touch screen

uint32_t swap_and_align(uint32_t data, uint8_t length); //swap and align in places uint32_T value according to transfered bytes length

void SERCOM_init_all(); // initialization of all used sercom channels: 0,2,3,5

#ifdef	__cplusplus
}
#endif

#endif	/* SERCOM_H */
