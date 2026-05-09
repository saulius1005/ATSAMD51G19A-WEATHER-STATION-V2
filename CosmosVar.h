/*
 * CosmosVar.h
 *
 * Created: 2024-12-01 15:37:52
 *  Author: Saulius
 */ 

#ifndef COSMOSVAR_H_
#define COSMOSVAR_H_

// Declare and initialize the solar position parameters for the specified location and time
SolarPositionParameters solar_params = {
	.latitude = 45.9087,        /**< Latitude of the location (in degrees) */ //4 digits after . means: +-110m
	.longitude = 20.2883,       /**< Longitude of the location (in degrees) */
	
	// Pre-calculated solar elevation and azimuth for the given location and time
	.elevation = 37.3,            /**< Average annual elevation for the selected coordinates (in degrees) */
	
	// Azimuth is calculated from South (180°) with an offset towards the East
	.azimuth = 171.4              /**< Azimuth direction (180° = South, 171.4° is 8.6° East of South) */
};

#endif /* COSMOSVAR_H_ */
