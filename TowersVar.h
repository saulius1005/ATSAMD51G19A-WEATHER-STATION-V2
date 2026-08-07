/* 
 * File:   TowersVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugpjûtis 7, 18.43
 */

#ifndef TOWERSVAR_H
#define	TOWERSVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

tower_t towers[TOWER_COUNT] = {{
        .id = 1,
        .position = {
            .azimuth   = 0,
            .elevation = 0
        },
        .panel = {
            .voltage = 0,
            .current = 0
        },
        .az_motor = {
            .voltage = 0,
            .current = 0
        },
        .el_motor = {
            .voltage = 0,
            .current = 0
        }},

    {
        .id = 2,
        .position = {
            .azimuth   = 0,
            .elevation = 0
        },
        .panel = {
            .voltage = 0,
            .current = 0
        },
        .az_motor = {
            .voltage = 0,
            .current = 0
        },
        .el_motor = {
            .voltage = 0,
            .current = 0
        }}
};


#ifdef	__cplusplus
}
#endif

#endif	/* TOWERSVAR_H */

