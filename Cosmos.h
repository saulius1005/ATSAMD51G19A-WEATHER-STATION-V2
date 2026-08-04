/*
 * Cosmos.h
 *
 * Created: 2024-12-01 15:37:27
 *  Author: Saulius
 */ 

#ifndef COSMOS_H_
#define COSMOS_H_

// Constants for converting between degrees and radians
#define DEG_TO_RAD 0.01745329251994329576923690768489 // pi / 180
#define RAD_TO_DEG 57.295779513082320876798154814105 // 180 / pi

////////////////////////////////////////////////////////////////////////////////
// Solar Position Parameters Structure
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Structure to hold the solar position parameters and datetime information.
 * 
 * This structure stores all necessary parameters to calculate the solar position, 
 * such as geographical location (latitude, longitude, altitude) and the date/time 
 * (year, month, day, hour, minute, second, milliseconds). It also includes the 
 * calculated solar position (elevation and azimuth).
 */
typedef struct {
    int32_t latitude;       /**< Latitude of the location (in degrees) */
    int32_t longitude;      /**< Longitude of the location (in degrees) */
    double elevation;      /**< Solar elevation angle (in degrees) */
    int32_t altitude;
    double refracted_elevation;
    double elevated_refracted_elevation;
    double azimuth;        /**< Solar azimuth angle (in degrees) */
} SolarPositionParameters;

// Declare the global solar position parameters object, which will hold the current solar position data
extern SolarPositionParameters solar_params;

bool is_daylight_saving_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour);
void calculate_solar_position();

#endif /* COSMOS_H_ */
