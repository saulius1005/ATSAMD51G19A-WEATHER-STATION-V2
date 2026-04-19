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
};


#ifdef	__cplusplus
}
#endif

#endif	/* BME680VAR_H */

