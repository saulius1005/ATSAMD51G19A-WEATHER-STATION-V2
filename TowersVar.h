/* 
 * File:   TowersVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 7, 18.43
 */

#ifndef TOWERSVAR_H
#define	TOWERSVAR_H

#include "Towers.h"


#ifdef	__cplusplus
extern "C" {
#endif

tower_t towers[256] = {/*{ //uncomment if needde to manualy declarate towers data :D nice joke to my self pfff
    .state = SEND,
    .id = 1,
    .position = {
        .azimuth   = 18022,
        .elevation = 5515
    },
    .panel = {
        .voltage = 2105,
        .current = 1122
    },
    .az_motor = {
        .voltage = 6003,
        .current = 278
    },
    .el_motor = {
        .voltage = 2430,
        .current = 432
    }},

    {
    .state = SEND,
    .id = 2,
    .position = {
        .azimuth   = 18103,
        .elevation = 5495
    },
    .panel = {
        .voltage = 21522,
        .current = 1266
    },
    .az_motor = {
        .voltage = 5988,
        .current = 10
    },
    .el_motor = {
        .voltage = 2401,
        .current = 0
    }}*/
};


#ifdef	__cplusplus
}
#endif

#endif	/* TOWERSVAR_H */

