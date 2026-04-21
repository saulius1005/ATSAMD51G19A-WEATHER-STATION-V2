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
    
#define status_ADD 0x73
#define Reset_ADD 0x60
#define ID_ADD 0x50
#define Config_ADD 0x75
#define Ctrl_meas_ADD 0x74
#define Ctrl_hum_ADD 0x72
#define Ctrl_gas_ADD 0x71 //0x70- ctrl_gas0  //16bit
#define Gas_wait_x_ADD 0x6D //0x64-0x6D //10 bytes?
#define Res_heat_x_ADD 0x63 //0x5A-0x63 //10 bytes?
#define Idac_heat_x_ADD 0x59 //0x50-0x59 //10 bytes?
#define Gas_r_msb_ADD 0x2A //0x2B - lsb //16bit
#define hum_msb_ADD 0x25 //0x26- lsb //16bit
#define temp_msb_ADD 0x22 //(7:4), 0x23- lsb, 0x24- xlsb //24bit
#define pres_msb_ADD 0x1F //(7:4), 0x20- lsb, 0x21- xlsb //24bit      
#define eas_status_0_ADD 0x1D 
    
#define par_t1_ADD 0xE9 //0xE9-0xEA
#define par_t2_ADD 0x8A //0x8A-0x8B
#define par_t3_ADD 0x8C
    
#define par_p1_ADD 0x8E //0x8E-0x8F
#define par_p2_ADD 0x90 //0x90-0x91
#define par_p3_ADD 0x92 
#define par_p4_ADD 0x94 //0x94-0x95
#define par_p5_ADD 0x96 //0x96-0x97
#define par_p6_ADD 0x99 
#define par_p7_ADD 0x98 
#define par_p8_ADD 0x9C //0x9C-0x9D
#define par_p9_ADD 0x9E //0x9E-0x9F
#define par_p10_ADD 0xA0
  
#define par_h1_ADD 0xE2 //0xE2 (3:0)- 0xE3
#define par_h2_ADD 0xE2 //0xE2 (7:4)- E1
#define par_h3_ADD 0xE4 
#define par_h4_ADD 0xE5
#define par_h5_ADD 0xE6
#define par_h6_ADD 0xE7 
#define par_h7_ADD 0xE8 
    
#define par_g1_ADD 0xED
#define par_g2_ADD 0xEB //0xEB- 0xEC
#define par_g3_ADD 0xEE 

#define res_heat_rang_ADD 0x02 //(5:4) 
#define res_heat_val_ADD 0x00 
    
#define BME680_RESET_value 0xB6
    
    
typedef enum {
    BME680_page_0 = 0,
    BME680_page_1,
}BME680_page_no_t;    
    
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
} BME680_Config_t;

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
    uint16_t gas_r_90;
    bool gas_valid_r;
    bool heat_stab_r;
    uint8_t gas_range_r;
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
    bool RESET; //resets device //default value is 0x00 and writing to it 0xB6 gives same effect as power-on reset 
    uint8_t ID; //device id

    BME680_Config_t Config;
    BME680_Ctrl_meas_t Ctrl_meas; 
    BME680_Ctrl_hum_t Ctrl_hum;
    BME680_Ctrl_gas_t Ctrl_gas;
    
    uint8_t Gas_wait_x[10];
    uint8_t Res_heat_x[10]; //target of heater resistance
    uint8_t Idac_heat_x[10]; //particular heater set point
    
    BME680_gas_r_t gas_r;
    bool gas_r_recived;
    
    uint16_t hum;
    uint32_t temp;
    uint32_t pres;
    
    BME680_eas_status_0_t eas_status_0;
} BME680_t;

extern BME680_t BME680;


#ifdef	__cplusplus
}
#endif

#endif	/* BME680_H */

