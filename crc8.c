#include "settings.h"
#include "crc8Var.h"

uint8_t TowerCRC(uint8_t device_id, uint8_t* buf){
	uint8_t i = 0;
		buf[i++] =  device_id; //ID (FF)
		buf[i++] = (uint8_t)(solar_params.coarse_azimuth >> 8); //azimuth (FFFF)
		buf[i++] = (uint8_t)(solar_params.coarse_azimuth & 0xFF);
		buf[i++] = (uint8_t)(solar_params.coarse_elevation >> 8);//elevation (FFFF)
		buf[i++] = (uint8_t)(solar_params.coarse_elevation & 0xFF);
		buf[i++] = (uint8_t) sensors.WIND.speed; // wind speed (FF)
        uint16_t saveOneBit = ((sensors.WIND.direction & 0x07) << 12) | (sensors.SUN.level & 0x0FFF); //wind direction (F)(values only 0- 7) + light level(FFF)(values only 0-4095) = (F+FFF)
		buf[i++] = (uint8_t)(saveOneBit >> 8); //split wind direction and part of light level data
		buf[i++] = (uint8_t)(saveOneBit & 0xFF); // left part of light level
	return i;
}

uint8_t crc8_cdma2000(uint8_t* buf, uint8_t i){
	uint8_t crc = 0xFF;
	for (uint8_t j = 0; j < i; j++)
	crc = crc8_table[crc ^ buf[j]];

	return crc;
}

bool verify_crc8_cdma2000(uint8_t *data, uint8_t length, uint8_t crc) {
	uint8_t calculatedcrc = 0xFF;
	for (size_t i = 0; i < length; i++) { // length = 8 baitai
		calculatedcrc = crc8_table[calculatedcrc ^ data[i]];
	}
    
	return calculatedcrc == crc;

}
