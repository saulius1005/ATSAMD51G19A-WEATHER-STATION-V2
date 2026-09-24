/* 
 * File:   RS485.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 7, 21.16
 */

#ifndef RS485_H
#define	RS485_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define RS485_RX_BUFFER_SIZE 128
    
#define RS485_TOWER_REG    SERCOM2_REGS
#define RS485_SENSORS_REG  SERCOM5_REGS

//void RS485_printf(const char *fmt, ...);
void USART_RS485_printf(volatile sercom_registers_t *SERCOM, const char *fmt, ...);

#ifdef	__cplusplus
}
#endif

#endif	/* RS485_H */

