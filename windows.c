#include "settings.h"
#include "windowsVar.h"

int32_t digits_to_number(const uint8_t *buffer, uint8_t length) { //for time and location, towers
    int32_t value = 0;

    while (length--) {
        value = value * 10 + *buffer++;
    }
    return value;
}

void UserInterface(Windows_names_t window) {
    
    if(screen_sleep.sleep) // if screens sleeping ignore further code
        return;

    switch (window) {
            //ignore init window and do nthing till it will be changed to one window from below list
        case MAIN_WINDOW:
        {
            keyboard.type = none;
            if (!Windows.background_updater) {
                ILI9341_fill_ALL_color_DMA(BLACK); //fill screen 
                ili9341_draw_rect(0, 0, 240, 16, WHITE, 0); //date and time button

                ili9341_draw_rect(0, 20, 115, 80, CYAN, 0); //weather data
                ili9341_draw_rect(117, 20, 123, 80, YELLOW, 0); //sun data

                ili9341_draw_rect(0, 104, 115, 80, GREEN, 0); //location data
                ili9341_draw_rect(117, 104, 123, 80, TEAL, 0); //gsm/gnss data

                ili9341_draw_rect(0, 188, 115, 80, MISTYROSE, 0); //towers data

                Windows.background_updater = true;
            } else { //if background was drawed show other data         

                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) { // update data every second once                                     
                    
                    uint16_t y = 4;                    
                    ILI9341_draw_formatted_line(60, &y, WHITE, BLACK, "%4d-%02d-%02d %02d:%02d:%02d",
                            RTC_Date_and_Time.RTC_year + 2000,
                            RTC_Date_and_Time.RTC_month,
                            RTC_Date_and_Time.RTC_day,
                            RTC_Date_and_Time.RTC_hour,
                            RTC_Date_and_Time.RTC_minute,
                            RTC_Date_and_Time.RTC_second);
                    y = 4;
                    ILI9341_draw_formatted_line(190, &y, WHITE, BLACK, "%s",
                            (RTC_Date_and_Time.time_sync == NONE_sync) ? "No fix" :
                            (RTC_Date_and_Time.time_sync == GSM_sync) ? "GSM fix " :
                            (RTC_Date_and_Time.time_sync == GNSS_sync) ? "GNSS fix" :
                            "Manual  ");

                    y = 24;

                    ILI9341_draw_formatted_line(15, &y, CYAN, BLACK, "Weather Data");
                    y -= 12;
                    ILI9341_draw_formatted_line(145, &y, YELLOW, BLACK, "Sun Data\n");

                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Temperature:%3dC° ", sensors.BME680.temperature);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Azimuth: %3.02f ", solar_params.azimuth);

                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Pressure: %4dhPa ", sensors.BME680.pressure);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Elevation: %3.02f ", solar_params.elevated_refracted_elevation);

                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Humidity: %3d% ", sensors.BME680.humidity);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Sun l.l.: %dmV", sensors.SUN.level); //example for layout
                    
                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Wind: %dm/s %s\n", sensors.WIND.speed, WindDirNames()); //example for layout
                    
                    


                    ILI9341_draw_formatted_line(15, &y, GREEN, BLACK, "Location Data");
                    y -= 12;
                    ILI9341_draw_formatted_line(140, &y, TEAL, BLACK, "Network Data\n");

                    ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Lat.: %2d.%04d°", A7672E_NET.latitude / 10000, abs(A7672E_NET.latitude % 10000));
                    y -= 12;

                    if (A7672E_GSM_STATUS.bad_signal) { //if rssi is 32...99 
                        ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "No Signal!");
                    } else {
                        ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "Signal: %3ddBm ", A7672E_GSM_STATUS.rssi);
                    }

                    ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Long.: %3d.%04d°", A7672E_NET.longitude / 10000, abs(A7672E_NET.longitude % 10000));
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "Registration: %d", A7672E_GSM_STATUS.reg_status);

                    ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Altitude: %4dm", A7672E_NET.altitude);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GNSS locked: %s", (A7672EGNSS.mode == 2) ? "2D" : (A7672EGNSS.mode == 3) ? "3D" : "NO");
                    
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "Server: %s", (A7672E_work.ssrx == Bad) ? "NO RX " : (A7672E_work.ssrx == Good) ? "ALL OK" : "      ");
                    //A7672E_work.ssrx = Neutral;// uncomment if needed mommentary answer

                    y = 192;
                    ILI9341_draw_formatted_line(15, &y, MISTYROSE, BLACK, "Towers Data:\n");
                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Towers :%3d", A7672E_NET.towers_in_total);
                    
                    uint64_t total_power = 0;
                    for(uint16_t i = 0; i< A7672E_NET.towers_in_total; i++){ //calculate all towers power
                        total_power += towers[i].panel.power;
                    }
                    
                    uint32_t power_kw_x100 = (total_power + 5000) / 10000;
                    
                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "PV P: %4d.%02dkW", power_kw_x100 / 100, power_kw_x100 % 100);
                    total_power = 0;
                    for(uint16_t i = 0; i< A7672E_NET.towers_in_total; i++){ //calculate all towers power
                        total_power += towers[i].az_motor.power;
                    }
                    power_kw_x100 = (total_power + 500) / 1000;
                    
                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Az.Mot.P: %4dW", power_kw_x100); //all towers azimuth motors power
                    total_power = 0;
                    for(uint16_t i = 0; i< A7672E_NET.towers_in_total; i++){ //calculate all towers power
                        total_power += towers[i].el_motor.power;
                    }
                    power_kw_x100 = (total_power + 500) / 1000;
                    
                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "El.Mot.P: %4dW", power_kw_x100); ////all towers elevation motors power
                    
                    //y = 300;
                    //ILI9341_draw_formatted_line(5, &y, MAGENTA, BLACK, "TCC0: %09d", Periodic_Checker_Devices.period_counter);
                    
                    
                    //ILI9341_draw_formatted_line(5, &y, MAGENTA, BLACK, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);

                }
                Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;

                if (XPT2046_switch(64, 4000, 3800, 4000)) {//touch map for date and time change button
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = TIME_WINDOW;
                    Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open
                } else if (XPT2046_switch(64, 1800, 1900, 2700)) {
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = LOCATION_WINDOW;
                    Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open                
                } else if (XPT2046_switch(64, 1800, 700, 1800)) {
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = TOWER_WINDOW;
                    Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open                
                }
                else if (XPT2046_switch(1900, 4000, 1900, 2700)) {
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = NETWORK_WINDOW;
                    Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open                
                }
            }
        }
            break;
        case TIME_WINDOW:
        {
            static uint8_t DT[14] = {2, 0}; //set year 20...
            static uint8_t TZ[2] = {0, 2}; //set time zone to default value of +02 (range -12 +14)
            static char TZside = '+'; //time zone direction symbol
            static uint8_t pressCount = 2;
            static uint8_t countProtection = 0;
            uint16_t text_color = WHITE;
            static uint16_t status_txt_color = WHITE;
            static uint16_t gsm_txt_color = WHITE;
            static uint16_t gnss_txt_color = WHITE;
            

            if (!Windows.background_updater) { //drawing not changing elements
                keyboard.background_color = DARK_GRAY;
                Keyboard_SetType(digits);
                ILI9341_fill_ALL_color_DMA(keyboard.background_color); //background
                
                

                ili9341_draw_rect(0, 0, 60, 20, BLACK, 1); //and buttons frames
                uint16_t y = 6;
                ILI9341_draw_formatted_line(27, &y, WHITE, BLACK, "<");

                ili9341_draw_rect(60, 0, 60, 20, YELLOW, 1);
                y = 6;
                ILI9341_draw_formatted_line(81, &y, BLACK, YELLOW, "MAN");

                ili9341_draw_rect(120, 0, 60, 20, GREEN, 1);
                y = 6;
                ILI9341_draw_formatted_line(141, &y, RED, GREEN, "GSM");

                ili9341_draw_rect(180, 0, 60, 20, DARK_BLUE, 1);
                y = 6;
                ILI9341_draw_formatted_line(198, &y, CYAN, DARK_BLUE, "GNSS");

                Windows.background_updater = true;
            } else {
                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) {

                    uint16_t y = 30;
                    ILI9341_draw_formatted_line(0, &y, status_txt_color, keyboard.background_color, "RTC time: %4d-%02d-%02d %02d:%02d:%02d", RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second);
                    ILI9341_draw_formatted_line(0, &y, status_txt_color, keyboard.background_color, "T.Z.: %2d ", RTC_Date_and_Time.RTC_time_zone);
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "D.S.T: %s", is_daylight_saving_time(RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour) ? "YES" : "NO ");

                    const char *timesourceintext =
                            (RTC_Date_and_Time.time_sync == NONE_sync) ? "NONE        " :
                            (RTC_Date_and_Time.time_sync == GSM_sync) ? "GSM Module  " :
                            (RTC_Date_and_Time.time_sync == GNSS_sync) ? "GNSS Module " : "Manual input";
                    ILI9341_draw_formatted_line(0, &y, status_txt_color, keyboard.background_color, "Date/time source: %s", timesourceintext);

                    ILI9341_draw_formatted_line(0, &y, gsm_txt_color, keyboard.background_color, "GSM lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GSM_year + 2000, A7672EGSM.GSM_month, A7672EGSM.GSM_day, A7672EGSM.GSM_hour, A7672EGSM.GSM_minute, A7672EGSM.GSM_second);
                    if (A7672EGSM.GNSS_year + A7672EGSM.GNSS_month + A7672EGSM.GNSS_day + A7672EGSM.GNSS_hour + A7672EGSM.GNSS_minute + A7672EGSM.GNSS_second == 0)
                        ILI9341_draw_formatted_line(0, &y, gnss_txt_color, keyboard.background_color, "GNSS lock at: no GNSS lock");
                    else
                        ILI9341_draw_formatted_line(0, &y, gnss_txt_color, keyboard.background_color, "GNSS lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GNSS_year + 2000, A7672EGSM.GNSS_month, A7672EGSM.GNSS_day, A7672EGSM.GNSS_hour, A7672EGSM.GNSS_minute, A7672EGSM.GNSS_second);
                    //ILI9341_draw_formatted_line(0, &y, text_color, DARK_GRAY, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);
                    if(status_txt_color != text_color)
                        status_txt_color = text_color;
                    if(gsm_txt_color != text_color)
                        gsm_txt_color = text_color;
                    if(gnss_txt_color != text_color)
                        gnss_txt_color = text_color;
                    Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
                }

                draw_keyboard();
                uint16_t y = 173;

                if (keyboard.status == OPEN) { 
                    
                    uint16_t x = 60;

                    if (Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL){ //infinite press protection
                        countProtection = 0;
                    }
                    else { // if touch pressing hard enough       
                        
                        y = 161;                                               
                        ILI9341_draw_formatted_line(2, &y, YELLOW, keyboard.background_color, "MAN TIME: ");
                        
                        ili9341_draw_rect(0, y-3, 239, 15, YELLOW, 0);

                        
                        for (uint16_t clr = 0; clr < 17; clr++) {
                            y = 173;
                            uint16_t background = (pressCount == clr) ? RED : keyboard.background_color;

                            if ((clr < 14))
                                ILI9341_draw_formatted_line(x, &y, text_color, background, "%01d", DT[clr]); //draw digits of the date and time
                            else if (clr == 14)
                                ILI9341_draw_formatted_line(x, &y, text_color, background, "%c", TZside); //draw time zone direction
                            else if (clr > 14 && clr < 17)
                                ILI9341_draw_formatted_line(x, &y, text_color, background, "%01d", TZ[clr - 15]); //draw digits of the time zone
                            x += 6;
                            if (clr == 3 || clr == 5) {//Year-Month-Day
                                y = 173;
                                ILI9341_draw_formatted_line(x, &y, text_color, keyboard.background_color, "-");
                                x += 6;
                            } else if (clr == 7) {//Year-Month-Day //space
                                y = 173;
                                ILI9341_draw_formatted_line(x, &y, text_color, keyboard.background_color, " ");
                                x += 6;
                            } else if (clr == 9 || clr == 11) {//Year-Month-Day Hour:Minutes:Seconds
                                y = 173;
                                ILI9341_draw_formatted_line(x, &y, text_color, keyboard.background_color, ":");
                                x += 6;
                            } else if (clr == 13) {//Year-Month-Day Hour:Minutes:Seconds //space
                                y = 173;
                                ILI9341_draw_formatted_line(x, &y, text_color, keyboard.background_color, " ");
                                x += 6;
                            }
                        }
                                                                                              
                        for (uint8_t i = 0; i < keyboard.key_count; i++) { //find where is pressing
                            key_data *key = &keyboard.keys[i];
                            if ((Read_XPT2046.X >= key->X0) && (Read_XPT2046.X < key->X1) && (Read_XPT2046.Y >= key->Y0) && (Read_XPT2046.Y < key->Y1)) {
                                if (countProtection != key->ASCII_value) { //accept only once and one symbol per pressing
                                    Windows.once_per_second_update += 1;
                                    if ( (key->ASCII_value-48 >= 0) && (key->ASCII_value-48 <= 9) ) {
                                        if (pressCount < 14) { //changing Date and Time digits
                                            DT[pressCount] = key->ASCII_value-48;
                                            pressCount++;
                                        } else if (pressCount <= 16) {
                                            TZ[pressCount - 15] = key->ASCII_value-48; //change time zone digits  
                                            pressCount++;
                                        }
                                    } else if (pressCount == 14 && ((key->ASCII_value == '-') || (key->ASCII_value == '+'))) { //only if changing time zone and using + or - symbols
                                        pressCount++;
                                        TZside = key->ASCII_value;
                                    }

                                    if (pressCount == 17 && key->ASCII_value == '>') {
                                        uint8_t yy = (DT[2] * 10) + DT[3];
                                        uint8_t MM = (DT[4] * 10) + DT[5];
                                        uint8_t dd = (DT[6] * 10) + DT[7];
                                        uint8_t hh = (DT[8] * 10) + DT[9];
                                        uint8_t mm = (DT[10] * 10) + DT[11];
                                        uint8_t ss = (DT[12] * 10) + DT[13];

                                       
                                        int8_t tzcheck = (TZ[0] * 10) + TZ[1];
                                        if (TZside == '-')// if time zone is negative
                                            tzcheck = 0 - tzcheck; //update time zone

                                        if (is_time_correct(yy, MM, dd, hh, mm, ss) && ((tzcheck >= -12) && (tzcheck <= 14))) {
                                            y = 161;
                                            status_txt_color = GREEN;
                                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                                            RTC_Date_and_Time.RTC_time_zone = tzcheck; //update time zone
                                            keyboard.status = CLOSE;
                                            //apply_timezone(&yy, &MM, &dd, &hh, RTC_Date_and_Time.RTC_time_zone); //uncomet if set utc time
                                            RTC_date_and_time_sync(datetime_to_RTC_format(yy, MM, dd, hh, mm, ss), MAN_sync);
                                        } else {
                                            status_txt_color = RED;
                                        }
                                    } else if ((pressCount > 2) && (key->ASCII_value == '<')) {
                                        pressCount--;
                                    } else if (key->ASCII_value == 127) { //clear whole line
                                        for (uint8_t x = 2; x < 14; x++) DT[x] = 0;
                                        pressCount = 0;
                                    }
                                    countProtection = key->ASCII_value;
                                    break; // if button presset stoping for cycle and continue further
                                }
                            }
                        }
                    }
                }

                // Touch button check
                if (XPT2046_switch(100, 916, 3700, 4000)) { // <  
                    
                    Windows.background_updater = false;
                    Windows.Window = MAIN_WINDOW;
                    keyboard.status = CLOSE;
                    Windows.once_per_second_update = 0;
                    status_txt_color = text_color;
                    gsm_txt_color = text_color;
                    gnss_txt_color = text_color;
                    
                    
                } else if (XPT2046_switch(1016, 1932, 3700, 4000)) { // MAN
                    
                    Windows.once_per_second_update = 0;
                    
                    if(keyboard.status){ //if keyboard open before close
                        y = 161;
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                    }
                    keyboard.status ^= 1;                       
                    
                } else if (XPT2046_switch(2032, 2948, 3700, 4000)) { // GSM
                    
                    Windows.once_per_second_update = 0;
                    status_txt_color = GREEN;
                    gsm_txt_color = GREEN; 
                    RTC_Date_and_Time.time_sync = NONE_sync;          
                    A7672E_work.state = SET_DEVICE;
                    if (keyboard.status) {
                        keyboard.status = CLOSE;
                        Windows.once_per_second_update = 0;
                    }
                    
                } else if (XPT2046_switch(3048, 3964, 3700, 4000)) { // GNSS
                    Windows.once_per_second_update = 0;

                    if (A7672EGNSS.mode != 0) {
                        
                        status_txt_color = GREEN;
                        gnss_txt_color = GREEN;
                        
                        RTC_Date_and_Time.time_sync = GSM_sync;
                        A7672E_work.state = SET_DEVICE;
                    } else {
                        status_txt_color = RED;
                        gnss_txt_color = RED;
                    }
                    if (keyboard.status) {
                        keyboard.status = CLOSE;
                        Windows.once_per_second_update = 0;
                    }
                    
                }
            }
        }
            break;

        case LOCATION_WINDOW:
        {
            static uint8_t total[3] = {7, 8, 5};
            static uint8_t LAT[7] = {0}; //show latitude as +00.0000
            static uint8_t LNG[8] = {0}; //show longitude as +000.0000
            static uint8_t ALT[5] = {0}; //show altitude as +0000

            static char posneg[3] = {'+', '+', '+'}; //latitude, longitude altitude default symbol +
            static uint8_t changing_current_param = 0; //0- none, 1-latitude, 2-longitude, 3-altitude, 4- auto (if gnss locked)
            static uint8_t pressCount[3] = {0};
            static uint8_t countProtection[3] = {0};
            uint16_t text_color = WHITE;
            static uint16_t gnss_txt_color = WHITE; 
            static uint16_t lat_txt_color = WHITE;
            static uint16_t lng_txt_color = WHITE;
            static uint16_t alt_txt_color = WHITE;
            

            if (!Windows.background_updater) { //drawing not changing elements            
                keyboard.background_color = DARK_GRAY;
                Keyboard_SetType(digits);

                ILI9341_fill_ALL_color_DMA(keyboard.background_color); //background

                ili9341_draw_rect(0, 0, 48, 20, BLACK, 1); //and buttons frames
                uint16_t y = 6;
                ILI9341_draw_formatted_line(18, &y, WHITE, BLACK, "<");

                ili9341_draw_rect(48, 0, 48, 20, YELLOW, 1);
                y = 6;
                ILI9341_draw_formatted_line(60, &y, BLACK, YELLOW, "LATI");

                ili9341_draw_rect(96, 0, 48, 20, GREEN, 1);
                y = 6;
                ILI9341_draw_formatted_line(108, &y, RED, GREEN, "LONG");

                ili9341_draw_rect(144, 0, 48, 20, DARK_BLUE, 1);
                y = 6;
                ILI9341_draw_formatted_line(156, &y, CYAN, DARK_BLUE, "ALTI");

                ili9341_draw_rect(192, 0, 48, 20, RED, 1);
                y = 6;
                ILI9341_draw_formatted_line(204, &y, WHITE, RED, "AUTO");
                
                

                Windows.background_updater = true;
            } else {
                
                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) {
                uint16_t y = 30;
                
                ILI9341_draw_formatted_line(0, &y, lat_txt_color, keyboard.background_color, "Latitude: %2d.%04d° ", A7672E_NET.latitude / 10000, abs(A7672E_NET.latitude % 10000));
                ILI9341_draw_formatted_line(0, &y, lng_txt_color, keyboard.background_color, "Longitude: %3d.%04d° ", A7672E_NET.longitude / 10000, abs(A7672E_NET.longitude % 10000));
                ILI9341_draw_formatted_line(0, &y, alt_txt_color, keyboard.background_color, "Altitude: %4dm ", A7672E_NET.altitude);
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Time: %4d-%02d-%02d %02d:%02d:%02d", RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second);
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Signal: %3ddBm ", A7672E_GSM_STATUS.rssi);
                    
                    const char *regstatus =
                            (A7672E_GSM_STATUS.reg_status == 0) ? "No          " :
                            (A7672E_GSM_STATUS.reg_status == 1) ? "Yes         " :
                            (A7672E_GSM_STATUS.reg_status == 2) ? "Searching   " :
                            (A7672E_GSM_STATUS.reg_status == 3) ? "Denied      " :
                            (A7672E_GSM_STATUS.reg_status == 4) ? "Unknown     " :
                            (A7672E_GSM_STATUS.reg_status == 5) ? "Yes roaming " :
                            (A7672E_GSM_STATUS.reg_status == 6) ? "SMS only    " :
                            (A7672E_GSM_STATUS.reg_status == 7) ? "SMS, roaming" :
                            (A7672E_GSM_STATUS.reg_status == 11) ? "Em. service " : "Unknown      ";
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Registration in Network: %s", regstatus);
                    ILI9341_draw_formatted_line(0, &y, gnss_txt_color, keyboard.background_color, "GNSS locked: %s", (A7672EGNSS.mode == 2) ? "2D" : (A7672EGNSS.mode == 3) ? "3D" : "NO");
                    if(gnss_txt_color != text_color) //after AUTO button press keep changed text color one second
                        gnss_txt_color = text_color;
                    
                    if(lat_txt_color != text_color) //after AUTO button press keep changed text color one second
                        lat_txt_color = text_color;
                    
                    if(lng_txt_color != text_color) //after AUTO button press keep changed text color one second
                        lng_txt_color = text_color;
                    
                    if(alt_txt_color != text_color) //after AUTO button press keep changed text color one second
                        alt_txt_color = text_color;
                    
                    Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
                }
                
                draw_keyboard();
                uint16_t y = 173;
                
                if (keyboard.status == OPEN) {
                    
                    uint16_t x = 2;
                    uint8_t location_param = changing_current_param - 1;
                    uint8_t *buffers[] = { LAT, LNG, ALT };
                    uint8_t *target_buffer = buffers[location_param];
                    uint8_t *press = &pressCount[location_param];
                  
                    if (Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL) { //infinite press protection
                        countProtection[location_param] = 0;
                    } else { // if touch pressing hard enough
                        
                        ili9341_draw_rect(0, y-3, 239, 15, YELLOW, 0);
                        
                        y=161;
                        ILI9341_draw_formatted_line(2, &y, YELLOW, keyboard.background_color, location_param_names[location_param].name);
                        
                        for (uint16_t clr = 0; clr < total[location_param] + 1; clr++) {
                            y = 173;
                            uint16_t background = (*press == clr) ? RED : keyboard.background_color;

                            if (clr == 0) { //draw latitude symbol - or + (default)
                                ILI9341_draw_formatted_line(x, &y, text_color, background, "%c", posneg[location_param]);
                            } else if (clr < total[location_param]) {
                                y = 173;
                                uint8_t value = 0;
                                value = target_buffer[clr - 1];
                                ILI9341_draw_formatted_line(x, &y, text_color, background, "%01d", value); //draw current data                          
                            }
                            x += 6;
                            const int8_t dot_pos[] = {2, 3, -1};
                            if (clr == dot_pos[location_param]) { // draw , for laitude after 2 digits and for longitude after 3 digits and skip for altitude
                                y = 173;
                                ILI9341_draw_formatted_line(x, &y, text_color, keyboard.background_color, ",");
                                x += 6;
                            }

                        }
                        
                        y=161;
                        uint8_t digits = total[location_param];                        
                        for (uint8_t i = 0; i < keyboard.key_count; i++) { //find where is pressing
                            key_data *key = &keyboard.keys[i];
                            if ((Read_XPT2046.X >= key->X0) && (Read_XPT2046.X < key->X1) && (Read_XPT2046.Y >= key->Y0) && (Read_XPT2046.Y < key->Y1)) {
                                if (countProtection[location_param] != key->ASCII_value) { //accept only once and one symbol per pressing
                                    Windows.once_per_second_update += 1;
                                    if ( (key->ASCII_value-48 >= 0) && (key->ASCII_value-48 <= 9) ) {
                                        if ((*press < digits) && *press > 0) { //changing digits only after + or - symbol                                                                                    
                                            target_buffer[(*press) - 1] = key->ASCII_value-48;
                                            (*press)++;
                                        }
                                    } else if (*press == 0 && ((key->ASCII_value == '-') || (key->ASCII_value == '+'))) { //only if changing  + or - symbols
                                        (*press)++;
                                        posneg[location_param] = key->ASCII_value;
                                    }

                                    if (*press == digits && key->ASCII_value == '>') {// pressing done button
                                        int32_t final_result = digits_to_number(target_buffer, digits - 1); //extract digit from buffer
                                        if (final_result > limits[location_param].max_value) { //check limits if they are too big
                                            switch(location_param){
                                                case 0: 
                                                    lat_txt_color = RED;
                                                break;
                                                case 1:
                                                    lng_txt_color = RED;
                                                break;
                                                case 2:
                                                    alt_txt_color = RED;
                                                break;
                                            }
                                        }
                                        else { //if long, lat and alt is correct
                                            if (posneg[location_param] == '-') { //if it was negative
                                                final_result = -final_result;
                                            }                              
                                            if(final_result != *limits[location_param].target){
                                            *limits[location_param].target = final_result;
                                            
                                                switch(location_param){ // i am too lazy make this all in struct...
                                                    case 0:                                                       
                                                        EEPROM_Write( offsetof(A7672E_network_settings_t, latitude), &A7672E_NET.latitude, sizeof(A7672E_NET.latitude)  );                                                         
                                                        lat_txt_color = GREEN;
                                                    break;
                                                    case 1:                                                    
                                                        EEPROM_Write( offsetof(A7672E_network_settings_t, longitude), &A7672E_NET.longitude, sizeof(A7672E_NET.longitude)  );                                                        
                                                        lng_txt_color = GREEN;
                                                    break;
                                                    case 2:
                                                        EEPROM_Write( offsetof(A7672E_network_settings_t, altitude), &A7672E_NET.altitude, sizeof(A7672E_NET.altitude)  ); 
                                                        alt_txt_color = GREEN;
                                                    break;
                                                }
                                            }
                                            

                                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                                            keyboard.status = CLOSE;
                                        }
                                    } else if ((*press > 0) && (key->ASCII_value == '<')) {
                                        (*press)--;
                                        
                                    } else if ((*press > 0) && (key->ASCII_value == 127)) { //clear whole line 
                                        memset(target_buffer, 0, digits);
                                        posneg[location_param] = '+'; //set back default symbol
                                        *press = 0;
                                    }
                                    countProtection[location_param] = key->ASCII_value;
                                    break; // if button presset stoping for cycle and continue further
                                }
                            }
                        }
                    }
                }
                // Touch button check
                if (XPT2046_switch(64, 812, 3700, 4000)) { // < 
                    
                    changing_current_param = 0;
                    Windows.background_updater = false;
                    Windows.Window = MAIN_WINDOW;
                    keyboard.status = CLOSE;
                    Windows.once_per_second_update = 0;
                    
                    gnss_txt_color = text_color; 
                    lat_txt_color = text_color;
                    lng_txt_color = text_color;
                    alt_txt_color = text_color;
                    
                } else if (XPT2046_switch(844, 1624, 3700, 4000)) { // LATI
                    
                    if(changing_current_param != 1 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before close
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 1;
                    
                } else if (XPT2046_switch(1656, 2436, 3700, 4000)) { // LONG
                    
                    if(changing_current_param != 2 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before close
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 2;
                    
                } else if (XPT2046_switch(2468, 3248, 3700, 4000)) { // ALTI
                    
                    if(changing_current_param != 3 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before close
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 3;
                    
                } else if (XPT2046_switch(3280, 4000, 3700, 4000)) { // AUTO

                    if(changing_current_param != 4 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                        keyboard.status = CLOSE;
                    }  
                    
                    if (A7672EGNSS.mode != 0) { //if mode is 2D or 3D update values not when it is 0(no lock)                  
                        A7672E_NET.latitude = (int32_t) A7672EGNSS.lat / 1000;
                        A7672E_NET.longitude = (int32_t) A7672EGNSS.log / 1000;
                        A7672E_NET.altitude = (int16_t) A7672EGNSS.alt;
                        gnss_txt_color = GREEN;
                        lat_txt_color = GREEN;
                        lng_txt_color = GREEN;
                        alt_txt_color = GREEN;
                    } else { //gnss NOT locked!!!
                        gnss_txt_color = RED;
                        lat_txt_color = RED;
                        lng_txt_color = RED;
                        alt_txt_color = RED;                        
                    }                                       

                    changing_current_param = 4;
                }

            }

        }   break;
        
        case TOWER_WINDOW:
        {
            static uint8_t changing_current_param = 0;
            static uint8_t total[2] = {3, 3};
            static uint8_t View_tower[3] = {0};
            static uint8_t Set_towers[3] = {0};
            
            static uint8_t pressCount[2] = { 0 };
            static uint8_t countProtection[2] = { 1 };
            
            static uint16_t current_id_in_view = 0;            
            
            uint16_t text_color = WHITE;
            
            static uint16_t id_txt_color = WHITE;
            
            if (!Windows.background_updater) { //drawing not changing elements            
                keyboard.background_color = DARK_GRAY;
                Keyboard_SetType(digits);

                ILI9341_fill_ALL_color_DMA(keyboard.background_color); //background

                ili9341_draw_rect(0, 0, 48, 20, BLACK, 1); //and buttons frames
                uint16_t y = 6;
                ILI9341_draw_formatted_line(18, &y, WHITE, BLACK, "<");

                ili9341_draw_rect(48, 0, 48, 20, YELLOW, 1);
                y = 6;
                ILI9341_draw_formatted_line(60, &y, BLACK, YELLOW, "View");

                ili9341_draw_rect(96, 0, 48, 20, GREEN, 1);
                y = 6;
                ILI9341_draw_formatted_line(108, &y, RED, GREEN, "SET");


                Windows.background_updater = true;
            } else {
                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) { //update every second except...
                    uint16_t y = 30;
                    
                    tower_t *current_tower = &towers[current_id_in_view];
                    
                    ILI9341_draw_formatted_line(0, &y, id_txt_color, keyboard.background_color, "Tower: %03d of %03d ID: %03d ",current_id_in_view+1, A7672E_NET.towers_in_total, current_tower -> id);
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Last update: %04d-%02d-%02d %02d:%02d:%02d", 
                            current_tower -> update_time.year, 
                            current_tower -> update_time.month, 
                            current_tower -> update_time.day, 
                            current_tower -> update_time.hour, 
                            current_tower -> update_time.minute, 
                            current_tower -> update_time.second);
                    
                    ILI9341_draw_formatted_line(0, &y, CYAN, keyboard.background_color, "Tracker Frame:");
                    y -= 12;
                    ILI9341_draw_formatted_line(120, &y, CYAN, keyboard.background_color, "Solar cells:");
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Azimuth: %3d.%02d°", current_tower -> position.azimuth / 100, current_tower -> position.azimuth % 100);
                    y -= 12;
                    ILI9341_draw_formatted_line(120, &y, text_color, keyboard.background_color, "Voltage: %3d.%01dV", current_tower -> panel.voltage / 10, current_tower -> panel.voltage % 10);
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Elevation: %2d.%02d°", current_tower -> position.elevation / 100, current_tower -> position.elevation % 100);                    
                    y -= 12;
                    ILI9341_draw_formatted_line(120, &y, text_color, keyboard.background_color, "Current: %2d.%02dA", current_tower -> panel.current / 100, current_tower -> panel.current % 100);
                                        
                    char *ess = (current_tower ->es == 0) ? "Normal " :
                                (current_tower ->es == 1) ? "El.min " :
                                (current_tower ->es == 2) ? "El.Max " :
                                (current_tower ->es == 4) ? "Az.min " :
                                (current_tower ->es == 8) ? "Az.Max " :
                                                            "unknown";
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "End switch: %s", ess);
                    y -= 12;
                    uint32_t power_kw_x100 = (current_tower -> panel.power + 5000) / 10000;
                    ILI9341_draw_formatted_line(120, &y, text_color, keyboard.background_color, "Power: %4d.%02dkW", power_kw_x100 / 100, power_kw_x100 % 100);
                    
                    
                    ILI9341_draw_formatted_line(0, &y, CYAN, keyboard.background_color, "Azimuth Motor:");
                    y -= 12;
                    ILI9341_draw_formatted_line(120, &y, CYAN, keyboard.background_color, "Elevation Motor:");
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Voltage: %2d.%01dV", current_tower -> az_motor.voltage / 10, current_tower -> az_motor.voltage % 10);
                    y -= 12;
                    ILI9341_draw_formatted_line(120, &y, text_color, keyboard.background_color, "Voltage: %2d.%01dV", current_tower -> el_motor.voltage / 10,current_tower -> el_motor.voltage % 10);
                    
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Current: %2d.%02dA", current_tower -> az_motor.current / 100, current_tower -> az_motor.current % 100);
                    y -= 12;
                    ILI9341_draw_formatted_line(120, &y, text_color, keyboard.background_color, "Current: %2d.%02dA", current_tower -> el_motor.current / 100, current_tower -> el_motor.current % 100);
                    
                    uint32_t power_w_x10 = (current_tower -> az_motor.power + 5) / 10;
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Power: %3d.%02dW", power_w_x10/100, power_w_x10%100);
                    y -= 12;
                    power_w_x10 = (current_tower -> el_motor.power + 5) / 10;
                    ILI9341_draw_formatted_line(120, &y, text_color, keyboard.background_color, "Power: %3d.%02dW", power_w_x10/100, power_w_x10%100);
                    
                    if(id_txt_color != text_color) //after AUTO button press keep changed text color one second
                        id_txt_color = text_color;
                    
                    Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
                }

                
                draw_keyboard();
                uint16_t y = 173;
                
                if (keyboard.status == OPEN) {
                    
                    uint16_t x = 2;
                    uint8_t location_param = changing_current_param - 1;
                    uint8_t *buffers[] = {View_tower, Set_towers};
                    uint8_t *target_buffer = buffers[location_param];
                    uint8_t *press = &pressCount[location_param];
                  
                    if (Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL) { //infinite press protection
                        countProtection[location_param] = 0;
                    } else { // if touch pressing hard enough
                        
                        ili9341_draw_rect(0, y-3, 239, 15, YELLOW, 0);
                        
                        y=161;
                        ILI9341_draw_formatted_line(2, &y, YELLOW, keyboard.background_color, "%s", location_param == 0 ? "View tower by ID" : "Set total towers number");
                        
                        for (uint16_t clr = 0; clr < total[location_param]; clr++) {
                            y = 173;
                            uint16_t background = (*press == clr) ? RED : keyboard.background_color;                            
                            uint8_t value = target_buffer[clr];
                            
                            ILI9341_draw_formatted_line(x, &y, text_color, background, "%01d", value); //draw current data                          
                            x += 6;  
                        }
                        
                        y=161;
                        uint8_t digits = total[location_param];                        
                        for (uint8_t i = 0; i < keyboard.key_count; i++) { //find where is pressing
                            key_data *key = &keyboard.keys[i];
                            if ((Read_XPT2046.X >= key->X0) && (Read_XPT2046.X < key->X1) && (Read_XPT2046.Y >= key->Y0) && (Read_XPT2046.Y < key->Y1)) {
                                if (countProtection[location_param] != key->ASCII_value) { //accept only once and one symbol per pressing
                                    Windows.once_per_second_update += 1;
                                    if ( (key->ASCII_value-48 >= 0) && (key->ASCII_value-48 <= 9) ) {
                                        if (*press < digits) { //changing digits only after + or - symbol                                                                                    
                                            target_buffer[(*press)] = key->ASCII_value-48; // swap bites in places 001 is 1 not 100 (for id and total number digit begins from left to right)
                                            (*press)++;
                                        }
                                    }

                                    if (*press == digits && key->ASCII_value == '>') { // pressing done button

                                        uint16_t final_result = digits_to_number(target_buffer, digits);
                                       
                                        if (final_result > (255 + location_param) ) {// tower count can't be 255 for view and 256 for set
                                            id_txt_color = RED;
                                        }
                                        else if (location_param == 0) { // view id from 0 to 254
                                            if (final_result > A7672E_NET.towers_in_total-1) {
                                                id_txt_color = RED;
                                            }
                                            else {
                                                current_id_in_view = final_result;
                                                id_txt_color = GREEN;
                                                ILI9341_fill_PART_color_DMA( keyboard.background_color, 0, 239, y, y + 23 );
                                                keyboard.status = CLOSE;
                                            }
                                        }
                                        else if (location_param == 1) { // set total towers number from 1 to 254
                                            if(final_result == 0){
                                                id_txt_color = RED;
                                            }
                                            else if (A7672E_NET.towers_in_total != final_result) {
                                                if(current_id_in_view > final_result - 1)// if current is above max towers int total count set to view last tower data
                                                    current_id_in_view = final_result - 1;
                                                A7672E_NET.towers_in_total = final_result;
                                                EEPROM_Write( offsetof(A7672E_network_settings_t, towers_in_total), &A7672E_NET.towers_in_total, sizeof(A7672E_NET.towers_in_total) );// find place in structure, set variable and size
                                                Towers_init(); //init new towers (ne restart requaired)
                                                id_txt_color = GREEN;
                                                ILI9341_fill_PART_color_DMA( keyboard.background_color, 0, 239, y, y + 23 );
                                                keyboard.status = CLOSE;                                                
                                            }
                                        }
                                    } else if ((*press > 0) && (key->ASCII_value == '<')) {
                                        (*press)--;
                                    } else if ((*press > 0) && (key->ASCII_value == 127)) { //clear whole line and close keyboard  
                                        memset(target_buffer, 0, digits);
                                        *press = 0;
                                    }
                                    countProtection[location_param] = key->ASCII_value;
                                    break; // if button presset stoping for cycle and continue further
                                }
                            }
                        }
                    }
                }


                // Touch button check
                if (XPT2046_switch(64, 812, 3700, 4000)) { // < 
                    Windows.background_updater = false;
                    Windows.Window = MAIN_WINDOW;
                    keyboard.status = CLOSE;
                    Windows.once_per_second_update = 0;
                    changing_current_param = 0;
                    
                    uint16_t id_txt_color = text_color;
                    
                } else if (XPT2046_switch(844, 1624, 3700, 4000)) { // View Tower data by id
                    
                    if(changing_current_param != 1 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before close
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 1;
                    
                } else if (XPT2046_switch(1656, 2436, 3700, 4000)) { // Set total towers in system
                    
                    if(changing_current_param != 2 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before close
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y+23);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 2;
                }

            }

        }   break;
            
        case NETWORK_WINDOW:{
            uint16_t text_color = WHITE;
            static uint16_t apn_txt_color = WHITE;
            static uint16_t tp_txt_color = WHITE;
            static uint16_t su_txt_color = WHITE;

            static uint8_t changing_current_param = 0;   
                        
            static uint8_t pressCount[3] = { 0 };
            static uint8_t countProtection[3] = { 55 };
            
            if (!Windows.background_updater) { //drawing not changing elements            
                keyboard.background_color = DARK_GRAY;

                ILI9341_fill_ALL_color_DMA(keyboard.background_color); //background

                ili9341_draw_rect(0, 0, 60, 20, BLACK, 1); //and buttons frames
                uint16_t y = 6;
                ILI9341_draw_formatted_line(27, &y, WHITE, BLACK, "<");

                ili9341_draw_rect(60, 0, 60, 20, YELLOW, 1);
                y = 6;
                ILI9341_draw_formatted_line(66, &y, BLACK, YELLOW, "APN name");

                ili9341_draw_rect(120, 0, 60, 20, GREEN, 1);
                y = 6;
                ILI9341_draw_formatted_line(126, &y, RED, GREEN, "T.Phone");

                ili9341_draw_rect(180, 0, 60, 20, DARK_BLUE, 1);
                y = 6;
                ILI9341_draw_formatted_line(183, &y, CYAN, DARK_BLUE, "Ser. URL");                              

                Windows.background_updater = true;
            } else {
                
                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) {
                uint16_t y = 30;
                
                    ILI9341_draw_formatted_line(0, &y, apn_txt_color, keyboard.background_color, "APN: %s", A7672E_NET.APN_USR);
                    ILI9341_draw_formatted_line(0, &y, tp_txt_color, keyboard.background_color, "Trusted phone: %s", A7672E_NET.TRST_PHN);
                    ILI9341_draw_formatted_line(0, &y, su_txt_color, keyboard.background_color, "Server url: %s\n", A7672E_NET.SERVER_URL); 

                    if(apn_txt_color != text_color) //after AUTO button press keep changed text color one second
                        apn_txt_color = text_color;
                    
                    if(tp_txt_color != text_color) //after AUTO button press keep changed text color one second
                        tp_txt_color = text_color;
                    
                    if(su_txt_color != text_color) //after AUTO button press keep changed text color one second
                        su_txt_color = text_color;
                    
                    Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
                }
                
                draw_keyboard();
                
                uint16_t y = 135; //text line where all starts symbols count and below it text fill window                
                uint16_t new_y = y; //used for touch buttons  to clear previous symbols counter and text window               
                
                if (keyboard.status == OPEN) {  
                    
                    uint8_t location_param = changing_current_param - 1;                    
                    GSM_NET_param_t *param = &win_params[location_param];
                    uint8_t max_size = param->size;
                    char *source_buffer = param->edit_buffer;
                    char *target_buffer = param->target_buffer;
                    
                    uint16_t text_window_y = ((max_size + 39) / 40); //px place for graphics. 40 symbols in one line max (actually 39)                    
                    new_y = text_window_y; //line number different count
                    text_window_y = y + (48 - (text_window_y * 12)) - 12; //max 4 y rows = 48
                    y = text_window_y;

                    
                    uint8_t *press = &pressCount[location_param];                    
                    
                    if (Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL) { //infinite press protection
                        countProtection[location_param] = 0;
                    } 
                    else { // if touch pressing hard enough                                                  
                        bool accept = false;
                        //uint8_t key_count = (location_param == 1) ? KEY_COUNT_DIGITS_KEYBOARD : KEY_COUNT_LETTERS_KEYBOARD;
                        for (uint8_t i = 0; i < /*key_count*/keyboard.key_count; i++) { //find where is pressing
                            key_data *key = &keyboard.keys[i];//selecting touch map. for trusted phone use digits and apn name or url use letters
                            if ((Read_XPT2046.X >= key->X0) && (Read_XPT2046.X < key->X1) && (Read_XPT2046.Y >= key->Y0) && (Read_XPT2046.Y < key->Y1)) {
                                if (countProtection[location_param] != key->ASCII_value){                                    
                                    
                                    if(key->ASCII_value == 14){//shift is pressed?
                                        if(keyboard.shift == true){//if yes
                                            keyboard.shift = false;
                                            ili9341_draw_rect(1,269,22,26,WHITE,0); //unmark shift button
                                        }                                            
                                        else{
                                            keyboard.shift = true;
                                            ili9341_draw_rect(1,269,22,26,RED,0); // mark shift button if it is pressed
                                        }                                        
                                    }
                                    else if(key->ASCII_value == '>'){ //if pressed > as enter   

                                            keyboard.status = CLOSE;
                                            keyboard.shift = false;
                                                                           
                                            if (memcmp(target_buffer, source_buffer, max_size) != 0){ //if new value is not the same
                                                memcpy(target_buffer, source_buffer, max_size); //copy it to permament buffer
                                                EEPROM_Write( param->eeprom_offset, target_buffer, max_size );//and update value in smarteeprom location                                                                                                                                             
                                                
                                                switch(location_param){
                                                    case 0: 
                                                        apn_txt_color = GREEN;
                                                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 30, 239, 30, 40); //clear apn name line background
                                                    break;
                                                    case 1:
                                                        tp_txt_color = GREEN;
                                                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 90, 239, 42, 54); //clear tp line background
                                                    break;
                                                    case 2:
                                                        su_txt_color = GREEN;
                                                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 72, 239, 54, 66); //clear su first line
                                                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, 66, 104); //and other 3 lines
                                                    break;
                                                }
                                                
                                                ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15); // clear text box
                                                
                                                accept = true;
                                                
                                                
                                            }
                                    }
                                    else if(key->ASCII_value == '<'){ //if pressed < as delete last one
                                        if(*press > 0) {//delete last one symbol if its more tan 0
                                            *press -= 1;// go back by one symbol   
                                            source_buffer[*press] = 0; //write NULL value to that symbol                                                                                                                     
                                        }
                                    }
                                    else if(key->ASCII_value == 127){ //if pressed X as delete all
                                        if(*press > 0) {//delete all if its more than 0
                                            memset(source_buffer, 0, max_size);
                                            *press = 0;//set zero to press counter
                                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, text_window_y - 3, text_window_y + (12* new_y) + 15); //clear all text window part including counter above
                                        }
                                    }
                                    else if(*press < max_size-1){ //fill only it fits into array
                                            char letter = key->ASCII_value;
                                            if( (keyboard.shift) && (key->ASCII_value >= 'a') && (key->ASCII_value <= 'z') ){ //BIG or small letters                                            
                                                letter -= 32; //from small to big
                                            }
                                            source_buffer[*press] = letter;                                        
                                            (*press)++;                                            
                                        }        
                                }       
                                countProtection[location_param] = key->ASCII_value;
                                break;
                                }
                            }
                            if(!accept){ //dont draw text box while pressed >
                                uint16_t x = 2; //start position on x coordination
                                uint16_t name_y = text_window_y;
                                ILI9341_draw_formatted_line(x, &name_y, YELLOW, keyboard.background_color, "%s", param->name);
                                ILI9341_draw_formatted_line(198, &text_window_y, YELLOW, keyboard.background_color, "%3d/%3d", *press, max_size-1);
                                ili9341_draw_rect(0, text_window_y - 3, 239, (12 * new_y) + 3, YELLOW, 0);
                                ILI9341_draw_formatted_line(x, &text_window_y, text_color, keyboard.background_color, "%s ", source_buffer);                                  
                            }
                      
                        }

                }
                // Touch button check
                if (XPT2046_switch(64, 812, 3700, 4000)) { // < 
                    
                    changing_current_param = 0;
                    Windows.background_updater = false;
                    Windows.Window = MAIN_WINDOW;
                    keyboard.status = CLOSE;
                    keyboard.shift = false;
                    Windows.once_per_second_update = 0;
                    
                    uint16_t apn_txt_color = text_color;
                    uint16_t tp_txt_color = text_color;
                    uint16_t su_txt_color = text_color;
                    
                } else if (XPT2046_switch(1016, 1932, 3700, 4000)) { // APN name
                    Keyboard_SetType(letters);
                    if(changing_current_param != 1 && keyboard.status){                        
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15);
                        reopen_keyboard();
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before colose
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 1;
                } else if (XPT2046_switch(2032, 2948, 3700, 4000)) { // trusted phone
                    Keyboard_SetType(digits);
                    if(changing_current_param != 2 && keyboard.status){
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15);
                        reopen_keyboard();
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before colose
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 2;
                } else if (XPT2046_switch(3048, 3964, 3700, 4000)) { // Server url
                    Keyboard_SetType(letters);
                    if(changing_current_param != 3 && keyboard.status){
                        ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15);
                        reopen_keyboard();
                    }
                    else{
                        if(keyboard.status){ //if keyboard open before colose
                            ILI9341_fill_PART_color_DMA(keyboard.background_color, 0, 239, y, y + (12* new_y) + 15);
                        }
                        keyboard.status ^= 1;                       
                    }
                    changing_current_param = 3;
                }

            }

        }
            break;
    }
}

