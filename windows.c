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
                if(RTC_Date_and_Time.time_sync == NONE_sync)
                    ILI9341_draw_formatted_line(190, &y, RED, BLACK,  "No fix  ");
                else if(RTC_Date_and_Time.time_sync == GSM_sync)
                    ILI9341_draw_formatted_line(190, &y, YELLOW, BLACK,  "GSM fix ");
                else if(RTC_Date_and_Time.time_sync == GNSS_sync)
                    ILI9341_draw_formatted_line(190, &y, GREEN, BLACK,  "GNSS fix");
                else
                    ILI9341_draw_formatted_line(190, &y, GREEN, BLACK,  "Manual  ");
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
        static uint8_t DT[14] = {2, 0};//set year 20...
        static uint8_t TZ[2] = {0, 2}; //set time zone to default value of +02 (range -12 +14)
        static char TZside = '+'; //time zone direction symbol
        static uint8_t pressCount = 2; 
        static uint8_t countProtection = 55;

        if(!Windows.background_updater){ //drawing not chaangig elements
            ILI9341_fill_color_DMA(NAVY); //background
            keyboard.background_color = NAVY; 

            ili9341_draw_rect(0, 0, 60, 20, BLACK, 1); //and buttons frames
            uint16_t y = 6;
            ILI9341_draw_formatted_line(27, &y, WHITE, BLACK,  "<");

            ili9341_draw_rect(60, 0, 60, 20, YELLOW, 1);
            y = 6;
            ILI9341_draw_formatted_line(81, &y, BLACK, YELLOW,  "MAN");

            ili9341_draw_rect(120, 0, 60, 20, GREEN, 1);
            y = 6;
            ILI9341_draw_formatted_line(141, &y, RED, GREEN,  "GSM");

            ili9341_draw_rect(180, 0, 60, 20, ORANGE, 1);
            y = 6;
            ILI9341_draw_formatted_line(198, &y, CYAN, ORANGE,  "GNSS");

            Windows.background_updater = true;
        }
        else{                                
            if(Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time){ 
                RTC_read_date_and_time();

                uint16_t y = 30;
                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "RTC time: %4d-%02d-%02d %02d:%02d:%02d", RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day,RTC_Date_and_Time.RTC_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second );
                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "T.Z.: %2d ", RTC_Date_and_Time.RTC_time_zone);
                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "D.S.T: %s", is_daylight_saving_time(RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day,RTC_Date_and_Time.RTC_hour)? "YES":"NO");

                const char *timesourceintext =
                (RTC_Date_and_Time.time_sync == NONE_sync) ? "NONE        " :
                (RTC_Date_and_Time.time_sync == GSM_sync)  ? "GSM Module  " :
                (RTC_Date_and_Time.time_sync == GNSS_sync) ? "GNSS Module " : "Manual input";
                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "Date/time source: %s", timesourceintext);

                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "GSM lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GSM_year + 2000, A7672EGSM.GSM_month, A7672EGSM.GSM_day, A7672EGSM.GSM_hour, A7672EGSM.GSM_minute, A7672EGSM.GSM_second); 
                if(A7672EGSM.GNSS_year + A7672EGSM.GNSS_month + A7672EGSM.GNSS_day + A7672EGSM.GNSS_hour + A7672EGSM.GNSS_minute + A7672EGSM.GNSS_second == 0)
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "GNSS lock at: no GNSS lock");
                else
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "GNSS lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GNSS_year + 2000, A7672EGSM.GNSS_month, A7672EGSM.GNSS_day, A7672EGSM.GNSS_hour, A7672EGSM.GNSS_minute, A7672EGSM.GNSS_second); 
                ILI9341_draw_formatted_line(0, &y, WHITE, DARK_GRAY, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2); 

                Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
            }

            draw_keyboard(Windows.keyboardAction);   

            if(Windows.keyboardAction == OPEN){ 
                uint16_t y = 130;
                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "MAN TIME: ");

                uint16_t x = 60;
                for(uint16_t clr = 0; clr < 17; clr++){ 
                    y = 130;
                    uint16_t background = (pressCount == clr) ? RED : NAVY;

                    if((clr < 14))
                        ILI9341_draw_formatted_line(x, &y, GREEN, background, "%01d", DT[clr]); //draw digits of the date and time
                    else if(clr == 14)
                        ILI9341_draw_formatted_line(x, &y, GREEN, background, "%c", TZside); //draw time zone direction
                    else if(clr > 14 && clr < 17)
                        ILI9341_draw_formatted_line(x, &y, GREEN, background, "%01d", TZ[clr-15]); //draw digits of the time zone
                    x += 6;
                    if(clr == 3 || clr == 5) {//Year-Month-Day
                        y = 130; ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, "-"); x += 6;
                    } else if(clr == 7) {//Year-Month-Day //space
                        y = 130; ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, " "); x += 6;
                    } else if(clr == 9 || clr == 11) {//Year-Month-Day Hour:Minutes:Seconds
                        y = 130; ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, ":"); x += 6;
                    } else if(clr == 13) {//Year-Month-Day Hour:Minutes:Seconds //space
                        y = 130; ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, " "); x += 6;
                    }
                }

                if(Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL) //infinite press protection
                    countProtection = 55; 

                if (Read_XPT2046.Z1 >= XPT_PRES_STRENGTH_LVL) { // if touch pressing hard enough
                    for(uint8_t i = 0; i < 16; i++){ //find where is pressing
                        if((Read_XPT2046.X >= keysMap.keyboard_buttons[i].X0) && (Read_XPT2046.X <= keysMap.keyboard_buttons[i].X1) &&  (Read_XPT2046.Y >= keysMap.keyboard_buttons[i].Y0) && (Read_XPT2046.Y <= keysMap.keyboard_buttons[i].Y1)) {
                            if(countProtection != pressCount) { //accept only once and one symbol per pressing
                                Windows.once_per_second_update += 1;
                                if(keysMap.keyboard_buttons[i].digit <= 9){ 
                                    if(pressCount < 14){ //changing Date and Time digits
                                        DT[pressCount] = keysMap.keyboard_buttons[i].digit;  
                                        pressCount++;
                                    }
                                    else if( pressCount <= 16 ){
                                        TZ[pressCount - 15] = keysMap.keyboard_buttons[i].digit; //change time zone digits  
                                        pressCount++;
                                    }  
                                }
                                else if(pressCount == 14 && ((keysMap.keyboard_buttons[i].value == '-')||(keysMap.keyboard_buttons[i].value == '+'))){ //only if changing time zone and using + or - symbols
                                    pressCount ++;
                                    TZside = keysMap.keyboard_buttons[i].value;
                                }

                                if(pressCount == 17 && keysMap.keyboard_buttons[i].value == '>' ){
                                    uint8_t yy = (DT[2] * 10) + DT[3];  
                                    uint8_t MM = (DT[4] * 10) + DT[5]; 
                                    uint8_t dd = (DT[6] * 10) + DT[7]; 
                                    uint8_t hh = (DT[8] * 10) + DT[9]; 
                                    uint8_t mm = (DT[10] * 10) + DT[11]; 
                                    uint8_t ss = (DT[12] * 10) + DT[13]; 

                                    y = 130;
                                    int8_t tzcheck = (TZ[0] * 10) + TZ[1];
                                    if(TZside == '-')// if time zone is negative
                                        tzcheck = 0 - tzcheck; //update time zone
                                    
                                    if(is_time_correct(yy, MM, dd, hh, mm, ss, false) && ( (tzcheck >= -12) && (tzcheck <= 14)) ){
                                        ILI9341_draw_formatted_line(204, &y, GREEN, NAVY,"SAVED");                                       
                                        RTC_Date_and_Time.RTC_time_zone =  tzcheck; //update time zone
                                        
                                        apply_timezone(&yy, &MM, &dd, &hh, RTC_Date_and_Time.RTC_time_zone); 
                                        RTC_date_and_time_sync(datetime_to_RTC_format(yy, MM, dd, hh, mm, ss), MAN_sync);
                                    } else { 
                                        ILI9341_draw_formatted_line(204, &y, RED, NAVY,  "ERROR");
                                    }
                                }            
                                else if((pressCount > 2) && (keysMap.keyboard_buttons[i].value == '<')){
                                    pressCount--; 
                                }
                                else if(keysMap.keyboard_buttons[i].value == 'x'){ //clear whole line and close keyboard
                                    for(uint8_t x = 2; x < 14; x++) DT[x] = 0;                                            
                                    pressCount = 2;
                                    uint16_t y = 130;
                                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,"                                             "); 
                                    Windows.Window = TIME_WINDOW;
                                    Windows.keyboardAction = CLOSE;  
                                    Windows.once_per_second_update = 88;//just  random digit
                                }
                                countProtection = pressCount;
                                break; // if button presset stoping for cycle and continue further
                            }
                        }                        
                    }  
                }
            }            

            // Touch button check
            if(XPT2046_switch(100, 916, 3700, 4000)){ // < 
                Windows.background_updater = false; 
                Windows.Window = MAIN_WINDOW;
                Windows.keyboardAction = CLOSE;
                Windows.once_per_second_update = 0; 
            } 
            else if (XPT2046_switch(1016, 1932, 3700, 4000)){  // MAN
                Windows.once_per_second_update = 0; 
                Windows.keyboardAction ^= 1;                    
            }
            else if (XPT2046_switch(2032, 2948, 3700, 4000)){ // GSM
                Windows.once_per_second_update = 0; 
                uint16_t y = 130;
                ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "           GSM time reset!            ");
                RTC_Date_and_Time.time_sync = NONE_sync;     
                RTC_Date_and_Time.RTC_sys_time = 0; 
                A7672E_work.source = GSM; 
                A7672E_work.cycle = false;
                A7672E_work.state = SET;
                if(Windows.keyboardAction == OPEN){ 
                    Windows.Window = TIME_WINDOW;
                    Windows.keyboardAction = CLOSE;
                    Windows.once_per_second_update = 89;                         
                }
            }
            else if (XPT2046_switch(3048, 3964, 3700, 4000)){ // GNSS
                Windows.once_per_second_update = 0; 
                uint16_t y = 130;
                if(A7672EGNSS.mode != 0){ 
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY,  "           GNSS time reset!          ");
                    RTC_Date_and_Time.time_sync = GSM_sync;      
                    A7672E_work.source = GNSS; 
                    A7672E_work.cycle = false;
                    A7672E_work.state = SET;                        
                }
                else{
                    ILI9341_draw_formatted_line(0, &y, RED, NAVY,  "              NO GNSS lock!            ");
                }
                if(Windows.keyboardAction == OPEN){ 
                    Windows.Window = TIME_WINDOW;
                    Windows.keyboardAction = CLOSE;
                    Windows.once_per_second_update = 89;                         
                }
            } 
        }
    } break;
        
        case SENSOR_WINDOW:
        break;
        case TOWER_WINDOW:
        break;
        case SETTINGS_WINDOW:
        break;
    }
}

