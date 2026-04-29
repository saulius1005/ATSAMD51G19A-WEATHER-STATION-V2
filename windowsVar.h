/* 
 * File:   windowsVar.h
 * Author: Saulius
 *
 * Created on Treèiadienis, 2026, balandis 29, 21.09
 */

#ifndef WINDOWSVAR_H
#define	WINDOWSVAR_H

#include "windows.h"


#ifdef	__cplusplus
extern "C" {
#endif

    Windows_t Windows = {
        .Window = INIT_WINDOW,
        .background_updater = false,
        .once_per_second_update = 0,
    };



#ifdef	__cplusplus
}
#endif

#endif	/* WINDOWSVAR_H */

