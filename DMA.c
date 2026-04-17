#include "settings.h"
#include "DMAVar.h"


// Initialize DMA controller with descriptor base addresses
void DMA_init(){
    MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_DMAC_Msk;
    DMAC_REGS->DMAC_BASEADDR = (uint32_t)descriptor_section;
    DMAC_REGS->DMAC_WRBADDR  = (uint32_t)wrb;
    DMAC_REGS->DMAC_CTRL = DMAC_CTRL_DMAENABLE_Msk | DMAC_CTRL_LVLEN0_Msk;
}

void DMA_SERCOM0_TX_init(){
    DMAC_REGS->CHANNEL[0].DMAC_CHCTRLA = DMAC_CHCTRLA_TRIGSRC(SERCOM0_DMAC_ID_TX) | DMAC_CHCTRLA_TRIGACT_BURST | DMAC_CHCTRLA_BURSTLEN_SINGLE | DMAC_CHCTRLA_THRESHOLD_1BEAT;
}

void SPI_DMA_LCD_send_area(const LCD_Transfer_t *transfer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    uint32_t pixels = transfer->pixel_count;

    ili9341_set_address_window(x0, y0, x1, y1); //set window 

    DMAC_REGS->CHANNEL[0].DMAC_CHCTRLA &= ~DMAC_CHCTRLA_ENABLE_Msk;

    descriptor_section[0].BTCNT = pixels / 2;
    descriptor_section[0].DSTADDR = (uint32_t)&SERCOM0_REGS->SPIM.SERCOM_DATA;
    descriptor_section[0].DESCADDR = 0;

    if (transfer->is_solid_color) { //if it is color fill
        static uint32_t color_word;
        color_word = ((uint32_t)transfer->source.color_val << 16) | transfer->source.color_val; //make it to 32bit
        descriptor_section[0].BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_BEATSIZE_WORD;
        descriptor_section[0].SRCADDR = (uint32_t)&color_word;
    } 
    else { //if it is image
        descriptor_section[0].BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_BEATSIZE_WORD | DMAC_BTCTRL_SRCINC_Msk;
        descriptor_section[0].SRCADDR = (uint32_t)transfer->source.image_data + (pixels * 2);
    }

    SERCOM0_REGS->SPIM.SERCOM_LENGTH &= ~SERCOM_SPIM_LENGTH_LENEN_Msk; //clear last used length and set it to default value 4bytes (32bit extention is enabled)

    ILI9341_DC_DATA();
    ILI9341_CS_LOW();

    DMAC_REGS->CHANNEL[0].DMAC_CHCTRLA |= DMAC_CHCTRLA_ENABLE_Msk;
    DMAC_REGS->CHANNEL[0].DMAC_CHCTRLB |= DMAC_CHCTRLB_CMD_RESUME;

    while (!(DMAC_REGS->CHANNEL[0].DMAC_CHINTFLAG & DMAC_CHINTFLAG_TCMPL_Msk));
    DMAC_REGS->CHANNEL[0].DMAC_CHINTFLAG = DMAC_CHINTFLAG_TCMPL_Msk;

    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));

    ILI9341_CS_HIGH();
}

void DMA_SERCOM3_RX_init(){
    DMAC_REGS->CHANNEL[1].DMAC_CHCTRLA = DMAC_CHCTRLA_TRIGSRC(SERCOM3_DMAC_ID_RX) | DMAC_CHCTRLA_TRIGACT_BURST | DMAC_CHCTRLA_BURSTLEN_SINGLE | DMAC_CHCTRLA_THRESHOLD_1BEAT;
}

void USART_DMA_Temp_Circular_BYTE_Init(char *RXBUF, uint16_t len){
    USART_set_read_length(1);//set length to one byte       
    descriptor_section[1].BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_DSTINC_Msk | DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_BLOCKACT_NOACT;
    descriptor_section[1].BTCNT = len ;
    descriptor_section[1].SRCADDR = (uint32_t)&SERCOM3_REGS->USART_INT.SERCOM_DATA; //pointer to storage array
    descriptor_section[1].DSTADDR = (uint32_t)RXBUF + len;
    descriptor_section[1].DESCADDR = 0; //if array full stop    
}

void USART_DMA_Circular_BYTE_ENABLE(bool enable){
    if(enable){        
        DMAC_REGS->CHANNEL[1].DMAC_CHCTRLA |= DMAC_CHCTRLA_ENABLE_Msk; //enable channell 
    }
    else{
        DMAC_REGS->CHANNEL[1].DMAC_CHCTRLA &= ~DMAC_CHCTRLA_ENABLE_Msk;
    }
}