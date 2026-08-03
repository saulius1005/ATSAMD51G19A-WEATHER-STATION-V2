#include "settings.h"
#include "windowsVar.h"

void UserInterface(Windows_names_t window){
    
    switch(window){
        //ignore init window and do nthing till it will be changed to one window from below list
        case MAIN_WINDOW:{
            if(!Windows.background_updater){
                ILI9341_fill_color_DMA(BLACK); //fill screen 
                ili9341_draw_rect(0, 0, 240, 16, WHITE, 0); //date and time button
                
                ili9341_draw_rect(0, 20, 115, 80, CYAN, 0); //weather data
                ili9341_draw_rect(117, 20, 123, 80, YELLOW, 0); //sun data
                
                ili9341_draw_rect(0, 104, 115, 80, GREEN, 0); //location data
                ili9341_draw_rect(117, 104, 123, 80, TEAL, 0); //gsm/gnss data
                
                ili9341_draw_rect(0, 188, 240, 100, MISTYROSE, 0); //towers data
                
                Windows.background_updater = true;
            }
            else { //if background was drawed show other data         
                
            ADC0_read(WIND_SPEED); //read wind speed to catch gust of wind
                    
            if(Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time){ // update data every second once
                uint16_t y = 4;
                ILI9341_draw_formatted_line(60, &y, WHITE, BLACK,  "%4d-%02d-%02d %02d:%02d:%02d", 
                    RTC_Date_and_Time.RTC_year + 2000, 
                    RTC_Date_and_Time.RTC_month, 
                    RTC_Date_and_Time.RTC_day, 
                    RTC_Date_and_Time.RTC_hour, 
                    RTC_Date_and_Time.RTC_minute, 
                    RTC_Date_and_Time.RTC_second);      
                y = 4;
                ILI9341_draw_formatted_line(190, &y, WHITE, BLACK,  "%s", 
                    (RTC_Date_and_Time.time_sync == NONE_sync) ? "No fix" : 
                    (RTC_Date_and_Time.time_sync == GSM_sync) ? "GSM fix " : 
                    (RTC_Date_and_Time.time_sync == GNSS_sync) ? "GNSS fix" : 
                    "Manual  ");
                
                RTC_read_date_and_time();               
                BME680_read_t_p_rh();
                calculate_solar_position();
                apply_all_elevation_modifies();
                
                y = 24;
                   
                ILI9341_draw_formatted_line(15, &y, CYAN, BLACK, "Weather Data:");
                y -=12;                 
                ILI9341_draw_formatted_line(145, &y, YELLOW, BLACK, "Sun Data:\n");      
                
                ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Temperature:%3dC° ", (BME680.temperature + 50) / 100);
                y -=12;
                ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Azimuth: %3.02f ", solar_params.azimuth);               
                
                ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Pressure: %4dhPa ", (BME680.pressure + 50) / 100);                
                y -=12;
                ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Elevation: %3.02f ", solar_params.elevated_refracted_elevation);     
                
                ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Humidity: %3d% ", (BME680.humidity + 500) / 1000);    
                
                ADC0_read(WIND_DIR); //read wind direction every second;
                ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Wind: %dm/s %s\n", WIND.speed, WindDirNames()); //example for layout
                
                                           
                ILI9341_draw_formatted_line(15, &y, GREEN, BLACK, "Location Data:");
                y -=12;                 
                ILI9341_draw_formatted_line(145, &y, TEAL, BLACK, "GSM/GNSS Data:\n"); 
                
                ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Lat.: %3d.%04d°", solar_params.latitude / 10000, solar_params.latitude % 10000);
                y -=12;                 
                
                if(A7672E_GSM_STATUS.bad_signal){ //if rssi is 32...99 
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GSM: No Signal!"); 
                }
                else{
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GSM signal: %3ddBm ", A7672E_GSM_STATUS.rssi);                     
                }          
                
                ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Long.: %3d.%04d°", solar_params.longitude / 10000, solar_params.longitude % 10000);
                y -=12;                 
                ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GSM Registration: %d", A7672E_GSM_STATUS.reg_status);
                
                ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Altitude: %4dm", solar_params.altitude);
                y -=12;                 
                ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GNSS locked: %s", (A7672EGNSS.mode == 2) ? "2D" : (A7672EGNSS.mode == 3) ? "3D" : "NO");
                
                
                y = 192;
                ILI9341_draw_formatted_line(85, &y, MISTYROSE, BLACK, "Towers Data:\n");
                ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Tower ID:");
                y -=12;
                ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "1");
                y -=12;
                ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "2");
                
                ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Tracker Frame:");
                y -=12;
                ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "179°A 57°E");
                y -=12;
                ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "177°A 56°E");
                
                ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Solar cells:");
                y -=12;
                ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "210V 9.4A");
                y -=12;
                ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "211V 9.5A");
                
                ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Azimuth Motor:");
                y -=12;
                ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "62V 1A");
                y -=12;
                ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "62V 1A");
                
                ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Elevation Motor:");
                y -=12;
                ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "25V 3A");
                y -=12;
                ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "24V 2A");
                
                y=300;
                ILI9341_draw_formatted_line(5, &y, MAGENTA, BLACK, "\nTouch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);   
    
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

        if(!Windows.background_updater){ //drawing not changing elements
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

