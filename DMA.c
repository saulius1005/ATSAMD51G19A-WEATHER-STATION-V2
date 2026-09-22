#include "settings.h"
#include "DMAVar.h"


void DMA_init(){// Initialize DMA controller with descriptor base addresses
    MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_DMAC_Msk;
    DMAC_REGS->DMAC_BASEADDR = (uint32_t)descriptor_section;
    DMAC_REGS->DMAC_WRBADDR  = (uint32_t)wrb;
    DMAC_REGS->DMAC_CTRL = DMAC_CTRL_DMAENABLE_Msk | DMAC_CTRL_LVLEN0_Msk;
}

void DMA_SERCOM0_TX_init(){ //for ili9341 lcd controller
    DMAC_REGS->CHANNEL[SPI_CH].DMAC_CHCTRLA = DMAC_CHCTRLA_TRIGSRC(DMA_devices[SPI_CH].TRIGSRC) | DMAC_CHCTRLA_TRIGACT_BURST | DMAC_CHCTRLA_BURSTLEN_SINGLE | DMAC_CHCTRLA_THRESHOLD_1BEAT;
}

void SPI_DMA_LCD_send_area(const LCD_Transfer_t *transfer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){//for ili9341 lcd controller fill color or draw pixels
    uint32_t pixels = transfer->pixel_count;

    ili9341_set_address_window(x0, y0, x1, y1); //set window 

    DMAC_REGS->CHANNEL[SPI_CH].DMAC_CHCTRLA &= ~DMAC_CHCTRLA_ENABLE_Msk;

    descriptor_section[SPI_CH].BTCNT = pixels / 2;
    descriptor_section[SPI_CH].DSTADDR = (uint32_t)&DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_DATA;
    descriptor_section[SPI_CH].DESCADDR = 0;

    if (transfer->is_solid_color) { //if it is color fill
        static uint32_t color_word;
        color_word = ((uint32_t)transfer->source.color_val << 16) | transfer->source.color_val; //make it to 32bit
        descriptor_section[SPI_CH].BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_BEATSIZE_WORD;
        descriptor_section[SPI_CH].SRCADDR = (uint32_t)&color_word;
    } 
    else { //if it is image
        descriptor_section[SPI_CH].BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_BEATSIZE_WORD | DMAC_BTCTRL_SRCINC_Msk;
        descriptor_section[SPI_CH].SRCADDR = (uint32_t)transfer->source.image_data + (pixels * 2);
    }

    DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_LENGTH &= ~SERCOM_SPIM_LENGTH_LENEN_Msk; //clear last used length and set it to default value 4bytes (32bit extention is enabled)

    ILI9341_DC_DATA();
    ILI9341_CS_LOW();

    DMAC_REGS->CHANNEL[SPI_CH].DMAC_CHCTRLA |= DMAC_CHCTRLA_ENABLE_Msk;
    DMAC_REGS->CHANNEL[SPI_CH].DMAC_CHCTRLB |= DMAC_CHCTRLB_CMD_RESUME;

    while (!(DMAC_REGS->CHANNEL[SPI_CH].DMAC_CHINTFLAG & DMAC_CHINTFLAG_TCMPL_Msk));
    DMAC_REGS->CHANNEL[SPI_CH].DMAC_CHINTFLAG = DMAC_CHINTFLAG_TCMPL_Msk;

    while (!(DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));

    ILI9341_CS_HIGH();
}

void DMA_USART_RS485_RX_init(DMA_channel_t channel){ //channels: 1- GSM A767E module USART, 2- TOWERS RS485, 3- Sensors RS485
    if(channel == SPI_CH) // if selected SPI channel ignore further code
        return;
    DMAC_REGS->CHANNEL[channel].DMAC_CHCTRLA = DMAC_CHCTRLA_TRIGSRC(DMA_devices[channel].TRIGSRC) | DMAC_CHCTRLA_TRIGACT_BURST | DMAC_CHCTRLA_BURSTLEN_SINGLE | DMAC_CHCTRLA_THRESHOLD_1BEAT;
}

void DMA_USART_RS485_Temp_Circular_BYTE_init(char *RXBUF, uint16_t len, DMA_channel_t channel){//channels: 1- GSM A767E module USART, 2- TOWERS RS485
    if(channel == SPI_CH) // if selected SPI channel ignore further code
        return;
    USART_set_read_length(1, channel);//set length to one byte       
    descriptor_section[channel].BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_DSTINC_Msk | DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_BLOCKACT_NOACT;
    descriptor_section[channel].BTCNT = len ;
    
    descriptor_section[channel].SRCADDR = (uint32_t)&DMA_devices[channel].SERCOM->USART_INT.SERCOM_DATA; //select source for DMA sercom2, 3 or 5
    
    descriptor_section[channel].DSTADDR = (uint32_t)RXBUF + len;
    descriptor_section[channel].DESCADDR = 0; //if array full stop    
}

void DMA_USART_RS485_Circular_BYTE_ENABLE(bool enable, DMA_channel_t channel){//channels: 0- spi not used, 1- GSM A767E module USART, 2- TOWERS RS485, 3-Sensors RS485
    if(channel == SPI_CH) // if selected SPI channel ignore further code
        return;
    if(enable){        
        DMAC_REGS->CHANNEL[channel].DMAC_CHCTRLA |= DMAC_CHCTRLA_ENABLE_Msk; //enable channell 
    }
    else{
        DMAC_REGS->CHANNEL[channel].DMAC_CHCTRLA &= ~DMAC_CHCTRLA_ENABLE_Msk;
    }
}


