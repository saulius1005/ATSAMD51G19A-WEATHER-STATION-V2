#include "settings.h"
#include "refractionVar.h"


void correct_solar_angles() { //Refraction max ~ 0.5-0.6 degree
    double T_C = BME680.temperature / 100.0f;// temperature in °C

    double es = 6.1121f * expf((18.678f - T_C / 234.5f) * (T_C / (257.14f + T_C))); // saturation vapor pressure (hPa)
    double mw = (BME680.humidity / 100000.0f) * es; // water vapor pressure (hPa) same as /1000 (to get RH in %) / 100 
    double T_K = T_C + 273.15f; // temperature in K (Kelvin)
    double rho_corr = (BME680.pressure / 101325.00f) * (273.15f / T_K); // air density correction factor same as /100 (Pa to hPa) and / 1013.25
    double humidity_dip = (mw / 1013.25f) * (11.27f / T_K);// humidity correction term
    double total_corr = rho_corr - humidity_dip; // combined atmospheric correction

    double angle = (solar_params.elevation + (7.31f / (solar_params.elevation + 4.4f))) * DEG_TO_RAD;// refraction angle (Bennett model)
    double t = tanf(angle); // tangent of angle

    if (fabsf(t) < 1e-6f) t = 1e-6f; // avoid singularity
    double cot_h = 1.0f / t;  // cotangent term

    double refraction_min = cot_h * total_corr * 1.02f; // refraction in arcminutes                             
    solar_params.refracted_elevation = (refraction_min / 60.0f) + solar_params.elevation; // convert refraction to degrees and add to elevation
}

void apply_altitude_dip(){ //it depends  :D
    double R_eff = R_EARTH / (1.0 - 0.16); //0.16 is k and it can be more precize but i need 2 more temperature sensors for so little accurate result (maybe in future)

    if (solar_params.altitude >= 0) {
        double dip_rad = acos(R_eff / (R_eff + solar_params.altitude));
        solar_params.elevated_refracted_elevation = solar_params.refracted_elevation + (float)(dip_rad * RAD_TO_DEG);
    } else {
        double abs_h = fabs(solar_params.altitude);
        if (abs_h > R_eff) abs_h = R_eff * 0.99; 
        double dip_rad = acos(R_eff / (R_eff + abs_h));
        solar_params.elevated_refracted_elevation = solar_params.refracted_elevation - (float)(dip_rad * RAD_TO_DEG);
    }
    
}

void apply_all_elevation_modifies(){
    correct_solar_angles();
    apply_altitude_dip();
    solar_params.coarse_elevation = (int16_t)(solar_params.elevated_refracted_elevation * 100);
    
    //start Sun tracking when elevation is -8 -4 degree below horizon
    //2026-05-12 power generation starts 4:40 (some clouds- average day) sun at the moment was -5.14 (elevation)
    //2026-05-11 power generation starts 4:20 (clear day- zero clouds) sun at the moment was -7.49
    //2026-05-10 power generation starts 4:50 (heavy clouds 24h no sun at all) sun at the moment was -4.47
    // I think i need calculate aproximitly if are clouds and how much and thes set start angle in range of -8 -4 degree maybe with this?.....:
    
    /*float start_angle = -5.5f;

    // humidity
    start_angle -= (70.0f - RH) * 0.025f;

    // pressure
    start_angle -= (pressure - 1010.0f) * 0.01f;

    // temperature
    start_angle -= (-5.0f - temperature) * 0.015f;

    // limits
    if (start_angle < -8.0f)
        start_angle = -8.0f;

    if (start_angle > -4.0f)
        start_angle = -4.0f;*/
}
