#include "settings.h"
#include "XPT2046Var.h"

uint16_t XPT2046_Read(uint32_t cmd){
    
    SPI0_Baud_Switch(2000000);//switch baudrate to 2.5Mhz
    
    uint32_t rx;
    XPT2046_CS_LOW();
    // 3 baitai: CMD + 2 dummy
    SERCOM0_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(3) | SERCOM_SPIM_LENGTH_LENEN(1);

    while (SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);

    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM0_REGS->SPIM.SERCOM_DATA = cmd;

    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));

    rx = SERCOM0_REGS->SPIM.SERCOM_DATA;
    
    XPT2046_CS_HIGH();  
      
    SPI0_Baud_Switch(30000000);//switch baudrate back to 30Mhz


    return (rx >> 3) & 0x0FFF;//12b
    //return (rx >> 8) & 0xFF;//8b
}
