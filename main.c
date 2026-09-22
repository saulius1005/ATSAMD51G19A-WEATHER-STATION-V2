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
    GCLK3_SERCOM_TC_core_init(); //clock core 1Mhz for TC0-us (16bit) and TC2-ms(32bit)
    SERCOM_init(SPI_SCREEN);                    // Initialize SERCOM0 peripheral in SPI master mode
    SERCOM_init(RS485_SENSOR);
    SERCOM_init(RS485_TOWER);
    SERCOM_init(USART_GSM);
    TC0_init();    
    TCC0_init(); //initialization of timer for constant GSM signal strength and registration in network regular check
    RTC_init_calendar();
    ILI9341_CS_HIGH();
    XPT2046_CS_HIGH();
    ILI9341_init_simple_32b();           // Initialize ILI9341 LCD in 32-bit transfer mode
    DMA_init();                          // Initialize DMA controller and global descriptors
    DMA_SERCOM0_TX_init();               // Configure DMA channel for SERCOM0 SPI TX transfers
    DMA_USART_RS485_RX_init(GSM_CH);               // Configure DMA channel for SERCOM3 USART RX
    DMA_USART_RS485_RX_init(TOWER_CH);
    DMA_USART_RS485_RX_init(SENSORS_CH);
    EEPROM_Check();
    TCC0_ON(1000); //set interval every 1ms
    
    A7672E_ENABLE();

    while(1){
        A7672EInit(); //SIMCOM A7672E initialization active until reach WORK mode
        A7672E_GO_WORK();
        XPT2046_Read_All(); //checking touch screen
        UserInterface(Windows.Window); //after initialization show main window     
        
        Tower_COM();//send data to test towers?
        Sensors_COM();// checking enveroment data
                
        
        RTC_Date_and_Time.RTC_sys_time = RTC_read_sys_time(); //read system time                       
        TC0_CHECKER(); //check tc0 timeout
        TCC0_CHECKER(); //check tcc0 timeout checking gsm signal strength and registration in network status
        

    }
}

