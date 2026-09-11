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
};

param_limit_t limits[] = { //location limits for latitude, logitude and altitude
    {900000,  &A7672E_NET.latitude},
    {1800000, &A7672E_NET.longitude},
    {9999,    &A7672E_NET.altitude}
};

param_text_t location_param_names[] = {
    {"Latitude:", 60},
    {"Longitude:", 66},
    {"Altitude:", 60}
};

char APN_name[APN_ADD_SYMBOLS_COUNT];
char APN_pass[TRUSTED_PHONE_SYMBOLS_COUNT];
char Server_url[SERVER_URL_COUNT];

GSM_NET_param_t win_params[] = {
    {
        APN_name,
        A7672E_NET.APN_USR,
        offsetof(A7672E_network_settings_t, APN_USR),
        sizeof(APN_name),
        "APN Name"
    },

    {
        APN_pass,
        A7672E_NET.TRST_PHN,
        offsetof(A7672E_network_settings_t, TRST_PHN),
        sizeof(APN_pass),
        "Trusted Phone Number"
    },

    {
        Server_url,
        A7672E_NET.SERVER_URL,
        offsetof(A7672E_network_settings_t, SERVER_URL),
        sizeof(Server_url),
        "Server URL"
    }
};

#ifdef	__cplusplus
}
#endif

#endif	/* WINDOWSVAR_H */

