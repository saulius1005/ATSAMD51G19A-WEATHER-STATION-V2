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


volatile __attribute__((aligned(16))) dmacdescriptor descriptor_section[4]; //0- ili9341 SPI, 1 - A7672E USART, 2 - Towers RS485, 3 - Sensors RS485
volatile __attribute__((aligned(16))) dmacdescriptor wrb[4]; //0- ili9341 SPI, 1-A7672E USART, 2 - Towers RS485, 3 - Sensors RS485

volatile uint8_t dma_done = 0;

DMA_devices_t DMA_devices[4] = {
    { SPI_CH,     SERCOM0_DMAC_ID_TX, SERCOM0_REGS },
    { GSM_CH,     SERCOM3_DMAC_ID_RX, SERCOM3_REGS },
    { TOWER_CH,   SERCOM2_DMAC_ID_RX, SERCOM2_REGS },
    { SENSORS_CH, SERCOM5_DMAC_ID_RX, SERCOM5_REGS }
};

#ifdef	__cplusplus
}
#endif

#endif	/* DMAVAR_H */

