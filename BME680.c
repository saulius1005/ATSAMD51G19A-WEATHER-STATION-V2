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
    uint32_t cmd = ((uint32_t) status_ADD << 16) | ((uint16_t) (page == BME680_page_1 ? 0x10 : 0x00) << 8) | BME680_SPI_ReadAddr(status_ADD) ; //set write add (0x73) | set page (0x00 or 0x10) | set read add (0xF3)     
    BME680.STATUS_spi_mem_page = ((swap_and_align(BME680_exchange_data(cmd, 3), 4) & 16) >> 4) == BME680_page_1 ? BME680_page_1 : BME680_page_0; //swap bytes in places from answer total received is 4 bytes, then if 1st page return 1 else 0
}

void BME680_read_ID(){ //can be readed corectly only when spi mem page = 0, otherwise receive 0x00;
    if(BME680.STATUS_spi_mem_page != 0){ //if page 1 change it to 0
        BME680_change_page(BME680_page_0);
    }
    BME680.ID =  swap_and_align(BME680_exchange_data(BME680_SPI_ReadAddr(ID_ADD), 1), 2) & 0xff;
}

void BME680_reset(){ //reset sensor same as power up reset. Requaired manual BME680.RESET value change to false if neede use it again
    if(BME680.RESET)//if already reset do nothing
        return;
    if(BME680.STATUS_spi_mem_page != 0){ //if page 1 change it to 0
        BME680_change_page(BME680_page_0);
    }
    BME680_exchange_data(((uint16_t)Reset_ADD<<16) | BME680_RESET_value, 2); //ignore what it returns
    BME680.RESET = true;
}

void BME680_Config(BME680_filter_t filter, bool spi_3w_en){ //write filter and spi3wire enable values to register 
    
    if((filter == BME680.Config.filter) && (spi_3w_en == BME680.Config.spi_3w_en)) //if filter and spi 3w eneable the same skip further code
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
     uint32_t cmd = ((uint32_t) Config_ADD << 16) | ((uint16_t)filter << 10) | ((uint16_t)spi_3w_en<<8)  | BME680_SPI_ReadAddr(Config_ADD);
     uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
     BME680.Config.filter = (answer >> 2) & 7;
     BME680.Config.spi_3w_en = answer & 1;
}

void BME680_Ctrl_meas(BME680_meas_os_t os_t, BME680_meas_os_t os_p, BME680_mode_t mode){ //write ctrl meas register
    if((os_t == BME680.Ctrl_meas.osrs_t) && (os_p == BME680.Ctrl_meas.osrs_p) && (mode == BME680.Ctrl_meas.mode))
        return;
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    uint32_t cmd = ((uint32_t) Ctrl_meas_ADD << 16) | ((uint16_t)os_t << 13) | ((uint16_t)os_p << 10) | ((uint16_t)mode << 8) | BME680_SPI_ReadAddr(Ctrl_meas_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    
    BME680.Ctrl_meas.osrs_t = (answer >> 5) & 7;
    BME680.Ctrl_meas.osrs_p = (answer >> 2) & 7;
    BME680.Ctrl_meas.mode = answer & 3;
}