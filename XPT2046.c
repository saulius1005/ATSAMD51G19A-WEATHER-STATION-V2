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
    if(A7672E_init.status != WORK) // if GSM module still not initialized skip further code
        return;
    
    if(!Read_XPT2046.speed){ //switch only it is ili9341 spi speed
        SPI0_Baud_Switch(1500000);//switch baudrate to 1.5Mhz      
    }
    XPT2046_CS_LOW();
    
    switch(Read_XPT2046.state){
        case SET_DEVICE:
                TC0_ON(100000); //check touch screen every 100ms (~10 time /s)
                Read_XPT2046.state = WAIT_DEVICE;
        break;
        case WAIT_DEVICE:
            if(TC0_timeout){
                Read_XPT2046.Z1 = XPT2046_Read(XPT_CMD_Z1); //every time read Z1                  
                if(Read_XPT2046.Z1 >= XPT_PRES_STRENGTH_LVL){ // if it is pressed only then read                 
                     Read_XPT2046.X = XPT2046_Read(XPT_CMD_X); //x    
                     Read_XPT2046.Y = XPT2046_Read(XPT_CMD_Y); //and y
                }               
                Read_XPT2046.state = SET_DEVICE;
            }            
        break;          
    }
    XPT2046_CS_HIGH();  
    
    if(!screen_sleep.sleep){ //if sleeping do not turn to ili9341 spi speed
        SPI0_Baud_Switch(30000000);//switch baudrate back to 30Mhz
        Read_XPT2046.speed = false;
    }
    SERCOM0_REGS->SPIM.SERCOM_LENGTH &= ~SERCOM_SPIM_LENGTH_LENEN_Msk;
}

bool XPT2046_switch(uint16_t X0, uint16_t X1, uint16_t Y0, uint16_t Y1){
  
    if (Read_XPT2046.Z1 >= XPT_PRES_STRENGTH_LVL) {// first check if touch pressure is sufficient
        bool touched = (Read_XPT2046.X >= X0 && Read_XPT2046.X < X1 && Read_XPT2046.Y >= Y0 &&  Read_XPT2046.Y < Y1); //the check x y 

        if (!Read_XPT2046.pressed && touched) { //if all ok
            Read_XPT2046.pressed = true;
            return true;
        }
    }
    else {      
        Read_XPT2046.pressed = false;//if too low release
    }
    return false;
}


