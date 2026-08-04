/* 
 * File:   windowsVar.h
 * Author: Saulius
 *
 * Created on Treèiadienis, 2026, balandis 29, 21.09
 */

#ifndef WINDOWSVAR_H
#define	WINDOWSVAR_H

#include "windows.h"
#include "keyboard.h"


#ifdef	__cplusplus
extern "C" {
#endif

    Windows_t Windows = {
        .Window = INIT_WINDOW,
        .background_updater = false,
        .once_per_second_update = 0,
        .keyboardAction = CLOSE, //set keyboard closed
    };
    
    param_limit_t limits[] = { //location limits for latitude, logitude and altitude
        {900000,  &solar_params.latitude},
        {1800000, &solar_params.longitude},
        {9999,    &solar_params.altitude}
    };
    
    param_text_t location_param_names[] = {
        {"Latitude:", 60},
        {"Longitude:", 66},
        {"Altitude:", 60}
    };

#ifdef	__cplusplus
}
#endif

#endif	/* WINDOWSVAR_H */

