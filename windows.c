#include "settings.h"
#include "windowsVar.h"

void UserInterface(Windows_names_t window){
    
    switch(window){
        //ignore init window and do nthing till it will be changed to one window from below list
        case MAIN_WINDOW:{
            if(!Windows.background_updater){
                ILI9341_fill_color_DMA(DARK_GRAY); //fill screen
                ili9341_draw_rect(20, 35, 80, 40, BLUE, 1);
                ili9341_draw_rect(20, 35, 80, 40, GREEN, 0);     
                Windows.background_updater = true;
            }
            else { //if background was drawed show other data         
                    
            if(Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time){ // update data every second once
                uint16_t y = 0;
                ILI9341_draw_formatted_line(60, &y, WHITE, DARK_GRAY,  "%4d-%02d-%02d %02d:%02d:%02d", 
                        RTC_Date_and_Time.RTC_year + 2000, 
                        RTC_Date_and_Time.RTC_month, 
                        RTC_Date_and_Time.RTC_day, 
                        RTC_Date_and_Time.RTC_hour, 
                        RTC_Date_and_Time.RTC_minute, 
                        RTC_Date_and_Time.RTC_second);      
                y = 0;
                if(RTC_Date_and_Time.time_sync == NONE)
                    ILI9341_draw_formatted_line(190, &y, RED, DARK_GRAY,  "No fix  ");
                if(RTC_Date_and_Time.time_sync == GSM_sync)
                    ILI9341_draw_formatted_line(190, &y, YELLOW, DARK_GRAY,  "GSM fix ");
                if(RTC_Date_and_Time.time_sync == GNSS_sync)
                    ILI9341_draw_formatted_line(190, &y, GREEN, DARK_GRAY,  "GNSS fix");
                y = 85;    
                
                RTC_read_date_and_time();               
                BME680_read_t_p_rh();
                   
                ILI9341_draw_formatted_line(10, &y, WHITE, DARK_GRAY, "T:%3dC° P: %4dhPa Rh: %3d%", 
                        (BME680.temperature + 50) / 100, 
                        (BME680.pressure + 50) / 100, 
                        (BME680.humidity +500) /1000);
                ILI9341_draw_formatted_line(10, &y, WHITE, DARK_GRAY, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);
    
            }
            Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;         
        }

        }break;
        case TIME_WINDOW:
        break;
        case SENSOR_WINDOW:
        break;
        case TOWER_WINDOW:
        break;
        case SETTINGS_WINDOW:
        break;
    }
}

