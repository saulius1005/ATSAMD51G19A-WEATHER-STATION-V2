/* 
 * File:   crc8.h
 * Author: Saulius
 *
 * Created on Sekmadienis, 2026, rugpjûtis 16, 11.12
 */

#ifndef CRC8_H
#define	CRC8_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t TowerCRC(uint8_t device_id, uint8_t* buf); //fill 8 bit buffer for crc
uint8_t crc8_cdma2000(uint8_t* buf, uint8_t i); //calculate crc
bool verify_crc8_cdma2000(uint8_t *data, uint8_t length, uint8_t crc); //checking if message calculated crc (calculate inside from *data array), also this function need to know total bytes in array and it check as received crc


#ifdef	__cplusplus
}
#endif

#endif	/* CRC8_H */

