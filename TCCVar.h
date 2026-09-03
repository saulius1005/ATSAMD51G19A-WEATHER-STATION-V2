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

regular_update_t Periodic_Checker = {
    .period_counter = 0,
    .GSM_GNSS_update = GSM_GNSS_UPDATE_PERIOD,
    .GSM_GNSS_update_flag = false,
    .GSM_update = GSM_UPDATE_PERIOD, //up to 4min15s
    .GSM_update_flag = false,
    .TOWERS_update = TOWERS_UPDATE_PERIOD, //up to 4min15s
    .TOWERS_update_flag = false,
    .SERVER_update = SERVER_UPDATE_PERIOD, //up to 18h12min16sec
    .SERVER_update_flag = false,
};

regular_update_devices_t Periodic_Checker_Devices = {
    .GSM = {
        .update_time = 1000,
        .respond_time = 10,
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
        .update_time = 5000,
        .respond_time = 1000,
        .update_stat = UPDATED,
        .start_at = 0,
    },
};


#ifdef	__cplusplus
}
#endif

#endif	/* TCCVAR_H */

