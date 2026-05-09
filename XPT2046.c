#include "settings.h"
#include "XPT2046Var.h"


uint16_t XPT2046_Read(uint32_t cmd){
    
    uint32_t rx;
    // 3 baitai: CMD + 2 dummy
    SERCOM0_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(3) | SERCOM_SPIM_LENGTH_LENEN(1);
    while (SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));
    SERCOM0_REGS->SPIM.SERCOM_DATA = cmd;

    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));
    rx = SERCOM0_REGS->SPIM.SERCOM_DATA;
      
    return (rx >> 3) & 0xFFF;//12b
}

void XPT2046_Read_All(){
    SPI0_Baud_Switch(2000000);//switch baudrate to 0.1Mhz
    XPT2046_CS_LOW();
    
    switch(Read_XPT2046.state){
        case SET:
                TC0_ON(5000); //set timeout 5ms
                Read_XPT2046.state = WAIT;
        break;
        case WAIT:
            if(TC0_timeout){
                if(Read_XPT2046.step == 0){
                    Read_XPT2046.X = XPT2046_Read(XPT_CMD_X);
                }
                else if(Read_XPT2046.step == 1){
                    Read_XPT2046.Y = XPT2046_Read(XPT_CMD_Y);
                }
                else if(Read_XPT2046.step == 2){
                    Read_XPT2046.Z1 = XPT2046_Read(XPT_CMD_Z1);
                }
                else if(Read_XPT2046.step == 3){
                    Read_XPT2046.Z2 = XPT2046_Read(XPT_CMD_Z2);                   
                }                
                Read_XPT2046.state = DONE;
            }            
        break;
        case DONE:       
            if(Read_XPT2046.step++ == 4)
                Read_XPT2046.step = 0;
            Read_XPT2046.state = SET;
            
        break;           
    }
    XPT2046_CS_HIGH();  
    
    SPI0_Baud_Switch(30000000);//switch baudrate back to 30Mhz
    SERCOM0_REGS->SPIM.SERCOM_LENGTH &= ~SERCOM_SPIM_LENGTH_LENEN_Msk;
}
