#include "settings.h"
#include "refractionVar.h"


void correct_solar_angles() { //Refraction max ~ 0.5-0.6 degree
    double T_C = sensors.BME680.temperature;// temperature in °C

    double es = 6.1121f * expf((18.678f - T_C / 234.5f) * (T_C / (257.14f + T_C))); // saturation vapor pressure (hPa)
    double mw = (sensors.BME680.humidity / 100.0f) * es; // water vapor pressure (hPa)
    double T_K = T_C + 273.15f; // temperature in K (Kelvin)
    double rho_corr = (sensors.BME680.pressure / 1013.25f) * (273.15f / T_K); // air density correction factor(Pa to hPa) and / 1013.25
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

    if (A7672E_NET.altitude >= 0) {
        double dip_rad = acos(R_eff / (R_eff + A7672E_NET.altitude));
        solar_params.elevated_refracted_elevation = solar_params.refracted_elevation + (float)(dip_rad * RAD_TO_DEG);
    } else {
        double abs_h = fabs(A7672E_NET.altitude);
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
    
    /*Sky observation 2026-09-23 : 09-28
     * 
     * 09-23: very heavy clouds all day long from morning to evening average day power generation less than 500-700w/h 
     * that day power generation starts at 06:43:16 (Sun elevation: -4.52 degree) and ends 19:43:57 (Sun elevation: -4.14 degree) 
     * 
     * 09-24: first day part was like day before and after 12:35 clouds starts to thin out but they don't disappear. Power generation starts at 06:44:10 (-4.66) ends 19:52:52 (-5.97),
     * 
     * 09-25:09-26 heavy clouds with one or two short periods with the sun. Day and night about 12h each 09-25 more less
     * 
     * 09-27: first clear sky morning after full week of heavy clouds (last sunny day with clouds 50-50% was 09-19). Power generation starts 06:44:29 (84.87az -5.54el) and so much of clear sky until 9:12:15 ... sun come back at 15:07:22 and power generation ends at 19:52:40 (276.94 az -7.10el)
     * 
     * 09-28: sunny day ~70-80% of the day. Power generation starts 6:40:50 (84.54az -6.28el) ends at 19:46:32 (274.51az -5.87el)
     * 
     * all observation in the morning was made when towers is still in last day evening positions.
     */
}
