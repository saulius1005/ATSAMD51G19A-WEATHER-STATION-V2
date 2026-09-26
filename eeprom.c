#include "settings.h"
#include "eepromVar.h"

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

void EEPROM_Check(){ //check if it is firs mcu run
    A7672E_network_settings_t dummy;
    EEPROM_Read(0, &dummy, sizeof(dummy));
    if (dummy.etester == EEEPROM_TEST_FIRST_VALUE){ //if eeprom value is valid update all data
        A7672E_NET = dummy; //update APN, its password and server url
    }
    else{
        EEPROM_Write(0, &A7672E_NET, sizeof(A7672E_NET)); //if not write default values
    }
 
}
