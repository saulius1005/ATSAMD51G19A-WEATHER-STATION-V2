/* 
 * File:   eeprom.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 28, 21.25
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SMEEPROM_SIZE 512
#define SMEEPROM_ADDR 0x44000000UL //smarteeprom starts at 0x44000000 and ends at 0x45000000 //DO NOT CHANGE unless....
#define EEEPROM_TEST_FIRST_VALUE 0 //for eeprom test if value is not like this, meaning it is first eeprom run and need to fill eeprom with default values
    
//config bits for Smart EEPROM
    
extern volatile uint8_t *SmartEEPROM8;
extern volatile uint16_t *SmartEEPROM16;
extern volatile uint32_t *SmartEEPROM32;

/*
 * exmpl:
 * SmartEEPROM8[0] = 123;
 * SmartEEPROM8[1] = 456;
 * 
 * SmartEEPROM16[0] = 12345;
 * SmartEEPROM16[2] = 23456;
 * 
 * SmartEEPROM32[0] = 7000000;
 * SmartEEPROM32[4] = 123000000;
 */

void EEPROM_Write8(uint32_t address, uint8_t data);//8bit eeprom write

uint8_t EEPROM_Read8(uint32_t address); //8bit read

void EEPROM_Write16(uint32_t address, uint16_t data); //16bit write

uint16_t EEPROM_Read16(uint32_t address); //16bir read

void EEPROM_Write32(uint32_t address, uint32_t data);//32bit write

uint32_t EEPROM_Read32(uint32_t address);//32bit read

/*
 * exmpl:
 * EEPROM_Write8(0, 123);//write at 0 address value of 123
 * uint8_t value = EEPROM_Read8(0);//read
 * 
 */
void EEPROM_Write(uint32_t address, const void *data, uint32_t size);//universal write
/*
 * 
 * uint8_t  a = 123;
 * uint16_t b = 12345;
 * uint32_t c = 123456789;
 * 
 * EEPROM_Write(0,   &a, sizeof(a));
 * EEPROM_Write(1,   &b, sizeof(b));
 * EEPROM_Write(3,   &c, sizeof(c));
 * 
 * EEPROM_Write(0, &config, sizeof(config)); //write whole struct
 * exmp: EEPROM_Write(0, &A7672E_NET, sizeof(A7672E_NET));
 */

void EEPROM_Read(uint32_t address, void *data, uint32_t size);//universal read

/*
 * EEPROM_Read(0, &config, sizeof(config)); //read whole struct
 * exmp: EEPROM_Read(0, &A7672E_NET, sizeof(A7672E_NET));
 */

void EEPROM_Check(); //check if it is firs mcu run


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

