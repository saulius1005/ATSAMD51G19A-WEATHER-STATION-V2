#include "settings.h"
#include "TowersVar.h"

void Towers_init(){
    for (uint16_t i = 0; i < A7672E_NET.towers_in_total; i++){
        towers[i].state = SEND;
        towers[i].id = i; //start id form 0 to 255 it will be total up to 256 towers
    }
}

uint16_t fast_atoi_hex(const char *p, uint8_t digits){// char to hex, also char int to int are in a7672e.c, also char float to int also in a7672e.c
    uint16_t value = 0;

    for (uint8_t i = 0; i < digits; i++){
        char c = p[i];
        value <<= 4;
        if (c >= '0' && c <= '9')
            value |= c - '0';
        else if (c >= 'A' && c <= 'F')
            value |= c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            value |= c - 'a' + 10;
    }

    return value;
}

void Tower_COM_DATA_Parser(char* buf, uint8_t id){
    uint8_t i = 0;

    while (i < RS485_RX_BUFFER_SIZE && buf[i] != '['){ //search frame beginning symbol [
        i++;
    }
    if (i < RS485_RX_BUFFER_SIZE){ //if found 
        uint8_t crc_data[15];
        i++; //parse data      

        for (uint8_t j = 0; j < 15; j++) {
            crc_data[j] = (uint8_t)fast_atoi_hex(&buf[(j * 2) + 1], 2);
        }
        
        uint8_t  idno  = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;
        uint16_t AZ  = fast_atoi_hex(&buf[i], 4); i += 4;
        uint16_t EL  = fast_atoi_hex(&buf[i], 4); i += 4;
        uint16_t PVU = fast_atoi_hex(&buf[i], 4); i += 4;
        uint16_t PVI = fast_atoi_hex(&buf[i], 3); i += 3;
        uint8_t  ES  = (uint8_t)fast_atoi_hex(&buf[i], 1); i += 1;

        uint16_t SU  = fast_atoi_hex(&buf[i], 3); i += 3;
        uint16_t SI  = fast_atoi_hex(&buf[i], 3); i += 3;
        uint16_t LU  = fast_atoi_hex(&buf[i], 3); i += 3;
        uint16_t LI  = fast_atoi_hex(&buf[i], 3); i += 3;
        uint8_t  CC  = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;
        
        //test message: [004792126807dc3fb025a0370ee15b58] // id 0 ,..., bad crc
        //test message: [004792126807dc3fb025a0370ee15b10] //id 0 ,...,  crc ok
        //test message: [01487312c307f3441026202c9680733e] //id 1, ..., crc ok
 
        bool crc_ok = verify_crc8_cdma2000(crc_data,(i - 3)/2, CC);//crc buf, total bytes not symbol count, received crc

        //RS485_printf("crc ok? %s\r\n", crc_ok? "YEP":"NOTI NOTI");//for debug
        
        if(!crc_ok)//skip further code if crc is not correct
            return;
        
        if(id != idno) // only if answer from correct id
            return;
            
        tower_t *tower = &towers[id];

        tower -> position.azimuth = AZ;
        tower -> position.elevation = EL;

        tower -> panel.voltage = PVU;
        tower -> panel.current = PVI;
        tower -> panel.power = (uint32_t)PVU*PVI;

        tower -> es = ES;

        tower -> az_motor.voltage = SU;
        tower -> az_motor.current = SI;
        tower -> az_motor.power = (uint32_t)SU*SI;

        tower -> el_motor.voltage = LU;
        tower -> el_motor.current = LI;  
        tower -> el_motor.power = (uint32_t)LU*LI;

        tower -> update_time.year = RTC_Date_and_Time.RTC_year + 2000;
        tower -> update_time.month = RTC_Date_and_Time.RTC_month;
        tower -> update_time.day = RTC_Date_and_Time.RTC_day;
        tower -> update_time.hour = RTC_Date_and_Time.RTC_hour;
        tower -> update_time.minute = RTC_Date_and_Time.RTC_minute;
        tower -> update_time.second = RTC_Date_and_Time.RTC_second;
        
        memcpy(tower ->prepared_to_server, buf, 30); //prepare data to server
    
    }
}

void Tower_COM_sequence(uint8_t id){
    static char buf[RS485_RX_BUFFER_SIZE] = {0};
    
    switch(towers[id].state){
        case SEND:{ 
            if (Periodic_Checker_Devices.TOWERS.update_stat == PREPARED) {
                uint8_t crcbuf[16] ={0};
                memset(buf, 0, RS485_RX_BUFFER_SIZE); //clear buf
                //converting elevation to uint type and do NOT FORGET use int16_t at receiver side
                USART_RS485_printf(RS485_TOWER_REG, "{%02x%04x%04x%02x%x%03x%02x}\r\n", id, solar_params.coarse_azimuth, (uint16_t)solar_params.coarse_elevation, sensors.WIND.speed, sensors.WIND.direction, sensors.SUN.level, crc8_cdma2000(crcbuf,TowerCRC(id, crcbuf)));//id,azimuth,elevation, wind speed, wind direction, crc8
                towers[id].state = WAIT_RESPOND;
                Periodic_Checker_Devices.TOWERS.start_at = Periodic_Checker_Devices.period_counter;
                DMA_USART_RS485_Storage_init(buf, RS485_RX_BUFFER_SIZE, TOWER_CH); //set dma settings
                DMA_USART_RS485_Enable(true, TOWER_CH); //enable dma
            }
        }break;
        case WAIT_RESPOND:{
            if (Periodic_Checker_Devices.period_counter >= Periodic_Checker_Devices.TOWERS.start_at + Periodic_Checker_Devices.TOWERS.respond_time) {
                //USART_RS485_printf(RS485_TOWER_REG, "end");//waiting time end debug
                DMA_USART_RS485_Enable(false, TOWER_CH);   //stop reading 
                towers[id].state = PROCESS;
            }           
        }break;
        case PROCESS:
            //RS485_printf("process %01d", id);//for debug
            Tower_COM_DATA_Parser(buf, id);            
            towers[id].state = COMPLETE;
            Periodic_Checker_Devices.TOWERS.update_stat = UPDATED;
            
        break;
            
    }
}

void Tower_COM(){
    static uint8_t switcher = 0;

    if(A7672E_init.status != WORK)//wait all GSM module initialization and start data sending after that (when main window is start showing up
        return;
    
    if(!switcher){ //measure and send data once for all towers starting with id1 not id0
        Sensors_COM();// checking enveroment data
        RTC_read_date_and_time();
        calculate_solar_position();
        apply_all_elevation_modifies();// uses temperature, pressure and humidity data.        
    }

    if(towers[switcher].state == COMPLETE){// if data processed complete send request to next tower
        switcher = (switcher + 1) % A7672E_NET.towers_in_total;
        towers[switcher].state = SEND;
    }
    else {//if tower state is SEND or WAIT
        
        Tower_COM_sequence(switcher);
    }
}