/* 
 * File:   USART.h
 * Author: Saulius
 *
 * Created on Treèiadienis, 2026, kovas 11, 22.56
 */

#ifndef USART_H
#define	USART_H
#ifdef	__cplusplus
extern "C" {
#endif

#define TOWERS_BAUD 115200 //RS485
#define GSM_BAUD 115200 //USART
#define SENSORS_BAUD 230400 //RS485
    
#define USART_GSM_REG    SERCOM3_REGS   

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

