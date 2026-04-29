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
extern volatile bool TC2_timeout;

void TC0_init(); //Initialization of TC0

void TC0_ON(uint32_t period_us); //TC0 timeout setting up function

void TC0_OFF(); // TC0 off function

void TC0_CHECKER(); //check if is timeout if yes TS0_timeout = true

void TC2_init(); //Initialization of TC2

void TC2_ON(uint32_t period_us);//TC2 timeout setting up function

void TC2_OFF();//TC2 off function

void TC2_CHECKER();//check if is timeout if yes TS2_timeout = true

#ifdef	__cplusplus
}
#endif

#endif	/* TC_H */

