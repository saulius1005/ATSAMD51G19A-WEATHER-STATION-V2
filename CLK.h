/* 
 * File:   CLK.h
 * Author: Saulius
 *
 * Created on September 25, 2026, 1:19 AM
 */

#ifndef CLK_H
#define	CLK_H

#ifdef	__cplusplus
extern "C" {
#endif

void cpu_120Mhz_DPLL0_XOSC1_init();// Configure CPU clock to ~128 MHz using DPLL0 with XOSC1 reference


#ifdef	__cplusplus
}
#endif

#endif	/* CLK_H */

