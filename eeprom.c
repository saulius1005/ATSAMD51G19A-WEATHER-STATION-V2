#include "settings.h"
#include "eepromVar.h"

void EEPROM_Write8(uint32_t address, uint8_t data){//8bit eeprom write
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    if(!(NVMCTRL_REGS->NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_LOCK_Msk)){
        SmartEEPROM8[address] = data;
    }
}

uint8_t EEPROM_Read8(uint32_t address){ //8bit read
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    return SmartEEPROM8[address];
}

void EEPROM_Write16(uint32_t address, uint16_t data){ //16bit write
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    if(!(NVMCTRL_REGS->NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_LOCK_Msk)){
        SmartEEPROM16[address] = data;
    }
}

uint16_t EEPROM_Read16(uint32_t address) { //16bir read
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    return SmartEEPROM16[address];
}


void EEPROM_Write32(uint32_t address, uint32_t data){//32bit write
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    if(!(NVMCTRL_REGS->NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_LOCK_Msk)){
        SmartEEPROM32[address] = data;
    }
}

uint32_t EEPROM_Read32(uint32_t address) { //32bit read
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    return SmartEEPROM32[address];
}

void EEPROM_Write(uint32_t address, const void *data, uint32_t size){ //universal write
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    if(NVMCTRL_REGS->NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_LOCK_Msk)
        return;
    volatile uint8_t *eeprom = (volatile uint8_t *)SMEEPROM_ADDR;
    for (uint32_t i = 0; i < size; i++){
        eeprom[address + i] = ((const uint8_t *)data)[i];
    }
}

void EEPROM_Read(uint32_t address, void *data, uint32_t size){ //universal read
    while (NVMCTRL_REGS-> NVMCTRL_SEESTAT & NVMCTRL_SEESTAT_BUSY_Msk);
    volatile uint8_t *eeprom = (volatile uint8_t *)SMEEPROM_ADDR;
    for (uint32_t i = 0; i < size; i++){
        ((uint8_t *)data)[i] = eeprom[address + i];
    }
}
