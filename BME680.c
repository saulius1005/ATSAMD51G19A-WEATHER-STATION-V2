#include "settings.h"
#include "BME680Var.h"

void BME680_write(uint32_t cmd, uint8_t length){ //max 4 bytes
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(length) | SERCOM_SPIM_LENGTH_LENEN_Msk;
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    
    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM0_REGS->SPIM.SERCOM_DATA = swap_and_align(cmd, length);

    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));
}

uint32_t BME680_exchange_data(uint32_t cmd, uint8_t tx_length){
   
    uint32_t rx = 0;
    BME680_CS_LOW();
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(tx_length+1) | SERCOM_SPIM_LENGTH_LENEN(1); // all time +1 of dummy
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM4_REGS->SPIM.SERCOM_DATA = swap_and_align(cmd, tx_length);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));
    rx = SERCOM4_REGS->SPIM.SERCOM_DATA;   
    BME680_CS_HIGH();  

    return rx>>8;//remove trash (first byte)
}