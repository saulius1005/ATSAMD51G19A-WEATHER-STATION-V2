/* 
 * File:   eepromVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 28, 22.23
 */

#ifndef EEPROMVAR_H
#define	EEPROMVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

volatile uint8_t *SmartEEPROM8 = (uint8_t *)SMEEPROM_ADDR;
volatile uint16_t *SmartEEPROM16 = (uint16_t *)SMEEPROM_ADDR;
volatile uint32_t *SmartEEPROM32 = (uint32_t *)SMEEPROM_ADDR;


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROMVAR_H */

