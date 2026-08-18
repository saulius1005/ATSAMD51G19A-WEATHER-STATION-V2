#include "settings.h"
#include "TowersVar.h"

/*  
 * 
 * Old weather station with AVR64DD32 data frame for towers (original v1.0):
 * 
 * //sending data to towers. Id can be 0-255 (0-FF hex). Data from towers will be send to logger from towers directly (never implemented)
 * 
 * 
        USART_printf(0, "{%02x%04x%04x%02x%x%03x%02x}\r\n",
        (uint8_t)i, //FF 0-255 // 
        (uint16_t)SUN.adjazimuth, //FFFF 0-35999
        (uint16_t)SUN.adjelevation, //FFFF 0-8999
        (uint8_t)readwindspeed.Result, //FF 0-30
        (uint8_t)readwinddirection.Result, //F 0-7
        (uint16_t)SUN.sunlevel, //FFF 0-600 //not actual. after reaserch (power generation starts when Sun is about -8 degrees below horizon in clear sky and -6 degrees when is heavy clouds)
        (uint8_t)crc8_cdma2000(crcbuf,TowerCRC(i, crcbuf)));
 */

/*
 *
 * New weather stattion with ATSAMD51G19A data frame for towers (2.0):
 * 
 * sending data to towers. Id can be 0-255 (0-FF hex). Data from towers sending directly back to weather station
 
        RS485_printf("{%02x%04x%04x%02x%x%02x}\r\n", 
        id, //0-255
        solar_params.coarse_azimuth, //0-35999
        (uint16_t)solar_params.coarse_elevation, //sending as uint16_t but actualy it is signed exmp: -700 : 8999 
        WIND.speed, //0-30
        WIND.direction, //0-7
                        //sun level removed
        crc8_cdma2000(crcbuf,TowerCRC(id, crcbuf))); //0-255
 */

/* 
 * Tower respond data frame example from tower controller(original v1.0):
 * frame start with [ and ends with ]
 * last 2 hex symbols is cdma2000 crc8
 * 
  USART_printf(0, "[%02x%04x%04x%03x%03x%x%03x%03x%03x%03x%02x]\r\n",
	(uint8_t)DEVICE_ID_NUMBER,
	(uint16_t)SensorData.HPElevation,
	(uint16_t)SensorData.HPAzimuth,
	(uint16_t)SensorData.PVU,
	(uint16_t)abs(SensorData.PVI),
	(uint8_t)SensorData.endSwitches,
	(uint16_t)StepperMotor.measuredVoltage,
	(uint16_t)abs(StepperMotor.measuredCurrent),
	(uint16_t)LinearMotor.measuredVoltage,
	(uint16_t)abs(LinearMotor.measuredCurrent),
	(uint8_t)crc8_cdma2000_id(DEVICE_ID_NUMBER)
	);
 */  

/*
 * Tower respond data frame example (v2.0):
 * frame start with [ and ends with ]
 * last 2 hex symbols is cdma2000 crc8
 
    same as v 1.0, only PVU is 4 Bytes instead of 3
 
 */
void Towers_init(){
    for (uint8_t i = 0; i < TOWER_COUNT; i++){
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
        
        //test message: [004792126807dc3fb025a0370ee15b58] //bad crc
        //test message: [004792126807dc3fb025a0370ee15b10] //crc ok
 
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
    
    }
}

void Tower_COM_sequence(uint8_t id){
    static char buf[RS485_RX_BUFFER_SIZE] = {0};
    
    switch(towers[id].state){
        case SEND:{ 
            uint8_t crcbuf[16] ={0};
            memset(buf, 0, RS485_RX_BUFFER_SIZE); //clear buf
            //converting elevation to uint type and do NOT FORGET use int16_t at receiver side
            RS485_printf("{%02x%04x%04x%02x%x%02x}\r\n", id, solar_params.coarse_azimuth, (uint16_t)solar_params.coarse_elevation, WIND.speed, WIND.direction, crc8_cdma2000(crcbuf,TowerCRC(id, crcbuf)));//id,azimuth,elevation, wind speed, wind direction, crc8
            towers[id].state = WAIT_RESPOND;
            TCC1_ON(2000000 / TOWER_COUNT); //dynamic data request time interval added EASY :D
            DMA_USART_RS485_Temp_Circular_BYTE_init(buf, RS485_RX_BUFFER_SIZE, TOWER_CH); //set dma settings
            DMA_USART_RS485_Circular_BYTE_ENABLE(true, TOWER_CH); //enable dma
        }break;
        case WAIT_RESPOND:
            if(TCC1_timeout){ //after timeout 
                DMA_USART_RS485_Circular_BYTE_ENABLE(false, TOWER_CH);   //stop reading 
                TCC1_timeout = false; //reset timer flag
                towers[id].state = PROCESS;
            }           
        break;
        case PROCESS:
            //RS485_printf("process %01d", id);//for debug
            Tower_COM_DATA_Parser(buf, id);
            
            towers[id].state = COMPLETE;
        break;
            
    }
}

void Tower_COM(){
    static uint8_t switcher = 0;

    if(A7672E_init.status != WORK)//wait all GSM module initialization and start data sending after that (when main window is start showing up
        return;

    if(towers[switcher].state == COMPLETE){// if data processed complete send request to next tower
        switcher = (switcher + 1) % TOWER_COUNT;
        towers[switcher].state = SEND;
    }
    else {//if tower state is SEND or WAIT
        Tower_COM_sequence(switcher);
    }
}