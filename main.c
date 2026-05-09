/* 
 * File:   main.c
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, sausio 29, 14.22
 */

#include "settings.h"

int main(void) {
    
    cpu_120Mhz_DPLL0_XOSC1_init();       // Configure CPU clock to 128 MHz using DPLL0 with XOSC1 source   
    GPIO_init();                         // Initialize all required GPIO pins (SPI, LCD control, etc.)
    GCLK1_SERCOM_SPIM_core_init();
    GCLK2_SERCOM_I2CM_USARTM_core_init();
    GCLK3_SERCOM_TC_core_init(); //clock core 1Mhz for TC0-us (16bit) and TC2-ms(32bit)
    SERCOM_init(SPI_SCREEN);                    // Initialize SERCOM0 peripheral in SPI master mode
    SERCOM_init(SPI_SENSOR);
    //SERCOM_init(I2C);
    SERCOM_init(USART);
    TC0_init();    
    TC2_init(); 
    RTC_init_calendar();
    ILI9341_CS_HIGH();
    XPT2046_CS_HIGH();
    ILI9341_init_simple_32b();           // Initialize ILI9341 LCD in 32-bit transfer mode
    DMA_init();                          // Initialize DMA controller and global descriptors
    DMA_SERCOM0_TX_init();               // Configure DMA channel for SERCOM0 SPI TX transfers
    DMA_SERCOM3_RX_init();               // Configure DMA channel for SERCOM3 USART RX
    
    uint16_t y = 1;      

    //ILI9341_draw_image_DMA(sunflower); //draw sunflower
    //delay_ms(500); 
    //ILI9341_draw_image_DMA(windmill); //draw windmill
    //delay_ms(500);  

     Source_data touch_areas[] = { //for blue square button- touch map
    {448, 1660, 3168, 3552, 100, 0, BTN_0} //x0, x1, y0, y1, z1(z0),keypad closed, id
    };

        
    while(1){
        A7672EInit(); //SIMCOM A7672E initialization active until reach WORK mode
        A7672ReadNEMAGNSS(); //read SIMCOM A7672E GNNS and GSM time (interrupt+ machine state) active when A7672E_init.status == WORK              
        source(&touch_areas[0]);//read touch screen. Keyboard image control by pressing blue square  
        UserInterface(Windows.Window); //after initialization show main window
        RTC_date_and_time_update(); //update rtc time with gsm or gnss 
        RTC_Date_and_Time.RTC_sys_time = RTC_read_sys_time(); //read system time                
        TC0_CHECKER(); //check tc0 timeout
        TC2_CHECKER(); //check tc2 timeout

    }
}

