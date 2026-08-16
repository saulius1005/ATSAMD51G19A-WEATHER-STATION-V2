#include "settings.h"
#include "crc8Var.h"

uint8_t TowerCRC(uint8_t device_id, uint8_t* buf){
	uint8_t i = 0;
		buf[i++] =  device_id; //ID (FF)
		buf[i++] = (uint8_t)(solar_params.coarse_azimuth >> 8); //azimuth (FFFF)
		buf[i++] = (uint8_t)(solar_params.coarse_azimuth & 0xFF);
		buf[i++] = (uint8_t)(solar_params.coarse_elevation >> 8);//elevation (FFFF)
		buf[i++] = (uint8_t)(solar_params.coarse_elevation & 0xFF);
		buf[i++] = (uint8_t) WIND.speed; // wind speed (FF)
		buf[i++] = (uint8_t)WIND.direction; //split wind direction and part of light level data
        //removed light level value DO NOT FORGET ABOUT THAT!!!!
	return i;
}

uint8_t crc8_cdma2000(uint8_t* buf, uint8_t i){
	uint8_t crc = 0xFF;
	for (uint8_t j = 0; j < i; j++)
	crc = crc8_table2[crc ^ buf[j]];

	return crc;
}
