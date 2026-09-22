#include "settings.h"
#include "SensorsVar.h"


char * WindDirNames(){ // return wind direction short name
	switch(sensors.WIND.direction) {        
		case 1: return "NE";  // Northeast
		case 2: return "E ";  // East
		case 3: return "SE"; // Southeast
		case 4: return "S ";  // South
		case 5: return "SW"; // Southwest
		case 6: return "W ";  // West
		case 7: return "NW"; // Northwest
	}
	return "N ";  // North
}


void Sensors_parser(char* buf){
    uint8_t i = 0;

    while (i < RS485_RX_BUFFER_SIZE && buf[i] != '['){ //search frame beginning symbol [
        i++;
    }
    
    if (i < RS485_RX_BUFFER_SIZE){ //if found 
        uint8_t crc_data[8];
        i++; //parse data      

        for (uint8_t j = 0; j < 8; j++) {
            crc_data[j] = (uint8_t)fast_atoi_hex(&buf[(j * 2) + 1], 2);
        }
        
        int8_t  temperature  = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;
        uint16_t pressure  = fast_atoi_hex(&buf[i], 4); i += 4;
        uint8_t humidity  = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;
        uint8_t wss = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;
        uint8_t wds = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;
        uint16_t  sls  = fast_atoi_hex(&buf[i], 4); i += 4;
        uint8_t  CC  = (uint8_t)fast_atoi_hex(&buf[i], 2); i += 2;

 
        bool crc_ok = verify_crc8_cdma2000(crc_data,(i - 3)/2, CC);//crc buf, total bytes not symbol count, received crc

        //RS485_printf("crc ok? %s\r\n", crc_ok? "YEP":"NOTI NOTI");//for debug
        
        if(!crc_ok)//skip further code if crc is not correct
            return;
                
        sensors.BME680.temperature = temperature;
        sensors.BME680.pressure = pressure;
        sensors.BME680.humidity = humidity;
        
        sensors.WIND.speed = wss;
        sensors.WIND.direction = wds;
        
        sensors.SUN.level = sls;
    }
}


void Sensors_COM(){
    static char buf[RS485_RX_BUFFER_SIZE] = {0};
    
    switch(sensors.state){
        case SEND:{ 
            if (Periodic_Checker_Devices.SENSORS.update_stat == PREPARED) {
                memset(buf, 0, RS485_RX_BUFFER_SIZE); //clear buf
                RS485_printf(RS485_SENSORS_REG, "{GET}\r\n"); //send data request
                sensors.state = WAIT_RESPOND;
                DMA_USART_RS485_Temp_Circular_BYTE_init(buf, RS485_RX_BUFFER_SIZE, SENSORS_CH); //set dma settings
                DMA_USART_RS485_Circular_BYTE_ENABLE(true, SENSORS_CH); //enable dma
            }
        }break;
        case WAIT_RESPOND:{
            if (Periodic_Checker_Devices.period_counter >= Periodic_Checker_Devices.SENSORS.start_at + Periodic_Checker_Devices.SENSORS.respond_time) {
                //RS485_printf(RS485_SENSORS_REG, "end");//timming is working
                DMA_USART_RS485_Circular_BYTE_ENABLE(false, SENSORS_CH);   //stop reading 
                sensors.state = PROCESS;
            }           
        }break;
        case PROCESS:        
            Sensors_parser(buf);
            sensors.state = SEND;
            Periodic_Checker_Devices.SENSORS.update_stat = UPDATED;         
        break;
            
    }
}
