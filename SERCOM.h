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

#define F_S_SPIM_G1 60000000ULL //sercom spimcore clock 
#define F_S_I2CM_G2 24000000UL //sercom i2c core clock
#define F_S_USART_G3 F_S_I2CM_G2//use the same core clock

/* 
 * Calculate asynchronous arithmetic baud register value.
 *
 * Formula based on SERCOM USART asynchronous arithmetic mode:
 * BAUD = 65536 - (65536 * 16 * f_baud / F_CPU)
 *
 * Notes:
 * - Requires F_CPU to be defined before including this header.
 * - Intended for standard oversampling mode (x16).
 */
#define SERCOM_SPI_BAUD(Fsck) ((uint8_t)((F_S_SPIM_G1 / (2UL * (Fsck))) - 1UL))//SPI
#define SERCOM_USART_ASYNC_ARITH_BAUD(f_baud) (uint16_t)(65536ULL - ((65536ULL * 16ULL * (f_baud)) / F_S_USART_G3))//USART
#define SERCOM_I2C_BAUD(Fscl, Trise_ns) ((uint32_t)(( (float)F_S_I2CM_G2 / (2.0 * (Fscl)) ) - 5.0 - ( ((float)F_S_I2CM_G2 * (Trise_ns)) / 2000000000.0 )))// I2C


#define UART_RX_BUFFER_SIZE 256 //cgnssinfo one sentance is about ~110symbols including echo of command. need to increase

/* 
 * SERCOM peripheral operating mode selector.
 * Used during SERCOM initialization to select interface type.
 */
typedef enum {
    USART_GSM,   // Universal Synchronous/Asynchronous Receiver/Transmitter
    SPI_SCREEN, //SPI for LCD and Touch screen
    SPI_SENSOR, //SPI for sensors
    USART_RS485      // Inter-Integrated Circuit
} sercom_init_t;

typedef enum {
    I2C_CMD_NoAction = 0,
    I2C_CMD_RepeatStart,
    I2C_CMD_Continue,
    I2C_CMD_Stop
} i2c_cmd_t;

typedef struct{ //for debuging purposes
    uint16_t FaultCode;
}I2CFAULTS_t;

extern I2CFAULTS_t I2C_SUCK;

typedef enum {
    UART_TX, // send
    UART_RX  // receive
} uart_dir_t;

void USART_set_read_length(uint8_t length, DMA_channel_t channel);

#ifdef	__cplusplus
}
#endif

#endif	/* SERCOM_H */
