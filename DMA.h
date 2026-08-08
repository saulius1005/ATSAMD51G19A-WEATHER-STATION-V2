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
} DMA_channel_t;

typedef struct {
    union {
        const uint16_t *image_data;
        uint16_t color_val;
    } source;

    uint32_t pixel_count;     // how much pixels for image
    uint8_t is_solid_color;
} LCD_Transfer_t;

extern volatile dmacdescriptor descriptor_section[3];
extern volatile dmacdescriptor wrb[3];

extern volatile uint8_t dma_done;

#ifdef	__cplusplus
}
#endif

#endif	/* DMA_H */
