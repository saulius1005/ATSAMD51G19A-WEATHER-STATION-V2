/* 
 * File:   BME680.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, balandis 17, 15.40
 */

#ifndef BME680_H
#define	BME680_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define BME680_CS_LOW()    (PORT_REGS->GROUP[1].PORT_OUTCLR = PORT_PB10) // CS LOW
#define BME680_CS_HIGH()   (PORT_REGS->GROUP[1].PORT_OUTSET = PORT_PB10) // CS HIGH
    
typedef enum {
    sleep_mode = 0,
    forced_mode,
}BME680_mode_t;
    
typedef enum {
    oversampling_SKIP = 0,
    oversampling_x1, //1
    oversampling_x2, //2
    oversampling_x4, //3
    oversampling_x8, //4
    oversampling_x16,// 5 and others (here only 5)
}BME680_meas_os_t;

typedef enum {
    Filter_coef_0 = 0,
    Filter_coef_1, //1
    Filter_coef_3, //2
    Filter_coef_7, //3
    Filter_coef_15, //4
    Filter_coef_31, //5 
    Filter_coef_63, //6
    Filter_coef_127, //7
}BME680_filter_t;

typedef enum {
    multiplication_factor_x1 = 0,
    multiplication_factor_x4,
    multiplication_factor_x16,
    multiplication_factor_x64,
}BME680_gas_wait_t;

typedef enum {
    heater_set_point_0 = 0,
    heater_set_point_1,    
    heater_set_point_2, 
    heater_set_point_3, 
    heater_set_point_4, 
    heater_set_point_5, 
    heater_set_point_6, 
    heater_set_point_7, 
    heater_set_point_8,            
    heater_set_point_9,       
}BME680_nb_conv_t;
    

typedef struct {
    uint8_t filter; //IIR filter settings
    bool spi_3w_en; //enable SPI 3 wire mode
} BME680_Confif_t;

typedef struct {
    uint8_t osrs_t; //temperature oversampling
    uint8_t osrs_p; //presure oversampling
    uint8_t mode; //sleep or forced mode
} BME680_Ctrl_meas_t;
     
typedef struct {
    bool spi_3w_init_en; //interrupt enable for new data
    uint8_t osrs_h; //humid oversampling
} BME680_Ctrl_hum_t;

typedef struct {
    bool run_gas;
    uint8_t nb_conv;
    bool heat_off;
} BME680_Ctrl_gas_t;

typedef struct {
    uint8_t gas_r_10;
    bool gas_valid_r;
    uint8_t heat_stab_r;
    uint8_t gas_range_r;
    uint8_t gas_r_92;  
} BME680_gas_r_t;

typedef struct {
    bool new_data_0;
    bool gas_measuring;
    bool measuring;
    uint8_t gas_maes_index_0;    
} BME680_eas_status_0_t;

typedef struct {
    int32_t  par_t1;
    int32_t  par_t2;
    int32_t  par_t3;

    int32_t  par_p1;
    int32_t  par_p2;
    int32_t  par_p3;
    int32_t  par_p4;
    int32_t  par_p5;
    int32_t  par_p6;
    int32_t  par_p7;
    int32_t  par_p8;
    int32_t  par_p9;
    int32_t  par_p10;

    int32_t  par_h1;
    int32_t  par_h2;
    int32_t  par_h3;
    int32_t  par_h4;
    int32_t  par_h5;
    int32_t  par_h6;
    int32_t  par_h7;

    int32_t  par_g1;
    int32_t  par_g2;
    int32_t  par_g3;
} BME680_CalibData_t;
       
typedef struct {
    bool STATUS_spi_mem_page; //spi page selection 0- page: 0x80-0xff, 1- page: 0x00-0x7F
    uint8_t RESET; //resets device //default value is 0x00 and writing to it 0xB6 gives same effect as power-on reset 
    uint8_t ID; //device id

    BME680_Confif_t Config;
    BME680_Ctrl_meas_t Ctrl_meas; 
    BME680_Ctrl_hum_t Ctrl_hum;
    BME680_Ctrl_gas_t Ctrl_gas;
    
    uint8_t Gas_wait_x;
    uint8_t Res_heat_x; //target of heater resistance
    uint8_t Idac_heat_x; //particular heater set point
    
    BME680_gas_r_t gas_r;
    
    uint16_t hum;
    uint32_t temp;
    uint32_t pres;
    
    BME680_eas_status_0_t eas_status_0;
} BME680_t;


#ifdef	__cplusplus
}
#endif

#endif	/* BME680_H */

