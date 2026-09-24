/* 
 * File:   DMA.h
 * Author: Saulius
 *
 * Description:
 *   DMA controller descriptor definitions for ATSAMD51.
 *   Defines the descriptor structure and allocates descriptor tables.
 *
 * Created on: Friday, 6 February 2026, 20:49
 */

#ifndef DMA_H
#define	DMA_H

#ifdef	__cplusplus
extern "C" {
#endif

/* --- DMAC descriptor structure ---
 * Represents one DMA transfer descriptor.
 * Used by the ATSAMD51 DMAC for chained transfers.
 */
typedef struct {
    volatile uint16_t BTCTRL;   // Block Transfer Control (beat size, src/dst increment, valid, etc.)
    volatile uint16_t BTCNT;    // Block Transfer Count (number of beats to transfer)
    volatile uint32_t SRCADDR;  // Source address (DMAC expects end address if SRCINC enabled)
    volatile uint32_t DSTADDR;  // Destination address
    volatile uint32_t DESCADDR; // Next descriptor in chain (0 = end of chain)
}dmacdescriptor;

typedef enum {
    DMA_MODE_PIXELS,
    DMA_MODE_COLOR
} DMA_Send_Mode_t;

typedef enum {
    SPI_CH = 0,
    GSM_CH,
    TOWER_CH,
    SENSORS_CH,
} DMA_channel_t;

typedef struct {
    DMA_channel_t channel; //for identification
    uint8_t TRIGSRC; //trigger id value for dma
    volatile sercom_registers_t *SERCOM; //pointer to sercom 2 , 3, 5
}DMA_devices_t;

typedef struct {
    union {
        const uint16_t *image_data;
        uint16_t color_val;
    } source;

    uint32_t pixel_count;     // how much pixels for image
    uint8_t is_solid_color;
} LCD_Transfer_t;

extern volatile dmacdescriptor descriptor_section[4];
extern volatile dmacdescriptor wrb[4];
extern volatile uint8_t dma_done;
extern DMA_devices_t DMA_devices[4];

void DMA_core_init(); // initializes DMA core

void DMA_SPI_LCD_TX_init(); // initializes ili9341 lcd controller dma channel for data TX

void DMA_SPI_LCD_send_area(const LCD_Transfer_t *transfer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); // fill screen with color or image

void DMA_USART_RS485_RX_init(DMA_channel_t channel); // initialize USART or RS485 DMA channel for data RX

void DMA_USART_RS485_Storage_init(char *RXBUF, uint16_t len, DMA_channel_t channel);

void DMA_USART_RS485_Enable(bool enable, DMA_channel_t channel);

void DMA_init_all(); //initialization of all devices who use DMA

#ifdef	__cplusplus
}
#endif

#endif	/* DMA_H */
