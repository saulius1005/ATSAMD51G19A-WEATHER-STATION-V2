/* 
 * File:   TC.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, kovas 6, 22.47
 */

#ifndef TC_H
#define	TC_H

#ifdef	__cplusplus
extern "C" {
#endif

    
extern volatile bool TC0_timeout;

void GCLK3_SERCOM_TC_core_init();

void TC0_init(); //Initialization of TC0

void TC0_ON(uint32_t period_us); //TC0 timeout setting up function

void TC0_OFF(); // TC0 off function

void TC0_CHECKER(); //check if is timeout if yes TS0_timeout = true

#ifdef	__cplusplus
}
#endif

#endif	/* TC_H */

