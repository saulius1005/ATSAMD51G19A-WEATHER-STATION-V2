/* 
 * File:   XPT2046Var.h
 * Author: Saulius
 *
 * Created on Antradienis, 2026, vasaris 10, 18.38
 */

#ifndef XPT2046VAR_H
#define	XPT2046VAR_H

#ifdef	__cplusplus
extern "C" {
#endif

TuchScreen Read_XPT2046 = {
    .X = 0,
    .Y = 0,
    .Z1 = 0,
    .Z2 = 0,
    .step = 0,
    .state = SET
};


#ifdef	__cplusplus
}
#endif

#endif	/* XPT2046VAR_H */

