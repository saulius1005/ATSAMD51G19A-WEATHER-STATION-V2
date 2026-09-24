#include "settings.h"

void USART_RS485_write_str(volatile sercom_registers_t *SERCOM, char *str, uint8_t length){  
    SERCOM->USART_INT.SERCOM_LENGTH = SERCOM_USART_INT_LENGTH_LEN(length) | SERCOM_USART_INT_LENGTH_LENEN_Msk;
    while(SERCOM->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_LENGTH_Msk);
    
    while (*str){
        while(!(SERCOM->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk)); //is DATA empty?
        SERCOM->USART_INT.SERCOM_DATA = *str++;
    }
    while (!(SERCOM->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk)); //is DATA and TXC shift register empty?   
    SERCOM->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_TXC_Msk;
}

void USART_RS485_printf(volatile sercom_registers_t *SERCOM, const char *fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    USART_RS485_write_str(SERCOM, buffer, (uint8_t)sizeof(buffer));
}