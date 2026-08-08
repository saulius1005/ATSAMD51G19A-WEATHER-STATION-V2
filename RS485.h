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


void RS485_printf(const char *fmt, ...);

#ifdef	__cplusplus
}
#endif

#endif	/* RS485_H */

