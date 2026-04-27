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
    GCLK2_SERCOM_I2CM_core_init();
    GCLK3_SERCOM_TC_core_init(); //clock core 1Mhz for TC0-us (16bit) and TC2-ms(32bit)
    SERCOM_init(SPI_SCREEN);                    // Initialize SERCOM0 peripheral in SPI master mode
    SERCOM_init(SPI_SENSOR);
    SERCOM_init(I2C);
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
    {544, 1660, 3232, 3744, 100, 0, BTN_0} //x0, x1, y0, y1, z1(z0),keypad closed, id
    };
           
    BMP180_ReadCalibration(&BMP180);
    uint32_t time_test = 0;
        
    while(1){
        A7672EInit(); //SIMCOM A7672E initialization active until reach WORK mode
        BMP180_Task(); //read BMP180 temp and pressure (interrupt+ machine state) active when A7672E_init.status == WORK
        A7672ReadNEMAGNSS(); //read SIMCOM A7672E GNNS and GSM time (interrupt+ machine state) active when A7672E_init.status == WORK              
        source(&touch_areas[0]);//read touch screen. Keyboard image control by pressing blue square  
        
        RTC_Date_and_Time.RTC_sys_time = RTC_read_sys_time();        
        if(A7672E_init.status == WORK){           
            y = 85;        
            if(time_test != RTC_Date_and_Time.RTC_sys_time){
                draw_formatted_line(10, &y, WHITE, RED, "BMP180 T: %2d.%01d, P: %4d.%02dhPa", BMP180.Temperature / 10, BMP180.Temperature % 10, BMP180.Pressure / 100, BMP180.Pressure % 100);//y145
                BMP180.DataReady = false; //repeat readings
                draw_formatted_line(10, &y, WHITE, RED, "GPS lock at: %02d%02d%02d %02d%02d%02d", A7672EGSM.GNSS_year, A7672EGSM.GNSS_month, A7672EGSM.GNSS_day, A7672EGSM.GNSS_hour, A7672EGSM.GNSS_minute, A7672EGSM.GNSS_second);
                draw_formatted_line(10, &y, WHITE, RED, "GSM lock at: %02d%02d%02d %02d%02d%02d", A7672EGSM.GSM_year, A7672EGSM.GSM_month, A7672EGSM.GSM_day, A7672EGSM.GSM_hour, A7672EGSM.GSM_minute, A7672EGSM.GSM_second);     
                RTC_read_date_and_time();
                draw_formatted_line(10, &y, WHITE, RED,  "RTC date and time: %02d%02d%02d %02d%02d%02d", RTC_Date_and_Time.RTC_year, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second); 
                draw_formatted_line(10, &y, WHITE, RED,  "sys time: %lu", time_test);       
                draw_formatted_line(10, &y, WHITE, RED,  "time sync value: %d", RTC_Date_and_Time.time_sync);
                
                BME680_Ctrl_hum(oversampling_x16, false); //humidity oversample and interrupt off 
                BME680_Ctrl_meas(oversampling_x16, oversampling_x16, forced_mode);
                       
                
                BME680_calculate_humidity();                
                BME680_calculate_temperature();
                BME680_calculate_pressure();


                BME680_Config(Filter_coef_127, false); //if page 0 swap it to 1, then change filter value to 15 and keep spi 3wire mode disabled            
                draw_formatted_line(10, &y, WHITE, RED, "BME T:%3d.%02d P:%4d.%02dhPa Rh: %d.%03d ", BME680.temperature/100, BME680.temperature%100, BME680.pressure / 100, BME680.pressure % 100, BME680.humidity/1000, BME680.humidity%1000);
                draw_formatted_line(10, &y, WHITE, RED, "cal: %d, %d, %d, %d, %d, %d, %d, raw: %X", 
                        BME680.calibration_data.par_h1,
                        BME680.calibration_data.par_h2,
                        BME680.calibration_data.par_h3,
                        BME680.calibration_data.par_h4,
                        BME680.calibration_data.par_h5,
                        BME680.calibration_data.par_h6,
                        BME680.calibration_data.par_h7,
                        BME680.hum);
            }
            time_test = RTC_Date_and_Time.RTC_sys_time;
            RTC_date_and_time_update(); //update rtc time with gsm or gnss          
        }
        
        TC0_CHECKER(); //check tc0 timeout
        TC2_CHECKER(); //check tc2 timeout

    }
}

