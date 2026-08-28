/* 
 * File:   MCU_configs.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 28, 23.07
 */

#ifndef MCU_CONFIGS_H
#define	MCU_CONFIGS_H

#ifdef	__cplusplus
extern "C" {
#endif

// ATSAMD51G19A Configuration Bit Settings

// 'C' source line config statements

// Config Source code for XC32 compiler.
// USER_WORD_0
#pragma config BOD33_DIS = SET
#pragma config BOD33USERLEVEL = 0x1C // Enter Hexadecimal value
#pragma config BOD33_ACTION = RESET
#pragma config BOD33_HYST = 0x2 // Enter Hexadecimal value
#pragma config NVMCTRL_BOOTPROT = 0

// USER_WORD_1
#pragma config NVMCTRL_SEESBLK = 0x1 //1- 2 Blocks of SmartEEPROM
#pragma config NVMCTRL_SEEPSZ = 0x0 //0- 4 Bytes page size, 512 Bytes SmartEEPROM
#pragma config RAMECC_ECCDIS = SET
#pragma config WDT_ENABLE = CLEAR
#pragma config WDT_ALWAYSON = CLEAR
#pragma config WDT_PER = CYC16384
#pragma config WDT_WINDOW = CYC16384
#pragma config WDT_EWOFFSET = CYC16384
#pragma config WDT_WEN = CLEAR

// USER_WORD_2
#pragma config NVMCTRL_REGION_LOCKS = 0xFFFFFFFF // Enter Hexadecimal value

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#ifdef	__cplusplus
}
#endif

#endif	/* MCU_CONFIGS_H */

