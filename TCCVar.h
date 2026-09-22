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
        .update_time = 1000,//1s
        .respond_time = 50,//50ms
        .update_stat = UPDATED,
        .start_at = 0,
        },
    .TIME = {
        .update_time = 500,//500ms
        .respond_time = 50,//50ms
        .update_stat = UPDATED,
        .start_at = 0,
        },
    .SERVER = {
        .update_time = 60000,//60s
        .respond_time = 20000,//20s
        .update_stat = UPDATED,
        .start_at = 0,
    },
    .TOWERS = {
        .update_time = 1000,
        .respond_time = 250,
        .update_stat = UPDATED,
        .start_at = 0,
    },
    .SENSORS = {
        .update_time = 1000,
        .respond_time = 300,
        .update_stat = UPDATED,
        .start_at = 300,
    },
};


#ifdef	__cplusplus
}
#endif

#endif	/* TCCVAR_H */

