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

#define USART_BAUD 115200
#define USART_SYMBOL_WIDTH 10UL //start + 8 bit data + stop
#define USART_TIMEOUT_US(symbols) (uint16_t)((USART_SYMBOL_WIDTH * 1000000UL * (symbols)) / (USART_BAUD) + 1) //calculate timeout value for TC1 counter.
    

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

