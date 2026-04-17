/* 
 * File:   DMAVar.h
 * Author: Saulius
 *
 * Created on Antradienis, 2026, kovas 17, 11.22
 */

#ifndef DMAVAR_H
#define	DMAVAR_H

#ifdef	__cplusplus
extern "C" {
#endif


volatile __attribute__((aligned(16))) dmacdescriptor descriptor_section[2]; //0- SPI, 1-USART
volatile __attribute__((aligned(16))) dmacdescriptor wrb[2]; //0-SPI, 1-USART

volatile uint8_t dma_done = 0;

#ifdef	__cplusplus
}
#endif

#endif	/* DMAVAR_H */

