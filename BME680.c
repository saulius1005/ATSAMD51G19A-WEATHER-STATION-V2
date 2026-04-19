#include "settings.h"
#include "BME680Var.h"

void BME680_write(uint32_t cmd, uint8_t length){ //max 4 bytes
    
    BME680_CS_LOW();
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(length) | SERCOM_SPIM_LENGTH_LENEN_Msk;
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM4_REGS->SPIM.SERCOM_DATA = swap_and_align(cmd, length);

    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));
    BME680_CS_HIGH();
}



uint32_t BME680_exchange_data(uint32_t cmd, uint8_t tx_length){
   
    uint32_t rx = 0;
    BME680_CS_LOW();
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(tx_length+1) | SERCOM_SPIM_LENGTH_LENEN_Msk; // all time +1 of dummy
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM4_REGS->SPIM.SERCOM_DATA = swap_and_align(cmd, tx_length);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));
    rx = SERCOM4_REGS->SPIM.SERCOM_DATA;   
    BME680_CS_HIGH();  

    return rx;//remove trash (first byte)
}

/*static inline uint8_t BME680_SPI_WriteAddr(uint8_t reg){
    return reg & 0x7F;
}*/

static inline uint8_t BME680_SPI_ReadAddr(uint8_t reg){
    return reg | 0x80;
}

void BME680_change_page(BME680_page_no_t page){ //set spi page and return page value after write once
    
    if(BME680.STATUS_spi_mem_page == page) //if it is the same page do not change it and skip further code
        return;   
    uint32_t cmd = ((uint32_t) status_ADD << 16) | ((uint16_t) (page == BME680_page_1 ? 16 : 0) << 8) | BME680_SPI_ReadAddr(status_ADD) ; //set write add (0x73) | set page (0x00 or 0x10) | set read add (0xF3)     
    BME680.STATUS_spi_mem_page = ((swap_and_align(BME680_exchange_data(cmd, 3), 4) & 16) >> 4) == BME680_page_1 ? BME680_page_1 : BME680_page_0; //swap bytes in places from answer total received is 4 bytes, then if 1st page return 1 else 0
}

void BME680_read_ID(){ //can be readed corectly only when spi mem page = 0, otherwise receive 0x00;
    BME680.ID =  swap_and_align(BME680_exchange_data(ID_ADD | 0x80, 1), 2) & 0xff;
}