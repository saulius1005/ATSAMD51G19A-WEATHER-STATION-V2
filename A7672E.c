#include "settings.h"
#include "A7672EVar.h"

void normalize_at_response(char *buf){
    uint16_t r = 0; // read
    uint16_t w = 0; // write

    // 1. konvertuojam visus \r ir \n á tarpà
    while (buf[r] != '\0') {
        if (buf[r] == '\r' || buf[r] == '\n') {
            // áraðom tik VIENÀ tarpà vietoj keliø CR/LF
            if (w > 0 && buf[w-1] != ' ')
                buf[w++] = ' ';
        } else {
            buf[w++] = buf[r];
        }
        r++;
    }

    // 2. paðalinam paskutiná tarpà (jei yra)
    if (w > 0 && buf[w-1] == ' ')
        w--;

    // 3. pridedam vienà \r\n gale
    //buf[w++] = '\r';
    //buf[w++] = '\n';
    buf[w] = '\0';
}

void remove_echo(char *buf, const char *cmd){
    char *pos = strstr(buf, cmd);
    if (!pos) return; // jei neranda, nedaryk nieko

    size_t cmd_len = strlen(cmd);

    // perkeliam likusá stringà á buf pradþià
    memmove(buf, pos + cmd_len, strlen(pos + cmd_len) + 1);
}

bool extract_at_response(char *buf, const char *cmd, uint8_t packs){
    char *start = strstr(buf, cmd);
    if (!start) return false;

    char *p = start;
    uint8_t found = 0;

    while (*p && found < (packs * 2)) { // 1 pack = 2x \r\n
        if (*p == '\r' && *(p + 1) == '\n') {
            found++;
            p++; // skip \n
        }
        p++;
    }

    if (found < (packs * 2))
        return false;

    // nukopijuojam tik reikalingà dalá á pradþià
    size_t len = p - start;
    memmove(buf, start, len);
    buf[len] = '\0';

    return true;
}

void terminal_header(uint16_t *y, const char *title) {
    ILI9341_fill_color_DMA(BLACK);
    *y = 0;
    ILI9341_draw_formatted_line(65, y, RED, BLACK, "%s", title);
}

void process_and_print(uint16_t *y, CommandList_t *cmd, char *buf) {
    extract_at_response(buf, cmd->ATTX, cmd->PCKCNT);
    remove_echo(buf, cmd->ATTX);
    normalize_at_response(buf);

    color_segment_t segments[2];

    size_t txlen = strlen(cmd->ATTX);
    if (txlen > 0) txlen--;

    segments[0] = (color_segment_t){ cmd->ATTX, YELLOW, BLACK };
    segments[1] = (color_segment_t){ buf, GREEN, BLACK };

    ILI9341_draw_colored_line(1, y, segments, 2);
}

void extract_gnss_data(char *buf) {
    char *start = strchr(buf, ' ');   // pirmas tarpas
    if (!start) return;
    
    start++; // pereinam uþ tarpo

    char *end = strchr(start, '\r'); // pirmas \r po duomenø
    if (!end) return;

    *end = '\0'; // nukerpam ties \r

    // perstumiam rezultatà á pradþià
    memmove(buf, start, strlen(start) + 1);
}

int fast_atoi(const char *p) {
    int val = 0;
    while (*p >= '0' && *p <= '9') {
        val = val * 10 + (*p - '0');
        p++;
    }
    return val;
}

uint32_t fast_atof_1e7(const char *p) {
    uint32_t val = 0;

    // integer dalis
    while (*p >= '0' && *p <= '9') {
        val = val * 10 + (*p - '0');
        p++;
    }

    val *= 10000000;

    if (*p == '.') {
        p++;
        uint32_t frac = 0;
        uint32_t scale = 1000000; // nes jau viena pozicija po kablelio

        while (*p >= '0' && *p <= '9' && scale) {
            frac += (*p - '0') * scale;
            scale /= 10;
            p++;
        }

        val += frac;
    }

    return val;
}

uint8_t is_leap_year(volatile uint8_t y){
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

uint8_t days_in_month(volatile uint8_t month, volatile uint8_t year){
    static const uint8_t d[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if (month == 2 && is_leap_year(year)) return 29;
    return d[month - 1];
}

uint8_t day_of_week(uint8_t y, uint8_t m, uint8_t d){
    static uint8_t t[] = {0, 3, 2, 5, 0, 3,
                          5, 1, 4, 6, 2, 4};

    uint16_t full_year = 2000 + y;

    full_year -= (m < 3);
    return (full_year + full_year/4 - full_year/100 + full_year/400 + t[m-1] + d) % 7;
}

uint8_t last_sunday(volatile uint8_t month, volatile uint16_t year){
    uint8_t last_day = days_in_month(month, year);

    for (uint8_t d = last_day; d >= 1; d--){
        if (day_of_week(year, month, d) == 0)
            return d;
    }
    return 1;
}

uint8_t is_dst(volatile uint8_t year, volatile uint8_t month, volatile uint8_t day, volatile uint8_t hour){
    uint8_t start_day = last_sunday(3, year);  // March
    uint8_t end_day   = last_sunday(10, year); // October

    // DST starts: last Sunday March 01:00 UTC
    if (month > 3 && month < 10)
        return 1;

    if (month == 3){
        if (day > start_day) return 1;
        if (day == start_day && hour >= 1) return 1;
        return 0;
    }

    if (month == 10){
        if (day < end_day) return 1;
        if (day == end_day && hour < 1) return 1;
        return 0;
    }

    return 0;
}

int8_t apply_timezone_with_dst(volatile uint8_t year, volatile uint8_t month, volatile uint8_t day, volatile uint8_t hour, int8_t TZ){
    if (is_dst(year, month, day, hour))
        return TZ + 1;
    
    return TZ;
}

void apply_timezone(volatile uint8_t *year, volatile uint8_t *month, volatile uint8_t *day, volatile uint8_t *hour, int8_t TZ){
    int16_t h = *hour + apply_timezone_with_dst(*year, *month, *day, *hour, TZ);

    if (h >= 24)
    {
        h -= 24;
        (*day)++;

        if (*day > days_in_month(*month, *year))
        {
            *day = 1;
            (*month)++;

            if (*month > 12)
            {
                *month = 1;
                (*year)++;
            }
        }
    }
    else if (h < 0)
    {
        h += 24;
        (*day)--;

        if (*day == 0)
        {
            (*month)--;

            if (*month == 0)
            {
                *month = 12;
                (*year)--;
            }

            *day = days_in_month(*month, *year);
        }
    }

    *hour = (uint8_t)h;
}

bool is_time_correct(uint8_t Y, uint8_t M, uint8_t D, uint8_t h, uint8_t m, uint8_t s){

    if (M < 1 || M > 12) return false; //basic checks
    if (h > 23) return false;
    if (m > 59) return false;
    if (s > 59) return false;
    if (D < 1 || D > days_in_month(M, Y)) return false; //check leap year

    return true;
}

void parse_gnss_data(char *buf, GNSS_data_list_t *out) {
    uint8_t field = 0;
    char *p = buf;

    while (*p) {

        switch (field) {
            case 0: out->mode = fast_atoi(p); break;
            case 1: out->GPS_SVs = fast_atoi(p); break;
            case 2: out->BEIDOU_SVs = fast_atoi(p); break;
            case 3: out->GLONASS_SVs = fast_atoi(p); break;
            case 4: out->GALILEO_SVs = fast_atoi(p); break;

            case 5: out->lat = fast_atof_1e7(p); break;
            case 6: out->N_S = *p; break;

            case 7: out->log = fast_atof_1e7(p); break;
            case 8: out->E_W = *p; break;

            case 9: out->date = fast_atoi(p); break;
            case 10: out->UTC_time = fast_atoi(p); break;

            case 11: out->alt = fast_atoi(p); break;
            case 12: out->speed = fast_atoi(p); break;
            case 13: out->course = fast_atoi(p); break;

            case 14: out->PDOP = fast_atof_1e7(p); break;
            case 15: out->HDOP = fast_atof_1e7(p); break;
            case 16: out->VDOP = fast_atof_1e7(p); break;

            case 17: out->NoSV = fast_atoi(p); break;
        }

        // pereinam prie kito field
        while (*p && *p != ',') p++;
        if (!*p) break;

        p++; // skip ','
        field++;
    }
    
    uint8_t yy = out->date % 100; //year is last;
    uint8_t MM = (out->date / 100) % 100;
    uint8_t dd = out->date / 10000; //day is first
    
    uint8_t hh = out->UTC_time / 10000;
    uint8_t mm = (out->UTC_time / 100) % 100;
    uint8_t ss = out->UTC_time % 100;
    
    if(is_time_correct(yy, MM, dd, hh, mm, ss)){ //check only basic date and time, skip rtc time checking
        A7672EGSM.GNSS_year  = yy;
        A7672EGSM.GNSS_month = MM;
        A7672EGSM.GNSS_day   = dd;

        A7672EGSM.GNSS_hour   = hh;
        A7672EGSM.GNSS_minute = mm;
        A7672EGSM.GNSS_second = ss;

        if(out->mode != 0){ //if time locked 2D or 3D
            apply_timezone(&A7672EGSM.GNSS_year, &A7672EGSM.GNSS_month, &A7672EGSM.GNSS_day, &A7672EGSM.GNSS_hour, RTC_Date_and_Time.RTC_time_zone);  
            A7672EGSM.GNSS_sys_time = datetime_to_RTC_format(A7672EGSM.GNSS_year, A7672EGSM.GNSS_month, A7672EGSM.GNSS_day, A7672EGSM.GNSS_hour, A7672EGSM.GNSS_minute, A7672EGSM.GNSS_second);        
        }
        A7672EGSM.GNSS_time_corect = true;
    }
    
}

void parse_gsm_datetime(char *buf) {//example: receiving "26/04/10,18:45:08+12"
    
    uint32_t yy = (buf[0]-'0')*10 + (buf[1]-'0');
    uint32_t MM = (buf[3]-'0')*10 + (buf[4]-'0');
    uint32_t dd = (buf[6]-'0')*10 + (buf[7]-'0');
    
    uint32_t hh = (buf[9]-'0')*10 + (buf[10]-'0');
    uint32_t mm = (buf[12]-'0')*10 + (buf[13]-'0');
    uint32_t ss = (buf[15]-'0')*10 + (buf[16]-'0');
    
    
    if(is_time_correct(yy, MM, dd, hh, mm, ss)){ //Check if time correct and also compare it with rtc time
        A7672EGSM.GSM_year = yy;
        A7672EGSM.GSM_month = MM;
        A7672EGSM.GSM_day = dd;    


        A7672EGSM.GSM_hour = hh;
        A7672EGSM.GSM_minute = mm;
        A7672EGSM.GSM_second = ss;

        A7672EGSM.GSM_sys_time = datetime_to_RTC_format(yy, MM, dd, hh, mm, ss); 
        A7672EGSM.GSM_time_corect = true;
    }
    
}

void extract_gsm_time(char *buf) {
    char *start = strchr(buf, '"');   // find "
    if (!start) return;
    
    start++; // pereinam uþ tarpo

    char *end = strchr(start, '"'); //
    if (!end) return;

    *end = '\0'; // nukerpam ties \r

    // perstumiam rezultatà á pradþià
    memmove(buf, start, strlen(start) + 1);
}

void dma_receive_time_SM(char * cmd , char * echo, uint32_t wait_ms) {
    static char buf[UART_RX_BUFFER_SIZE] = {0};
    
    switch(A7672E_work.state){
        case SET:            
            A7672E_work.cycle = false;
            memset(buf, 0, UART_RX_BUFFER_SIZE); //clear buf
            USART_printf("%s", cmd); //send 
            USART_DMA_Temp_Circular_BYTE_Init(buf, UART_RX_BUFFER_SIZE); //set dma settings
            USART_DMA_Circular_BYTE_ENABLE(true); //enable dma
            TC2_ON(1000 * wait_ms); //set answer waiting time
            A7672E_work.state = WAIT;
        break;

        case WAIT:
            if(TC2_timeout){ //after timeout
                USART_DMA_Circular_BYTE_ENABLE(false);   //stop reading 
                TC2_timeout = false; //reset timer flag
                A7672E_work.state = DONE;
            }
        break;
        
        case DONE:
            if(A7672E_work.source == GSM){ //if gsm time
                extract_at_response(buf, echo, 2); //remove symbols before and after message (if received)
            }
            remove_echo(buf, echo); // for GSM and GNSS time remove echo
            if(A7672E_work.source == GSM){ //for GSM
                normalize_at_response(buf); //remove \r, \n
                extract_gsm_time(buf); //get only data 
                parse_gsm_datetime(buf); // split data from time
                RTC_date_and_time_update();
                if(A7672EGSM.GSM_time_corect)
                    A7672E_work.cycle = true;
            }
            else if(A7672E_work.source == GNSS){ //if it was GNSS time
                extract_gnss_data(buf); //keep only GNSS data
                parse_gnss_data(buf, &A7672EGNSS); //keep time and date only
                RTC_date_and_time_update();
                if(A7672EGSM.GNSS_time_corect)
                    A7672E_work.cycle = true;
            }
            A7672E_work.state = SET;
            
            
        break;
    }
}

void A7672ReadNEMAGNSS(){   // READS GSM and GNSS date and time if success update RTC timer
    if(A7672E_init.status != WORK)//if not WORK mode 
        return; //skip further code
    
    //if(RTC_Date_and_Time.time_sync == GNSS_sync)//if time is synced with gnss
    //    return; //skip further code
    if(A7672E_work.source == GSM_AND_GNSS)
        return;

    uint16_t read_interval = 250; //read every 100 ms
    if(RTC_Date_and_Time.time_sync == GSM) //if synced by GSM read every 500ms of GNSS
        read_interval = 500;
    
    switch(A7672E_work.source){
        case GNSS: //Read GNSS time
            dma_receive_time_SM("AT+CGNSSINFO\r", "AT+CGNSSINFO\r\r\n+CGNSSINFO:", read_interval); //cmd and echo, wait 0.3 second
            if(A7672E_work.cycle) A7672E_work.source = GSM_AND_GNSS; //gsm synced and corected with gnss
        break;
        
        case GSM: //Read GSM time
                dma_receive_time_SM("AT+CCLK?\r", "AT+CCLK?\r", read_interval); //cmd and echo, wait 0.1 second
                if(A7672E_work.cycle) A7672E_work.source = GNSS; //switch to gnss time correction after GSM time is completed and corect             
        break;
    }
}

void extract_rssi(char *buf){
    //exp.: received buf after echo remove "16,99\r\n\r\nOK\r\n"
    char *end;
    uint8_t rssi = (uint8_t)strtol(buf, &end, 10); //read digits util first no digit simbol ,
    
    A7672E_GSM_STATUS.bad_signal = false;
    if(rssi < 32){
        A7672E_GSM_STATUS.rssi = (rssi * 2) - 113;
    }
    else { //99 and other betwean 32...99
        A7672E_GSM_STATUS.bad_signal = true;
    }
    
    if (*end == ',')
    {
        A7672E_GSM_STATUS.ber = (uint8_t)strtol(end + 1, NULL, 10); // read ber value until first no digit simbol \r
    }
};
void extract_reg_status(char *buf){
    //exp.: received buf after echo remove "0,1\r\n\r\nOK\r\n"
    char *end;
    A7672E_GSM_STATUS.n = (uint8_t)strtol(buf, &end, 10); //read digits util first no digit simbol ,
    if (*end == ',')
    {
        A7672E_GSM_STATUS.reg_status = (uint8_t)strtol(end + 1, NULL, 10); // read ber value until first no digit simbol \r
    }    
};

void dma_receive_reg_sig(char * cmd , char * echo, uint32_t wait_ms) { //sendin and receiving gsm signal signal strength and registration status
    static char buf[UART_RX_BUFFER_SIZE] = {0};
    
    switch(A7672E_GSM_STATUS_STATE.state){
        case SET:            
            A7672E_GSM_STATUS_STATE.cycle = false;
            memset(buf, 0, UART_RX_BUFFER_SIZE); //clear buf
            USART_printf("%s", cmd); //send 
            USART_DMA_Temp_Circular_BYTE_Init(buf, UART_RX_BUFFER_SIZE); //set dma settings
            USART_DMA_Circular_BYTE_ENABLE(true); //enable dma
            TCC0_ON(1000 * wait_ms); //set answer waiting time
            A7672E_GSM_STATUS_STATE.state = WAIT;
        break;

        case WAIT:
            if(TCC0_timeout){ //after timeout
                USART_DMA_Circular_BYTE_ENABLE(false);   //stop reading 
                TCC0_timeout = false; //reset timer flag
                A7672E_GSM_STATUS_STATE.state = DONE;
            }
        break;
        
        case DONE:
            remove_echo(buf, echo); // for GSM and GNSS time remove echo
            if(A7672E_GSM_STATUS_STATE.source == SIGNAL){
                extract_rssi(buf);
            }
            else if(A7672E_GSM_STATUS_STATE.source == REGISTRATION){
                extract_reg_status(buf);
            }
            A7672E_GSM_STATUS_STATE.state = SET;
            A7672E_GSM_STATUS_STATE.cycle = true;
            
        break;
    }
}

void A7672ReadGSMBasic(){   // READS GSM signal strength and registration in network status (Active all time)
    if(A7672E_init.status != WORK)//if not WORK mode 
        return; //skip further code
    if(RTC_Date_and_Time.time_sync == NONE_sync) //if no time sync
        return; //skip further code
    
    uint16_t read_interval = 2000; //read every 2 s
    
    switch(A7672E_GSM_STATUS_STATE.source){
        case SIGNAL: //Read GSM signal strength
            dma_receive_reg_sig("AT+CSQ\r", "AT+CSQ\r\r\n+CSQ: ", read_interval); //cmd and echo, wait 2 second
            if(A7672E_GSM_STATUS_STATE.cycle) A7672E_GSM_STATUS_STATE.source = REGISTRATION; //if gnss locked, next time after RTC_Date_and_Time.time_sync resting (NONE) it starts with GSM time update. if RTC_Date_and_Time.time_sync reseting with (GSM) it starts GNSS time update
        break;
        
        case REGISTRATION: //Read GSM registration status
            dma_receive_reg_sig("AT+CREG?\r", "AT+CREG?\r\r\n+CREG: ", read_interval); //cmd and echo, wait 2 second
            if(A7672E_GSM_STATUS_STATE.cycle) A7672E_GSM_STATUS_STATE.source = SIGNAL; //switch to gnss time correction after GSM time is completed and corect     
        break;
    }
}

void A7672EPowerUpRead(uint16_t * y, char * buf){ //reads all data right away after start up
    switch(A7672E_init.state){
        case SET:
            A7672E_init.cycle = false; //start cycle
            terminal_header(y, "INITIALIZATION");
            USART_DMA_Temp_Circular_BYTE_Init(buf, UART_RX_BUFFER_SIZE);
            USART_DMA_Circular_BYTE_ENABLE(true); //enable dma usart reading
            TC0_ON(25000000UL); //read all for 25 seconds
            A7672E_init.state = WAIT;
        break;

        case WAIT:{
            uint16_t temp_y = 12;
            ILI9341_draw_formatted_line(1, &temp_y, GREEN, BLACK, "%s", buf); //till waiting draw all data
            if(TC0_timeout){
                USART_DMA_Circular_BYTE_ENABLE(false); //stop reading
                TC0_timeout = false; //reset timer flag
                A7672E_init.state = SET; //reset state 
                A7672E_init.cycle = true; //stop cycle    
                *y = 0; //reset y
            }
        }break;
    }
}

void A7672ESetUp(uint16_t *y, char *buf, A7672E_Config_t *cfg){
    CommandList_t *cmd = &cfg->list[A7672E_init.step]; //set list from where we use cmd

    switch (A7672E_init.state) {
        case SET:
            if (A7672E_init.step == 0) { //set once 
                A7672E_init.cycle = false;
                terminal_header(y, cfg->header); //show title
            }
            memset(buf, 0, UART_RX_BUFFER_SIZE); //clear buf
            USART_printf("%s", cmd->ATTX); // cend cmd
            TC0_ON(1000000UL * (cmd->WaitTimeInSeconds)); // wait answer
            USART_DMA_Temp_Circular_BYTE_Init(buf, UART_RX_BUFFER_SIZE); // set up dma
            USART_DMA_Circular_BYTE_ENABLE(true); //enable usart dma
            A7672E_init.state = WAIT;
        break;

        case WAIT:
            if (TC0_timeout) { //if time end
                USART_DMA_Circular_BYTE_ENABLE(false); //disable dma
                A7672E_init.state = DONE;
            }
        break;

        case DONE:
            process_and_print(y, cmd, buf); //process and show data on screen
            if (++A7672E_init.step == cfg->size) { //check if it is the last cmd from list
                A7672E_init.cycle = true; //if yes end all cycle
                A7672E_init.step = 0; //reset steps
                *y = 0; //and reset y
            }
            A7672E_init.state = SET;
        break;
    }
}

void A7672EInit() {
    if(A7672E_init.status == WORK)//if WORK mode 
        return; //skip further code
    
    static char buf[UART_RX_BUFFER_SIZE] = {0};
    static uint16_t y = 0;   

    switch(A7672E_init.status){
        case INIT: //Read data after power on ~25 seconds
            A7672EPowerUpRead(&y, buf);
            if(A7672E_init.cycle) A7672E_init.status = HTTPSETUP;
        break;
        case HTTPSETUP: //Set up HTTP connection
            A7672ESetUp(&y, buf, &http_cfg);
            if(A7672E_init.cycle) A7672E_init.status = GNSSSETUP;
        break;
        case GNSSSETUP: //Set up GNSS 
            A7672ESetUp(&y, buf, &gps_cfg);
            if(A7672E_init.cycle) A7672E_init.status = SHOW_FIRST_WINDOW;
        break;
        case SHOW_FIRST_WINDOW: //Drawing  first window
            Windows.Window = MAIN_WINDOW;
            A7672E_init.status = WORK;
        break;
    }
}