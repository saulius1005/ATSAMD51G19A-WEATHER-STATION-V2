/* 
 * File:   BME680Var.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, balandis 17, 15.41
 */

#ifndef BME680VAR_H
#define	BME680VAR_H

#include "BME680.h"


#ifdef	__cplusplus
extern "C" {
#endif

BME680_t BME680 = {
    .ID = 0, 
    .STATUS_spi_mem_page = 0,
    .RESET = false,
    .Config = {
        .filter = Filter_coef_0,
        .spi_3w_en = false,
    },
    .Ctrl_meas = {
        .osrs_t = oversampling_SKIP,
        .osrs_p = oversampling_SKIP,
        .mode = sleep_mode,
    },
    .Ctrl_hum = {
        .osrs_h = oversampling_SKIP,
        .spi_3w_init_en = false,
    },
    .Ctrl_gas = {
        .nb_conv = heater_set_point_0,
        .heat_off = false,
        .run_gas = false,
    },
    .Gas_wait_x = {0},
    .Res_heat_x = {0},
    .Idac_heat_x = {0},
    .gas_r = {
        .gas_r_90 = 0,
        .gas_range_r = 0,
        .gas_valid_r = 0,
        .heat_stab_r = 0,
    },
    .gas_r_recived = false,
    
};


#ifdef	__cplusplus
}
#endif

#endif	/* BME680VAR_H */

