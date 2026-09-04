/* 
 * File:   TCCVar.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, rugpjûtis 3, 09.14
 */

#ifndef TCCVAR_H
#define	TCCVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

volatile bool TCC0_timeout = false;
volatile bool TCC1_timeout = false;

regular_update_devices_t Periodic_Checker_Devices = {
    .period_counter = 0,
    .GSM = {
        .update_time = 2000,
        .respond_time = 250,
        .update_stat = UPDATED,
        .start_at = 0,
        },
    .TIME = {
        .update_time = 500,
        .respond_time = 20,
        .update_stat = UPDATED,
        .start_at = 0,
        },
    .SERVER = {
        .update_time = 8000,
        .respond_time = 2000,
        .update_stat = UPDATED,
        .start_at = 0,
    },
    .TOWERS = {
        .update_time = 5000 / TOWER_COUNT,
        .respond_time = 500,
        .update_stat = UPDATED,
        .start_at = 0,
    },
};


#ifdef	__cplusplus
}
#endif

#endif	/* TCCVAR_H */

