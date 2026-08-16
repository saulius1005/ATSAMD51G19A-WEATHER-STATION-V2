#include "settings.h"
#include "TowersVar.h"

void Towers_init(){
    for (uint8_t i = 0; i < TOWER_COUNT; i++){
        towers[i].state = SEND;
        towers[i].id = i + 1;
    }
}

/*  
 * 
 * Old weaather station with AVR64DD32 data frame for towers (original v1.0):
 * 
 * //sending data to towers. Id can be 0-255 (0-FF hex). Data from towers will be send to logger from towers directly
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

void Tower_COM_sequence(uint8_t id){
    static char buf[RS485_RX_BUFFER_SIZE] = {0};
    
    switch(towers[id].state){
        case SEND:{ 
            uint8_t crcbuf[16] ={0};
            memset(buf, 0, UART_RX_BUFFER_SIZE); //clear buf
            RS485_printf("{%02x%04x%04x%02x%x%02x}\r\n", id, solar_params.coarse_azimuth, solar_params.coarse_elevation, WIND.speed, WIND.direction, crc8_cdma2000(crcbuf,TowerCRC(id, crcbuf)));//id,azimuth,elevation, wind speed, wind direction, crc8
            towers[id].state = WAIT_RESPOND;
            TCC1_ON(2000000); //send test every 2 seconds?
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
            //Do process if data received- ALL ok, move on
            //if not continue next 3 times? if no respond exp. id 2 set fault
        break;
            
    }
}

void Tower_COM(){
    static uint8_t switcher = 0;

    if(A7672E_init.status != WORK)//wait all GSM module initialization and start data sending after that (when main window is start showing up
        return;

    if(towers[switcher].state == PROCESS){// if data processed send request to next tower
        switcher = (switcher + 1) % TOWER_COUNT;
        towers[switcher].state = SEND;
    }
    else {//if tower state is SEND or WAIT
        Tower_COM_sequence(switcher);
    }
}