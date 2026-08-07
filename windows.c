#include "settings.h"
#include "windowsVar.h"

int32_t digits_to_number(const uint8_t *buffer, uint8_t length) { //for time and location
    int32_t value = 0;

    while (length--) {
        value = value * 10 + *buffer++;
    }
    return value;
}

void UserInterface(Windows_names_t window) {

    switch (window) {
            //ignore init window and do nthing till it will be changed to one window from below list
        case MAIN_WINDOW:
        {
            if (!Windows.background_updater) {
                ILI9341_fill_color_DMA(BLACK); //fill screen 
                ili9341_draw_rect(0, 0, 240, 16, WHITE, 0); //date and time button

                ili9341_draw_rect(0, 20, 115, 80, CYAN, 0); //weather data
                ili9341_draw_rect(117, 20, 123, 80, YELLOW, 0); //sun data

                ili9341_draw_rect(0, 104, 115, 80, GREEN, 0); //location data
                ili9341_draw_rect(117, 104, 123, 80, TEAL, 0); //gsm/gnss data

                ili9341_draw_rect(0, 188, 240, 100, MISTYROSE, 0); //towers data

                Windows.background_updater = true;
            } else { //if background was drawed show other data         

                ADC0_read(WIND_SPEED); //read wind speed to catch gust of wind

                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) { // update data every second once
                    RTC_read_date_and_time();
                    BME680_read_t_p_rh();
                    calculate_solar_position();
                    apply_all_elevation_modifies();    
                    
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

                    ILI9341_draw_formatted_line(15, &y, CYAN, BLACK, "Weather Data:");
                    y -= 12;
                    ILI9341_draw_formatted_line(145, &y, YELLOW, BLACK, "Sun Data:\n");

                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Temperature:%3dC° ", (BME680.temperature + 50) / 100);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Azimuth: %3.02f ", solar_params.azimuth);

                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Pressure: %4dhPa ", (BME680.pressure + 50) / 100);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, YELLOW, BLACK, "Elevation: %3.02f ", solar_params.elevated_refracted_elevation);

                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Humidity: %3d% ", (BME680.humidity + 500) / 1000);

                    ADC0_read(WIND_DIR); //read wind direction every second;
                    ILI9341_draw_formatted_line(5, &y, CYAN, BLACK, "Wind: %dm/s %s\n", WIND.speed, WindDirNames()); //example for layout


                    ILI9341_draw_formatted_line(15, &y, GREEN, BLACK, "Location Data:");
                    y -= 12;
                    ILI9341_draw_formatted_line(145, &y, TEAL, BLACK, "GSM/GNSS Data:\n");

                    ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Lat.: %2d.%04d°", solar_params.latitude / 10000, abs(solar_params.latitude % 10000));
                    y -= 12;

                    if (A7672E_GSM_STATUS.bad_signal) { //if rssi is 32...99 
                        ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GSM: No Signal!");
                    } else {
                        ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GSM signal: %3ddBm ", A7672E_GSM_STATUS.rssi);
                    }

                    ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Long.: %3d.%04d°", solar_params.longitude / 10000, abs(solar_params.longitude % 10000));
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GSM Registration: %d", A7672E_GSM_STATUS.reg_status);

                    ILI9341_draw_formatted_line(5, &y, GREEN, BLACK, "Altitude: %4dm", solar_params.altitude);
                    y -= 12;
                    ILI9341_draw_formatted_line(125, &y, TEAL, BLACK, "GNSS locked: %s", (A7672EGNSS.mode == 2) ? "2D" : (A7672EGNSS.mode == 3) ? "3D" : "NO");


                    y = 192;
                    ILI9341_draw_formatted_line(85, &y, MISTYROSE, BLACK, "Towers Data:\n");
                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Tower ID:");
                    y -= 12;
                    ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "1");
                    y -= 12;
                    ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "2");

                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Tracker Frame:");
                    y -= 12;
                    ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "179°A 57°E");
                    y -= 12;
                    ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "177°A 56°E");

                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Solar cells:");
                    y -= 12;
                    ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "210V 9.4A");
                    y -= 12;
                    ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "211V 9.5A");

                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Azimuth Motor:");
                    y -= 12;
                    ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "62V 1A");
                    y -= 12;
                    ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "62V 1A");

                    ILI9341_draw_formatted_line(5, &y, MISTYROSE, BLACK, "Elevation Motor:");
                    y -= 12;
                    ILI9341_draw_formatted_line(105, &y, MISTYROSE, BLACK, "25V 3A");
                    y -= 12;
                    ILI9341_draw_formatted_line(175, &y, MISTYROSE, BLACK, "24V 2A");

                    y = 300;
                    ILI9341_draw_formatted_line(5, &y, MAGENTA, BLACK, "\nTouch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);

                }
                Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;

                if (XPT2046_switch(100, 3850, 3800, 4000)) {//touch map for date and time change button
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = TIME_WINDOW;
                    Windows.once_per_second_update = 0; //reset update to show data instantly after new windows is open
                } else if (XPT2046_switch(100, 1900, 1890, 2700)) {
                    Windows.background_updater = false; //prepare to update screen
                    Windows.Window = LOCATION_WINDOW;
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
            static uint8_t countProtection = 55;

            if (!Windows.background_updater) { //drawing not changing elements
                ILI9341_fill_color_DMA(NAVY); //background
                keyboard.background_color = NAVY;

                ili9341_draw_rect(0, 0, 60, 20, BLACK, 1); //and buttons frames
                uint16_t y = 6;
                ILI9341_draw_formatted_line(27, &y, WHITE, BLACK, "<");

                ili9341_draw_rect(60, 0, 60, 20, YELLOW, 1);
                y = 6;
                ILI9341_draw_formatted_line(81, &y, BLACK, YELLOW, "MAN");

                ili9341_draw_rect(120, 0, 60, 20, GREEN, 1);
                y = 6;
                ILI9341_draw_formatted_line(141, &y, RED, GREEN, "GSM");

                ili9341_draw_rect(180, 0, 60, 20, ORANGE, 1);
                y = 6;
                ILI9341_draw_formatted_line(198, &y, CYAN, ORANGE, "GNSS");

                Windows.background_updater = true;
            } else {
                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) {
                    RTC_read_date_and_time();

                    uint16_t y = 30;
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "RTC time: %4d-%02d-%02d %02d:%02d:%02d", RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second);
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "T.Z.: %2d ", RTC_Date_and_Time.RTC_time_zone);
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "D.S.T: %s", is_daylight_saving_time(RTC_Date_and_Time.RTC_year + 2000, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour) ? "YES" : "NO");

                    const char *timesourceintext =
                            (RTC_Date_and_Time.time_sync == NONE_sync) ? "NONE        " :
                            (RTC_Date_and_Time.time_sync == GSM_sync) ? "GSM Module  " :
                            (RTC_Date_and_Time.time_sync == GNSS_sync) ? "GNSS Module " : "Manual input";
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "Date/time source: %s", timesourceintext);

                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "GSM lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GSM_year + 2000, A7672EGSM.GSM_month, A7672EGSM.GSM_day, A7672EGSM.GSM_hour, A7672EGSM.GSM_minute, A7672EGSM.GSM_second);
                    if (A7672EGSM.GNSS_year + A7672EGSM.GNSS_month + A7672EGSM.GNSS_day + A7672EGSM.GNSS_hour + A7672EGSM.GNSS_minute + A7672EGSM.GNSS_second == 0)
                        ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "GNSS lock at: no GNSS lock");
                    else
                        ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "GNSS lock at: %4d-%02d-%02d %02d:%02d:%02d", A7672EGSM.GNSS_year + 2000, A7672EGSM.GNSS_month, A7672EGSM.GNSS_day, A7672EGSM.GNSS_hour, A7672EGSM.GNSS_minute, A7672EGSM.GNSS_second);
                    //ILI9341_draw_formatted_line(0, &y, WHITE, DARK_GRAY, "Touch X:%04d, Y:%04d, Z1:%04d, Z2:%04d", Read_XPT2046.X, Read_XPT2046.Y, Read_XPT2046.Z1, Read_XPT2046.Z2);

                    Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
                }

                draw_keyboard(Windows.keyboardAction);

                if (Windows.keyboardAction == OPEN) {
                    uint16_t y = 130;
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "MAN TIME: ");

                    uint16_t x = 60;
                    for (uint16_t clr = 0; clr < 17; clr++) {
                        y = 130;
                        uint16_t background = (pressCount == clr) ? RED : NAVY;

                        if ((clr < 14))
                            ILI9341_draw_formatted_line(x, &y, GREEN, background, "%01d", DT[clr]); //draw digits of the date and time
                        else if (clr == 14)
                            ILI9341_draw_formatted_line(x, &y, GREEN, background, "%c", TZside); //draw time zone direction
                        else if (clr > 14 && clr < 17)
                            ILI9341_draw_formatted_line(x, &y, GREEN, background, "%01d", TZ[clr - 15]); //draw digits of the time zone
                        x += 6;
                        if (clr == 3 || clr == 5) {//Year-Month-Day
                            y = 130;
                            ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, "-");
                            x += 6;
                        } else if (clr == 7) {//Year-Month-Day //space
                            y = 130;
                            ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, " ");
                            x += 6;
                        } else if (clr == 9 || clr == 11) {//Year-Month-Day Hour:Minutes:Seconds
                            y = 130;
                            ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, ":");
                            x += 6;
                        } else if (clr == 13) {//Year-Month-Day Hour:Minutes:Seconds //space
                            y = 130;
                            ILI9341_draw_formatted_line(x, &y, GREEN, NAVY, " ");
                            x += 6;
                        }
                    }

                    if (Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL){ //infinite press protection
                        countProtection = 55;
                    }
                    else { // if touch pressing hard enough
                        for (uint8_t i = 0; i < 16; i++) { //find where is pressing
                            key_data *key = &keysMap.keyboard_buttons[i];
                            if ((Read_XPT2046.X >= key->X0) && (Read_XPT2046.X <= key->X1) && (Read_XPT2046.Y >= key->Y0) && (Read_XPT2046.Y <= key->Y1)) {
                                if (countProtection != pressCount) { //accept only once and one symbol per pressing
                                    Windows.once_per_second_update += 1;
                                    if (key->digit <= 9) {
                                        if (pressCount < 14) { //changing Date and Time digits
                                            DT[pressCount] = key->digit;
                                            pressCount++;
                                        } else if (pressCount <= 16) {
                                            TZ[pressCount - 15] = key->digit; //change time zone digits  
                                            pressCount++;
                                        }
                                    } else if (pressCount == 14 && ((key->value == '-') || (key->value == '+'))) { //only if changing time zone and using + or - symbols
                                        pressCount++;
                                        TZside = key->value;
                                    }

                                    if (pressCount == 17 && key->value == '>') {
                                        uint8_t yy = (DT[2] * 10) + DT[3];
                                        uint8_t MM = (DT[4] * 10) + DT[5];
                                        uint8_t dd = (DT[6] * 10) + DT[7];
                                        uint8_t hh = (DT[8] * 10) + DT[9];
                                        uint8_t mm = (DT[10] * 10) + DT[11];
                                        uint8_t ss = (DT[12] * 10) + DT[13];

                                        y = 130;
                                        int8_t tzcheck = (TZ[0] * 10) + TZ[1];
                                        if (TZside == '-')// if time zone is negative
                                            tzcheck = 0 - tzcheck; //update time zone

                                        if (is_time_correct(yy, MM, dd, hh, mm, ss) && ((tzcheck >= -12) && (tzcheck <= 14))) {
                                            ILI9341_draw_formatted_line(204, &y, GREEN, NAVY, "SAVED");
                                            RTC_Date_and_Time.RTC_time_zone = tzcheck; //update time zone
                                            //apply_timezone(&yy, &MM, &dd, &hh, RTC_Date_and_Time.RTC_time_zone); //uncomet if set utc time
                                            RTC_date_and_time_sync(datetime_to_RTC_format(yy, MM, dd, hh, mm, ss), MAN_sync);
                                        } else {
                                            ILI9341_draw_formatted_line(204, &y, RED, NAVY, "ERROR");
                                        }
                                    } else if ((pressCount > 2) && (key->value == '<')) {
                                        pressCount--;
                                    } else if (key->value == 'x') { //clear whole line and close keyboard
                                        for (uint8_t x = 2; x < 14; x++) DT[x] = 0;
                                        pressCount = 2;
                                        uint16_t y = 130;
                                        ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "                                             ");
                                        Windows.Window = TIME_WINDOW;
                                        Windows.keyboardAction = CLOSE;
                                        Windows.once_per_second_update = 88; //just  random digit
                                    }
                                    countProtection = pressCount;
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
                    Windows.keyboardAction = CLOSE;
                    Windows.once_per_second_update = 0;
                } else if (XPT2046_switch(1016, 1932, 3700, 4000)) { // MAN
                    Windows.once_per_second_update = 0;
                    Windows.keyboardAction ^= 1;
                } else if (XPT2046_switch(2032, 2948, 3700, 4000)) { // GSM
                    Windows.once_per_second_update = 0;
                    uint16_t y = 130;
                    ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "           GSM time reset!              ");
                    //RTC_Date_and_Time.time_sync = NONE_sync;          
                    A7672E_work.source = GSM;
                    A7672EGSM.GSM_time_corect = false;
                    A7672E_work.cycle = false;
                    A7672E_work.state = SET;
                    if (Windows.keyboardAction == OPEN) {
                        Windows.Window = TIME_WINDOW;
                        Windows.keyboardAction = CLOSE;
                        Windows.once_per_second_update = 89;
                    }
                } else if (XPT2046_switch(3048, 3964, 3700, 4000)) { // GNSS
                    Windows.once_per_second_update = 0;
                    uint16_t y = 130;
                    if (A7672EGNSS.mode != 0) {
                        ILI9341_draw_formatted_line(0, &y, GREEN, NAVY, "           GNSS time reset!            ");
                        //RTC_Date_and_Time.time_sync = GSM_sync;
                        A7672E_work.source = GNSS;
                        A7672E_work.cycle = false;
                        A7672EGSM.GNSS_time_corect = false;
                        A7672E_work.state = SET;
                    } else {
                        ILI9341_draw_formatted_line(0, &y, RED, NAVY, "              NO GNSS lock!              ");
                    }
                    if (Windows.keyboardAction == OPEN) {
                        Windows.Window = TIME_WINDOW;
                        Windows.keyboardAction = CLOSE;
                        Windows.once_per_second_update = 89;
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
            static uint8_t countProtection[3] = {55};
            uint16_t text_color = WHITE;

            if (!Windows.background_updater) { //drawing not changing elements            
                keyboard.background_color = DARK_GREEN;

                ILI9341_fill_color_DMA(keyboard.background_color); //background

                ili9341_draw_rect(0, 0, 48, 20, BLACK, 1); //and buttons frames
                uint16_t y = 6;
                ILI9341_draw_formatted_line(18, &y, WHITE, BLACK, "<");

                ili9341_draw_rect(48, 0, 48, 20, YELLOW, 1);
                y = 6;
                ILI9341_draw_formatted_line(60, &y, BLACK, YELLOW, "LATI");

                ili9341_draw_rect(96, 0, 48, 20, GREEN, 1);
                y = 6;
                ILI9341_draw_formatted_line(108, &y, RED, GREEN, "LONG");

                ili9341_draw_rect(144, 0, 48, 20, ORANGE, 1);
                y = 6;
                ILI9341_draw_formatted_line(156, &y, CYAN, ORANGE, "ALTI");

                ili9341_draw_rect(192, 0, 48, 20, RED, 1);
                y = 6;
                ILI9341_draw_formatted_line(204, &y, WHITE, RED, "AUTO");

                Windows.background_updater = true;
            } else {
                if (Windows.once_per_second_update != RTC_Date_and_Time.RTC_sys_time) { //update every second except...
                    RTC_read_date_and_time();
                    uint16_t y = 30;
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Latitude: %2d.%04d° ", solar_params.latitude / 10000, abs(solar_params.latitude % 10000));
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Longitude: %3d.%04d° ", solar_params.longitude / 10000, abs(solar_params.longitude % 10000));
                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "Altitude: %4dm ", solar_params.altitude);
                    Windows.once_per_second_update = RTC_Date_and_Time.RTC_sys_time;
                }

                draw_keyboard(Windows.keyboardAction);

                if (Windows.keyboardAction == OPEN) {
                    uint16_t y = 130;
                    uint16_t x = 60;
                    uint8_t location_param = changing_current_param - 1;
                    uint8_t *buffers[] = { LAT, LNG, ALT };
                    uint8_t *target_buffer = buffers[location_param];
                    uint8_t *press = &pressCount[location_param];

                    ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, location_param_names[location_param].name);
                    x = location_param_names[location_param].x;

                    for (uint16_t clr = 0; clr < total[location_param] + 1; clr++) {
                        y = 130;
                        uint16_t background = (*press == clr) ? RED : keyboard.background_color;

                        if (clr == 0) { //draw latitude symbol - or + (default)
                            ILI9341_draw_formatted_line(x, &y, text_color, background, "%c", posneg[location_param]);
                        } else if (clr < total[location_param]) {
                            y = 130;
                            uint8_t value = 0;
                            value = target_buffer[clr - 1];
                            ILI9341_draw_formatted_line(x, &y, text_color, background, "%01d", value); //draw current data                          
                        }
                        x += 6;
                        const int8_t dot_pos[] = {2, 3, -1};
                        if (clr == dot_pos[location_param]) { // draw , for laitude after 2 digits and for longitude after 3 digits and skip for altitude
                            y = 130;
                            ILI9341_draw_formatted_line(x, &y, text_color, keyboard.background_color, ",");
                            x += 6;
                        }

                    }

                    if (Read_XPT2046.Z1 < XPT_PRES_STRENGTH_LVL) { //infinite press protection
                        countProtection[location_param] = 55;
                    } else { // if touch pressing hard enough
                        uint8_t digits = total[location_param];                        
                        for (uint8_t i = 0; i < 16; i++) { //find where is pressing
                            key_data *key = &keysMap.keyboard_buttons[i];
                            if ((Read_XPT2046.X >= key->X0) && (Read_XPT2046.X <= key->X1) && (Read_XPT2046.Y >= key->Y0) && (Read_XPT2046.Y <= key->Y1)) {
                                if (countProtection[location_param] != *press) { //accept only once and one symbol per pressing
                                    Windows.once_per_second_update += 1;
                                    if (key->digit <= 9) {
                                        if ((*press < digits) && *press > 0) { //changing digits only after + or - symbol                                                                                    
                                            target_buffer[(*press) - 1] = key->digit;
                                            (*press)++;
                                        }
                                    } else if (*press == 0 && ((key->value == '-') || (key->value == '+'))) { //only if changing  + or - symbols
                                        (*press)++;
                                        posneg[location_param] = key->value;
                                    }

                                    if (*press == digits && key->value == '>') {// pressing done button
                                        int32_t final_result = digits_to_number(target_buffer, digits - 1); //extract digit from buffer
                                        if (final_result > limits[location_param].max_value) { //check limits if they are too big
                                            ILI9341_draw_formatted_line( 204, &y, RED, keyboard.background_color, "ERROR" );
                                        }
                                        else { //if long, lat and alt is correct
                                            if (posneg[location_param] == '-') { //if it was negative
                                                final_result = -final_result;
                                            }
                                            *limits[location_param].target = final_result;
                                            ILI9341_draw_formatted_line( 204, &y, GREEN, keyboard.background_color, "SAVED" );
                                        }
                                    } else if ((*press > 0) && (key->value == '<')) {
                                        (*press)--;
                                    } else if (key->value == 'x') { //clear whole line and close keyboard  
                                        memset(target_buffer, 0, digits);
                                        posneg[location_param] = '+'; //set back default symbol
                                        *press = 0;
                                        uint16_t y = 130;
                                        ILI9341_draw_formatted_line(0, &y, text_color, keyboard.background_color, "                                             ");
                                        Windows.background_updater = false;
                                        Windows.keyboardAction = CLOSE;
                                        Windows.once_per_second_update = 88; //just  random digit
                                    }
                                    countProtection[location_param] = *press;
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
                    Windows.keyboardAction = CLOSE;
                    changing_current_param = 0;
                    Windows.once_per_second_update = 0;
                } else if (XPT2046_switch(844, 1624, 3700, 4000)) { // LATI
                    if ((changing_current_param == 2) || (changing_current_param == 3)) {
                        Windows.background_updater = false;
                        if (Windows.keyboardAction == CLOSE)
                            Windows.keyboardAction = OPEN;
                    } else {
                        Windows.keyboardAction ^= 1;
                    }
                    Windows.once_per_second_update = 0;
                    changing_current_param = 1;
                } else if (XPT2046_switch(1656, 2436, 3700, 4000)) { // LONG
                    if ((changing_current_param == 1) || (changing_current_param == 3)) {
                        Windows.background_updater = false;
                        if (Windows.keyboardAction == CLOSE)
                            Windows.keyboardAction = OPEN;
                    } else {
                        Windows.keyboardAction ^= 1;
                    }
                    Windows.once_per_second_update = 0;
                    changing_current_param = 2;
                } else if (XPT2046_switch(2468, 3248, 3700, 4000)) { // ALTI
                    if ((changing_current_param == 1) || (changing_current_param == 2)) {
                        Windows.background_updater = false;
                        if (Windows.keyboardAction == CLOSE)
                            Windows.keyboardAction = OPEN;
                        ;
                    } else {
                        Windows.keyboardAction ^= 1;
                    }
                    Windows.once_per_second_update = 0;
                    changing_current_param = 3;
                } else if (XPT2046_switch(3280, 4000, 3700, 4000)) { // AUTO
                    uint16_t y = 130;
                    if (!A7672EGNSS.mode) {
                        ILI9341_draw_formatted_line(100, &y, RED, keyboard.background_color, "NO GNSS LOCK!");
                    } else {
                        solar_params.latitude = (int32_t) A7672EGNSS.lat / 1000;
                        solar_params.longitude = (int32_t) A7672EGNSS.log / 1000;
                        solar_params.altitude = (int16_t) A7672EGNSS.alt;
                        ILI9341_draw_formatted_line(100, &y, GREEN, keyboard.background_color, "UPDATED");
                    }
                    Windows.keyboardAction = CLOSE;
                    Windows.once_per_second_update = 0;
                    changing_current_param = 4;
                }

            }

        }
            break;
        case TOWER_WINDOW:
            break;
        case SETTINGS_WINDOW:
            break;
    }
}

