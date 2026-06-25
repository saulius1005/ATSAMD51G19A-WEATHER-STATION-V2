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
            
            if(XPT2046_switch(1000, 3850, 3800, 4000)){//check data and time changing button status
                Windows.background_updater = false; //prepare to update screen
                Windows.Window = TIME_WINDOW;
                Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open
            }            
        }

        }break;
        case TIME_WINDOW:{
            static uint8_t DT[14] = {0};
            static uint16_t MANdateY = 0;
            static uint8_t MANdateM = 0;
            static uint8_t MANdateD = 0;
            static uint8_t MANtimeH = 0;
            static uint8_t MANtimem = 0;
            static uint8_t MANtimeS = 0;
            static uint8_t pressCount = 0;
            static uint8_t countProtection = 55;
            
            if(!Windows.background_updater){
                ILI9341_fill_color_DMA(NAVY); //fill screen
                keyboard.background_color = NAVY; //use same color for keyboard if not used
                
                /*uint16_t MANdateY = RTC_Date_and_Time.RTC_year + 2000;
                uint8_t MANdateM = RTC_Date_and_Time.RTC_month;
                uint8_t MANdateD = RTC_Date_and_Time.RTC_day;
                uint8_t MANtimeH = RTC_Date_and_Time.RTC_hour;
                uint8_t MANtimem = RTC_Date_and_Time.RTC_minute;
                uint8_t MANtimeS = RTC_Date_and_Time.RTC_second;*/
                //draw buttons
                ili9341_draw_rect(0, 0, 60, 20, BLACK, 1); //date and time button
                uint16_t y = 6;
                ILI9341_draw_formatted_line(27, &y, WHITE, BLACK,  "<");
                
                ili9341_draw_rect(60, 0, 60, 20, YELLOW, 1); //date and time button
                y = 6;
                ILI9341_draw_formatted_line(81, &y, BLACK, YELLOW,  "MAN");
                
                ili9341_draw_rect(120, 0, 60, 20, GREEN, 1); //date and time button
                y = 6;
                ILI9341_draw_formatted_line(141, &y, RED, GREEN,  "GSM");
                
                ili9341_draw_rect(180, 0, 60, 20, ORANGE, 1); //date and time button
                y = 6;
                ILI9341_draw_formatted_line(198, &y, CYAN, ORANGE,  "GNSS");
                
                Windows.background_updater = true;
            }
            else{                                
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
                
                draw_keyboard(Windows.keyboardAction); //draw keyboard if opened  
                
                if(Windows.keyboardAction == OPEN){ //if keyboard is open use fast data update
                    uint16_t y = 130;
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "MAN TIME: ");

                uint16_t x = 60; //start roughly after "man time: " text

                for(uint16_t clr = 0; clr < 14; clr++){
                    y -= 12; // keep the same line

                    uint16_t foreground = GREEN;
                    uint16_t background = (pressCount == clr) ? RED : NAVY;

                    ILI9341_draw_formatted_line(x, &y, foreground, background, "%01d", DT[clr]);
                    x += 6;
                    if(clr == 3 || clr == 5){
                        y -= 12; // keep the same line
                        ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, "-"); //after year and month
                        x += 6;
                    }
                    else if(clr == 7){
                        y -= 12; // keep the same line
                        ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, " "); //after day
                        x += 6;
                    }
                    else if(clr == 9 || clr == 11){
                        y -= 12; // keep the same line
                        ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, ":"); //after hour and minutes
                        x += 6;
                    }
                }
                    if(Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL)// protection from holding touch keyboard button
                        countProtection = 55;
                
                    for(uint8_t i = 0; i<16; i++){ //searching where is pressed on the touchscreen (find one place of 16 button maps)
                        if((Read_XPT2046.X >= keysMap.keyboard_buttons[i].X0) && (Read_XPT2046.X <= keysMap.keyboard_buttons[i].X1) && (Read_XPT2046.Y >= keysMap.keyboard_buttons[i].Y0) && (Read_XPT2046.Y <= keysMap.keyboard_buttons[i].Y1) && (Read_XPT2046.Z1 >= XPT_PRES_STRENGTH_LVL)){
                            if(countProtection != pressCount)
                            {
                                Windows.once_per_second_update += 1; //reset update to show data instantly
                                if(keysMap.keyboard_buttons[i].digit <= 9){ //check if it is digit
                                    DT[pressCount] = keysMap.keyboard_buttons[i].digit;  
                                    pressCount++;

                                }

                                if(pressCount == 14 && keysMap.keyboard_buttons[i].value == '>' ){
                                    ILI9341_draw_formatted_line(150, &y, GREEN, NAVY,  " typo save ");
                                }           
                                else if((pressCount > 0) && (keysMap.keyboard_buttons[i].value == '<')){
                                    pressCount--;
                                    ILI9341_draw_formatted_line(150, &y, GREEN, NAVY,  "typo back");
                                }
                                else if((pressCount > 0) && (keysMap.keyboard_buttons[i].value == 'x')){
                                    for(uint8_t x = 0; x < 14; x++){
                                        DT[pressCount] = 0; 
                                    }                                           
                                    pressCount = 0;
                                    ILI9341_draw_formatted_line(150, &y, GREEN, NAVY,  "typo delete");
                                }
                                countProtection = pressCount;
                            }
                        }                        
                    }
                    

                    
                }            
                //set touch maps for buttons
                if(XPT2046_switch(100, 916, 3700, 4000)){//check back button status
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = MAIN_WINDOW;
                    Windows.keyboardAction = CLOSE;// close keyboard when leaving window
                    Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open
                } 
                else if (XPT2046_switch(1016, 1932, 3700, 4000)){ //check manually time edit button
                    Windows.once_per_second_update = 0; //reset update to show data instantly
                    Windows.keyboardAction ^= 1; //toggle switch for keyboard                   
                }
                
                
            }
        }break;
        case SENSOR_WINDOW:
        break;
        case TOWER_WINDOW:
        break;
        case SETTINGS_WINDOW:
        break;
    }
}

