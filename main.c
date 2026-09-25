/* 
 * File:   main.c
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, sausio 29, 14.22
 */

#include "MCU_configs.h"
#include "settings.h"

int main(void) {
    
    cpu_120Mhz_DPLL0_XOSC1_init();       // Configure CPU clock to 128 MHz using DPLL0 with XOSC1 source   
    Towers_init(); //create empty towers list
    GPIO_init();                         // Initialize all required GPIO pins (SPI, LCD control, etc.)
    A7672E_DISABLE();
    GCLK1_SERCOM_SPIM_core_init();
    GCLK2_SERCOM_USARTM_core_init();
    GCLK3_SERCOM_TC_core_init();
    SERCOM_init_all();
    TC0_init();  
    TCC0_init(); //initialization of timer for constant GSM signal strength and registration in network regular check
    RTC_init_calendar();
    ILI9341_CS_HIGH();
    XPT2046_CS_HIGH();
    ILI9341_init();           // Initialize ILI9341 LCD in 32-bit transfer mode
    DMA_init_all();
    EEPROM_Check();
    TCC0_ON(1000); //set interval every 1ms
    
    A7672E_ENABLE();

    while(1){
        A7672EInit(); //SIMCOM A7672E initialization active until reach WORK mode
        A7672E_GO_WORK(); //sending requests for signal strength, registration, send data to server
        XPT2046_Read_All(); //checking touch screen
        UserInterface(Windows.Window); //show windows all controll from touch screen     
        
        Tower_COM();//read sensors and send datao to towers                             
        RTC_Date_and_Time.RTC_sys_time = RTC_read_sys_time(); //read system time    
                     
        TC0_CHECKER(); //check tc0 timeout (A7672E init and ili9341 screen init)
        TCC0_CHECKER(); //check tcc0 timeout to update data from gsm module, sensors, towers, server
        ili9341_sleep(); //check if screen is not touced some time if so go to sleep or waking up       
    }
}

