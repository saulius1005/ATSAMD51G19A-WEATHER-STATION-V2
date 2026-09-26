#include "settings.h"
#include "XPT2046Var.h"


uint16_t XPT2046_Read(uint32_t cmd){
    
    uint32_t rx;
    // 3 baitai: CMD + 2 dummy
    DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(3) | SERCOM_SPIM_LENGTH_LENEN(1); //dma does nothing just using same structure and spi channel
    while (DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    while (!(DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));
    DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_DATA = cmd;

    while (!(DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));
    rx = DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_DATA;
      
    return (rx >> 3) & 0xFFF;//12b
}

void XPT2046_Read_All(){    
    if(A7672E_init.status != WORK) // if GSM module still not initialized skip further code
        return;
    
    if(!Read_XPT2046.speed){ //switch only it is ili9341 spi speed
        SPI0_Baud_Switch(2500000);//2.5Mhz, top is ~4Mhz (if it is too fast top of touch screen starts not respond and increasing speed unresponsive part is also increasing. start point X0 Y4095)      
        Read_XPT2046.speed = true; //if lcd is sleeping change spi speed once
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
                    screen_sleep.start_at = Periodic_Checker_Devices.period_counter; //and reset sleep start counter                   
                }               
                Read_XPT2046.state = SET_DEVICE;
            }            
        break;          
    }
    XPT2046_CS_HIGH();  
    
    if(!screen_sleep.sleep){ //if not sleeping switch spi speed to ili9341
        SPI0_Baud_Switch(30000000);//switch baudrate back to 30Mhz and top ~37Mhz
        Read_XPT2046.speed = false; //switch back flag to xpt speed
    }
    else if(XPT2046_switch(64,4000,64,4000) && screen_sleep.sleep){//if in sleep
        SPI0_Baud_Switch(30000000);//switch baudrate back to 30Mhz       
        Windows.once_per_second_update = 0; //update window flag
        Windows.background_updater = false; //update background update flag
        ili9341_CMD(0x29,1); //DISPON
        A7672E_LCD_BCKL_ON();// backlight on
        screen_sleep.sleep = false; //GO to WORK !    
    }
    DMA_devices[SPI_CH].SERCOM->SPIM.SERCOM_LENGTH &= ~SERCOM_SPIM_LENGTH_LENEN_Msk; //dma does nothing just same spi line
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


