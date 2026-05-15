#include "settings.h"
#include "windowsVar.h"

void UserInterface(Windows_names_t window){
    
    switch(window){
        //ignore init window and do nthing till it will be changed to one window from below list
        case MAIN_WINDOW:{
            if(!Windows.background_updater){
                ILI9341_fill_color_DMA(DARK_GRAY); //fill screen
                //ili9341_draw_rect(20, 35, 80, 40, BLUE, 1);
                //ili9341_draw_rect(20, 35, 80, 40, GREEN, 0);     
                ili9341_draw_rect(55, 0, 185, 10, BLACK, 1); //date and time button
                Windows.background_updater = true;
            }
            else { //if background was drawed show other data         
                
            if(XPT2046_switch(1000, 3850, 3800, 4000)){//check data and time changing button status
                Windows.background_updater = false; //prepare to update screen
                Windows.Window = TIME_WINDOW;
            }
                    
            if(Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time){ // update data every second once
                uint16_t y = 0;
                ILI9341_draw_formatted_line(60, &y, WHITE, BLACK,  "%4d-%02d-%02d %02d:%02d:%02d", 
                        RTC_Date_and_Time.RTC_year + 2000, 
                        RTC_Date_and_Time.RTC_month, 
                        RTC_Date_and_Time.RTC_day, 
                        RTC_Date_and_Time.RTC_hour, 
                        RTC_Date_and_Time.RTC_minute, 
                        RTC_Date_and_Time.RTC_second);      
                y = 0;
                if(RTC_Date_and_Time.time_sync == NONE)
                    ILI9341_draw_formatted_line(190, &y, RED, BLACK,  "No fix  ");
                if(RTC_Date_and_Time.time_sync == GSM_sync)
                    ILI9341_draw_formatted_line(190, &y, YELLOW, BLACK,  "GSM fix ");
                if(RTC_Date_and_Time.time_sync == GNSS_sync)
                    ILI9341_draw_formatted_line(190, &y, GREEN, BLACK,  "GNSS fix");
                y = 85;    
                
                RTC_read_date_and_time();               
                BME680_read_t_p_rh();
                calculate_solar_position();
                apply_all_elevation_modifies();
                   
                ILI9341_draw_formatted_line(10, &y, WHITE, DARK_GRAY, "T:%3dC° P: %4dhPa Rh: %3d%", 
                        (BME680.temperature + 50) / 100, 
                        (BME680.pressure + 50) / 100, 
                        (BME680.humidity +500) /1000);
                ILI9341_draw_formatted_line(10, &y, WHITE, DARK_GRAY, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);                
                ILI9341_draw_formatted_line(10, &y, WHITE, DARK_GRAY, "Az: %3.02f El: %3.02f ElR:  %3.02f ElRA: %3.02f", solar_params.azimuth,  solar_params.elevation, solar_params.refracted_elevation, solar_params.elevated_refracted_elevation);
    
            }
            Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;         
        }

        }break;
        case TIME_WINDOW:
            if(!Windows.background_updater){
                ILI9341_fill_color_DMA(NAVY); //fill screen
                ili9341_draw_rect(0, 0, 50, 20, BLACK, 1); //date and time button
                uint16_t y = 6;
                ILI9341_draw_formatted_line(22, &y, WHITE, BLACK,  "<");
                
                ili9341_draw_rect(53, 0, 60, 20, YELLOW, 1); //date and time button
                y = 6;
                ILI9341_draw_formatted_line(60, &y, BLACK, YELLOW,  "MAN");
                
                ili9341_draw_rect(117, 0, 60, 20, GREEN, 1); //date and time button
                y = 6;
                ILI9341_draw_formatted_line(120, &y, RED, GREEN,  "GSM");
                
                ili9341_draw_rect(180, 0, 60, 20, ORANGE, 1); //date and time button
                y = 6;
                ILI9341_draw_formatted_line(190, &y, CYAN, ORANGE,  "GNSS");
                
                Windows.background_updater = true;
            }
            else{                
                if(XPT2046_switch(100, 900, 3700, 4000)){//check back button status
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = MAIN_WINDOW;
                }
                if(Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time){ //update every second
                    RTC_read_date_and_time();
                                       
                    uint16_t y = 30;
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "RTC TIME: %4d-%02d-%02d %02d:%02d:%02d", RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day,RTC_Date_and_Time.RTC_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second );
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "TIME ZONE: %d", RTC_Date_and_Time.RTC_time_zone);
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "DAYLIGHT SAVING TIME: %s", is_daylight_saving_time(RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day,RTC_Date_and_Time.RTC_hour)? "YES":"NO");

                    y = 80;
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "GSM TIME lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GSM_year + 2000, A7672EGSM.GSM_month, A7672EGSM.GSM_day, A7672EGSM.GSM_hour, A7672EGSM.GSM_minute, A7672EGSM.GSM_second); 
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "GNSS lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GNSS_year + 2000, A7672EGSM.GNSS_month, A7672EGSM.GNSS_day, A7672EGSM.GNSS_hour, A7672EGSM.GNSS_minute, A7672EGSM.GNSS_second); 
                    ILI9341_draw_formatted_line(0, &y, WHITE, DARK_GRAY, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2); 
                }
                Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
            }
        break;
        case SENSOR_WINDOW:
        break;
        case TOWER_WINDOW:
        break;
        case SETTINGS_WINDOW:
        break;
    }
}

