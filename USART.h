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
    
#define USART_START_TIMEOUT_TIMES 3
#define USART_START_TIMEOUT_X_US 10 // waiting timeout for start is 10x times more than regular timeout

    
USARTFAULTS_t USART_SUCK = {
    .FaultCode = 0,
    .Data = 0,
    .CycleStorage = {0},
    //.RAWData = {0},
    .TimeoutCounter = 0,
    .rx_read_index = 0,
    .FrameData = {0},
};    

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

