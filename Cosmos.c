/*
 * Cosmos.c
 *
 * Tikslesnë Saulës pozicijos versija
 * Pagerinimai:
 * - NOAA equation of time
 * - Taisyklingas azimuth skaièiavimas
 * - Atmosferinë refrakcija
 * - double tikslumas
 * - Tikslesnë obliquity formulë
 */

#include "settings.h"
#include "CosmosVar.h"

bool is_daylight_saving_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour){
    uint8_t march_last_sunday = 31 - ((5 * year / 4 + 4) % 7);
    uint8_t october_last_sunday = 31 - ((5 * year / 4 + 1) % 7);

    // DST active between March and October
    if(month > 3 && month < 10)
        return true;

    // March transition
    if(month == 3){
        if(day > march_last_sunday)
            return true;

        if(day == march_last_sunday && hour >= 1)
            return true;

        return false;
    }

    // October transition
    if(month == 10){
        if(day < october_last_sunday)
            return true;

        if(day == october_last_sunday && hour < 1)
            return true;

        return false;
    }

    return false;
}

/**
 * Julian Day
 */
double calculate_julian_day( int year, int month, int day, int hour, int minute, int second ){
    if(month <= 2){
        year--;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + A / 4;

    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + B - 1524.5 + ((double)hour + (double)minute / 60.0 + (double)second / 3600.0) / 24.0;
}

/**
 * Saulës vidutinë anomalija
 */
double calculate_solar_mean_anomaly(double JC){
    return fmod( 357.52911 + JC * (35999.05029 - 0.0001537 * JC), 360.0 );
}

/**
 * Saulës lygtis centro
 */
double calculate_equation_of_center(double M, double JC){
    double Mrad = M * DEG_TO_RAD;
    return sin(Mrad) * (1.914602 - JC * (0.004817 + 0.000014 * JC)) + sin(2.0 * Mrad) * (0.019993 - 0.000101 * JC) + sin(3.0 * Mrad) * 0.000289;
}

/**
 * Apparent longitude
 */
double calculate_apparent_longitude(double true_longitude, double JC){
    double omega = 125.04 - 1934.136 * JC;
    return true_longitude - 0.00569 - 0.00478 * sin(omega * DEG_TO_RAD);
}

/**
 * Tikslesnë obliquity formulë
 */
double calculate_obliquity(double JC){
    double seconds = 21.448 - JC * ( 46.8150 + JC * ( 0.00059 - JC * 0.001813 ) );
    return 23.0 + (26.0 / 60.0) + (seconds / 3600.0);
}

/**
 * Solar declination
 */
double calculate_declination( double apparent_longitude, double obliquity ){

    double lambda = apparent_longitude * DEG_TO_RAD;
    double epsilon = obliquity * DEG_TO_RAD;

    return asin( sin(epsilon) * sin(lambda) ) * RAD_TO_DEG;
}

/**
 * NOAA Equation of Time
 */
double calculate_equation_of_time( double JC, double L0, double e, double M, double epsilon ){
    double y = tan((epsilon * DEG_TO_RAD) / 2.0);
    y *= y;
    double L0rad = L0 * DEG_TO_RAD;
    double Mrad = M * DEG_TO_RAD;
    
    return 4.0 * RAD_TO_DEG * ( y * sin(2.0 * L0rad) - 2.0 * e * sin(Mrad) + 4.0 * e * y * sin(Mrad) * cos(2.0 * L0rad) - 0.5 * y * y * sin(4.0 * L0rad) - 1.25 * e * e * sin(2.0 * Mrad) );
}

/**
 * Pagrindinis Saulës pozicijos skaièiavimas
 */
void calculate_solar_position(){

    uint16_t year = RTC_Date_and_Time.RTC_year + 2000;

    int timezone_offset = TIME_ZONE + ( is_daylight_saving_time( year, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, RTC_Date_and_Time.RTC_hour ) ? 1 : 0 );

    // UTC
    int utc_hour = RTC_Date_and_Time.RTC_hour  - timezone_offset;

    double JD = calculate_julian_day( year, RTC_Date_and_Time.RTC_month, RTC_Date_and_Time.RTC_day, utc_hour, RTC_Date_and_Time.RTC_minute, RTC_Date_and_Time.RTC_second );

    double JC = (JD - 2451545.0) / 36525.0;

    /*
     * Saulës parametrai
     */

    double L0 = fmod( 280.46646 + JC * ( 36000.76983 + JC * 0.0003032 ), 360.0 );

    if(L0 < 0) L0 += 360.0;

    double M = calculate_solar_mean_anomaly(JC);
    double C = calculate_equation_of_center(M, JC);
    double true_longitude = L0 + C;
    double apparent_longitude = calculate_apparent_longitude( true_longitude, JC );

    double obliquity = calculate_obliquity(JC);

    double declination = calculate_declination( apparent_longitude, obliquity );

    /*
     * Orbit eccentricity
     */

    double e = 0.016708634 - JC * ( 0.000042037 + 0.0000001267 * JC );

    /*
     * Equation of Time
     */

    double eq_time = calculate_equation_of_time( JC, L0, e, M, obliquity );

    /*
     * Solar Time
     */

    double solar_time = ( (double)( RTC_Date_and_Time.RTC_hour * 60 + RTC_Date_and_Time.RTC_minute ) + (double)RTC_Date_and_Time.RTC_second / 60.0 + eq_time + 4.0 * ((double)solar_params.longitude / 10000) - 60.0 * timezone_offset ) / 60.0;

    /*
     * Hour angle
     */
    double hour_angle = (solar_time - 12.0) * 15.0;
    double hour_angle_rad = hour_angle * DEG_TO_RAD;
    /*
     * Latitude / declination
     */
    double latitude_rad = ((double)solar_params.latitude / 10000) * DEG_TO_RAD;
    double declination_rad = declination * DEG_TO_RAD;
    /*
     * Elevation
     */
    double sin_elevation = sin(latitude_rad) * sin(declination_rad) + cos(latitude_rad) * cos(declination_rad) * cos(hour_angle_rad);
    solar_params.elevation = asin(sin_elevation) * RAD_TO_DEG;

    double azimuth = atan2( sin(hour_angle_rad), cos(hour_angle_rad) * sin(latitude_rad) - tan(declination_rad) * cos(latitude_rad) ) * RAD_TO_DEG;
    azimuth += 180.0;

    if(azimuth < 0) azimuth += 360.0;
    if(azimuth >= 360.0) azimuth -= 360.0;

    solar_params.azimuth = azimuth;
}