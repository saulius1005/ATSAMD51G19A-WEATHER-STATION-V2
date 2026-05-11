/* 
 * File:   refraction.h
 * Author: Saulius
 *
 * Created on Pirmadienis, 2026, geguþë 11, 22.57
 */

#ifndef REFRACTION_H
#define	REFRACTION_H

#ifdef	__cplusplus
extern "C" {
#endif

//#define R_EARTH 6371000.0 //simple
#define R_EARTH 6378137.0 //wgs-84

void apply_all_elevation_modifies();


#ifdef	__cplusplus
}
#endif

#endif	/* REFRACTION_H */

