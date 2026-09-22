#include "settings.h"

/*void RS485_write_str(char *str){
    uint8_t length = 0;
    while(str[length]) length++;//calculate how much bytes in total

    SERCOM2_REGS->USART_INT.SERCOM_LENGTH = SERCOM_USART_INT_LENGTH_LEN(length) | SERCOM_USART_INT_LENGTH_LENEN_Msk;//set length to usart hardware once
    while(SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_LENGTH_Msk);//wait sync
    
    const uint32_t* arr = (const uint32_t*) str; //create pointer of 32bit length
    
    length = (length+3)>>2;//how much it will be of 32bits
    SERCOM2_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_TXC_Msk;//clear last flag 
    while(length--){
        while(!(SERCOM2_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk));
        SERCOM2_REGS->USART_INT.SERCOM_DATA = *arr++;
    }
    while(!(SERCOM2_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk)); //stop bid set and shift register is empty and no new data  
    SERCOM2_REGS->USART_INT.SERCOM_LENGTH &= ~SERCOM_USART_INT_LENGTH_LENEN_Msk;   
}

void RS485_printf(const char *fmt, ...){
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    RS485_write_str(buffer); 
} */

void RS485_write_str(volatile sercom_registers_t *SERCOM, char *str){
    /*uint8_t length = 0;
    while(str[length]) length++;

    SERCOM->USART_INT.SERCOM_LENGTH = SERCOM_USART_INT_LENGTH_LEN(length) | SERCOM_USART_INT_LENGTH_LENEN_Msk;

    while(SERCOM->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_LENGTH_Msk);
    const uint32_t* arr = (const uint32_t*)str;

    length = (length + 3) >> 2;

    SERCOM->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_TXC_Msk;

    while(length--) {
        while(!(SERCOM->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk));
        SERCOM->USART_INT.SERCOM_DATA = *arr++;
    }
    while(!(SERCOM->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk));
    SERCOM->USART_INT.SERCOM_LENGTH &= ~SERCOM_USART_INT_LENGTH_LENEN_Msk;*/
    while (*str){
        while(!(SERCOM->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk)); //is DATA empty?
        SERCOM->USART_INT.SERCOM_DATA = *str++;
    }
    while (!(SERCOM->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk)); //is DATA and TXC shift register empty?   
    SERCOM->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_TXC_Msk;
}

void RS485_printf(volatile sercom_registers_t *SERCOM, const char *fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    RS485_write_str(SERCOM, buffer);
}