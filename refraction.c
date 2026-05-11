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

    if (solar_params.altitude >= 0.0) {
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
}

/* //Tester code
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define DEG_TO_RAD 0.017453292519943295
#define RAD_TO_DEG 57.29577951308232

struct {
    float elevation;
} solar_params;

struct {
    int32_t pressure;   // Pa (Paskaliai, kaip ið BME680)
    int32_t temperature;// °C * 100
    int32_t humidity;   // % * 1000
} BME680;

float calculate_refraction(){
    
    float h = solar_params.elevation;                          // solar elevation (degrees)
    float T_C = BME680.temperature / 100.0f;                   // temperature in °C
    float P_hPa = BME680.pressure / 100.0f;                    // pressure in hPa
    float RH = BME680.humidity / 1000.0f;                     // relative humidity (0–100%)

    float es = 6.1121f * expf((18.678f - T_C / 234.5f) * (T_C / (257.14f + T_C)));       // saturation vapor pressure (hPa)
    float mw = (BME680.humidity / 100000.0f) * es;                             // water vapor pressure (hPa)
    float T_K = T_C + 273.15f;                                 // temperature in Kelvin
    float rho_corr = (P_hPa / 1013.25f) * (273.15f / T_K);     // air density correction factor
    float humidity_dip = (mw / 1013.25f) * (11.27f / T_K);     // humidity correction term
    float total_corr = rho_corr - humidity_dip;                // combined atmospheric correction

    float angle = (h + (7.31f / (h + 4.4f))) * DEG_TO_RAD;     // refraction angle (Bennett model)
    float t = tanf(angle);                                      // tangent of angle

    if (fabsf(t) < 1e-6f) t = 1e-6f;                           // avoid singularity
    float cot_h = 1.0f / t;                                     // cotangent term

    float refraction_min = cot_h * total_corr * 1.02f;         // refraction in arcminutes
    return refraction_min / 60.0f;                             // convert to degrees
}

int main()
{
    // Testo duomenys
    solar_params.elevation = -16.00f; 
    BME680.pressure = 99400;      // 1 atm (Pa)
    BME680.temperature = 2800;     // 15°C
    BME680.humidity = 56000;       // Testui padidinkime iki 85%, kad matytøsi didesnis skirtumas

    //refrakcijà su nustatyta drëgme
    float refr_with_humidity = calculate_refraction();

    //refrakcijà "sausam" orui (RH = 0)
    int32_t original_rh = BME680.humidity;
    BME680.humidity = 0;
    float refr_dry_air = calculate_refraction();
    BME680.humidity = original_rh; // Gràþiname atgal

    printf("--- Aplinkos sàlygos ---\n");
    printf("Aukðtis virð horizonto: %.2f°\n", solar_params.elevation);
    printf("Temperatûra: %.2f°C, Slëgis: %.2f hPa, RH: %.1f%%\n\n", 
            BME680.temperature/100.0f, BME680.pressure/100.0f, BME680.humidity/1000.0f);

    printf("--- Rezultatai ---\n");
    printf("Refrakcija (su RH):      %.6f°\n", refr_with_humidity);
    printf("Refrakcija (sausas oras): %.6f°\n", refr_dry_air);

    float diff_deg = refr_dry_air - refr_with_humidity;
    printf("RH átaka (skirtumas):    %.6f° (%.2f lanko sek.)\n", 
            diff_deg, diff_deg * 3600.0f);
    
    printf("\nMatomas aukðtis:         %.4f°\n", solar_params.elevation + refr_with_humidity);

    return 0;
}*/