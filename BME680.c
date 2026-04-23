#include "settings.h"
#include "BME680Var.h"

void BME680_write(uint32_t cmd, uint8_t length){ //max 4 bytes
    
    BME680_CS_LOW();
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(length) | SERCOM_SPIM_LENGTH_LENEN_Msk;
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM4_REGS->SPIM.SERCOM_DATA = swap_and_align(cmd, length);

    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));
    BME680_CS_HIGH();
}

uint32_t BME680_exchange_data(uint32_t cmd, uint8_t tx_length){
   
    uint32_t rx = 0;
    BME680_CS_LOW();
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(tx_length+1) | SERCOM_SPIM_LENGTH_LENEN_Msk; // all time +1 of dummy
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM4_REGS->SPIM.SERCOM_DATA = swap_and_align(cmd, tx_length);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));
    rx = SERCOM4_REGS->SPIM.SERCOM_DATA;   
    BME680_CS_HIGH();  

    return rx;//remove trash (first byte)
}

uint32_t BME680_exchange_data_simple(uint32_t cmd, uint8_t tx_length){
   
    uint32_t rx = 0;
    BME680_CS_LOW();
    SERCOM4_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(tx_length) | SERCOM_SPIM_LENGTH_LENEN_Msk; // all time +1 of dummy
    while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    SERCOM4_REGS->SPIM.SERCOM_DATA = cmd;
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk));
    rx = SERCOM4_REGS->SPIM.SERCOM_DATA;   
    BME680_CS_HIGH();  

    return rx;//remove trash (first byte)
}

/*static inline uint8_t BME680_SPI_WriteAddr(uint8_t reg){
    return reg & 0x7F;
}*/

static inline uint8_t BME680_SPI_ReadAddr(uint8_t reg){
    return reg | 0x80;
}

void BME680_change_page(BME680_page_no_t page){ //set spi page and return page value after write once
    
    if(BME680.STATUS_spi_mem_page == page) //if it is the same page do not change it and skip further code
        return;   
    uint32_t cmd = ((uint32_t) status_ADD << 16) | ((uint16_t) (page == BME680_page_1 ? 0x10 : 0x00) << 8) | BME680_SPI_ReadAddr(status_ADD) ; //set write add (0x73) | set page (0x00 or 0x10) | set read add (0xF3)     
    BME680.STATUS_spi_mem_page = ((swap_and_align(BME680_exchange_data(cmd, 3), 4) & 16) >> 4) == BME680_page_1 ? BME680_page_1 : BME680_page_0; //swap bytes in places from answer total received is 4 bytes, then if 1st page return 1 else 0
}

void BME680_read_ID(){ //can be readed corectly only when spi mem page = 0, otherwise receive 0x00;
    if(BME680.STATUS_spi_mem_page != 0){ //if page 1 change it to 0
        BME680_change_page(BME680_page_0);
    }
    BME680.ID =  swap_and_align(BME680_exchange_data(BME680_SPI_ReadAddr(ID_ADD), 1), 2) & 0xff;
}

void BME680_reset(){ //reset sensor same as power up reset. Requaired manual BME680.RESET value change to false if neede use it again
    if(BME680.RESET)//if already reset do nothing
        return;
    if(BME680.STATUS_spi_mem_page != 0){ //if page 1 change it to 0
        BME680_change_page(BME680_page_0);
    }
    BME680_exchange_data(((uint16_t)Reset_ADD<<16) | BME680_RESET_value, 2); //ignore what it returns
    BME680.RESET = true;
}

void BME680_Config(BME680_filter_t filter, bool spi_3w_en){ //write filter and spi3wire enable values to register 
    
    if((filter == BME680.Config.filter) && (spi_3w_en == BME680.Config.spi_3w_en)) //if filter and spi 3w eneable the same skip further code
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
     uint32_t cmd = ((uint32_t) Config_ADD << 16) | ((uint16_t)filter << 10) | ((uint16_t)spi_3w_en<<8)  | BME680_SPI_ReadAddr(Config_ADD);
     uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
     BME680.Config.filter = (answer >> 2) & 7;
     BME680.Config.spi_3w_en = answer & 1;
}

void BME680_Ctrl_meas(BME680_meas_os_t os_t, BME680_meas_os_t os_p, BME680_mode_t mode){ //write ctrl meas register
    if((os_t == BME680.Ctrl_meas.osrs_t) && (os_p == BME680.Ctrl_meas.osrs_p) && (mode == BME680.Ctrl_meas.mode))
        return;
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    uint32_t cmd = ((uint32_t) Ctrl_meas_ADD << 16) | ((uint16_t)os_t << 13) | ((uint16_t)os_p << 10) | ((uint16_t)mode << 8) | BME680_SPI_ReadAddr(Ctrl_meas_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    
    BME680.Ctrl_meas.osrs_t = (answer >> 5) & 7;
    BME680.Ctrl_meas.osrs_p = (answer >> 2) & 7;
    BME680.Ctrl_meas.mode = answer & 3;
}

void BME680_Ctrl_hum(BME680_meas_os_t os_h, bool spi_3w_int_en){
    if((os_h == BME680.Ctrl_hum.osrs_h) && (spi_3w_int_en == BME680.Ctrl_hum.spi_3w_init_en))
        return;
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    uint32_t cmd = ((uint32_t) Ctrl_hum_ADD << 16) | ((uint16_t)spi_3w_int_en << 14) | ((uint16_t)os_h << 8) | BME680_SPI_ReadAddr(Ctrl_hum_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    
    BME680.Ctrl_hum.osrs_h = answer & 7;
    BME680.Ctrl_hum.spi_3w_init_en = (answer >> 6) & 1;
}

void BME680_Ctrl_gas(bool run_gas, bool heat_off, BME680_nb_conv_t nb_conv){
    if((run_gas == BME680.Ctrl_gas.run_gas) && (heat_off == BME680.Ctrl_gas.heat_off) && (nb_conv == BME680.Ctrl_gas.nb_conv))
        return;
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    uint32_t cmd = ((uint32_t) Ctrl_gas_ADD << 16) | ((uint16_t)run_gas << 12) | ((uint16_t)nb_conv << 8) | BME680_SPI_ReadAddr(Ctrl_gas_ADD); //ctrl_gas_1 register
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    
    BME680.Ctrl_gas.nb_conv = answer & 15;
    BME680.Ctrl_gas.run_gas = (answer >> 4) & 1;
    
    cmd = ((uint32_t) (Ctrl_gas_ADD - 1) << 16) | ((uint16_t)heat_off << 11) | BME680_SPI_ReadAddr(Ctrl_gas_ADD - 1); //ctrl_gas_0 register
    answer = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    
    BME680.Ctrl_gas.heat_off = (answer >> 3) & 1;
}

void BME680_gas_wait_90(uint8_t * gas_wait){
    uint8_t datachanged = 0;
    for(uint8_t i = 0; i < 10; i++){
        if(gas_wait[i] == BME680.Gas_wait_x[i])
            datachanged++;
        else
            break;
    }
    if(datachanged == 9) //data is the same
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    for(uint8_t i = 0; i < 10; i++){
        uint32_t cmd = ((uint32_t) (Gas_wait_x_ADD - i) << 16) | ((uint16_t)gas_wait[9 - i] << 8) | BME680_SPI_ReadAddr(Gas_wait_x_ADD - i);
        BME680.Gas_wait_x[9 - i]  = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    }
}

void BME680_res_heat_90(uint8_t * res_heat){
    uint8_t datachanged = 0;
    for(uint8_t i = 0; i < 10; i++){
        if(res_heat[i] == BME680.Res_heat_x[i])
            datachanged++;
        else
            break;
    }
    if(datachanged == 9) //data is the same
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    for(uint8_t i = 0; i < 10; i++){
        uint32_t cmd = ((uint32_t) (Res_heat_x_ADD - i) << 16) | ((uint16_t)res_heat[9 - i] << 8) | BME680_SPI_ReadAddr(Res_heat_x_ADD - i);
        BME680.Res_heat_x[9 - i]  = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    }
}

void BME680_idac_heat_90(uint8_t * idac_heat){
    uint8_t datachanged = 0;
    for(uint8_t i = 0; i < 10; i++){
        if(idac_heat[i] == BME680.Idac_heat_x[i])
            datachanged++;
        else
            break;
    }
    if(datachanged == 9) //data is the same
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    for(uint8_t i = 0; i < 10; i++){
        uint32_t cmd = ((uint32_t) (Idac_heat_x_ADD - i) << 16) | ((uint16_t)idac_heat[9 - i] << 8) | BME680_SPI_ReadAddr(Idac_heat_x_ADD - i);
        BME680.Idac_heat_x[9 - i]  = swap_and_align(BME680_exchange_data(cmd, 3), 4);
    }
}

void BME680_read_gas(){ //need manually reset BME680.gas_r_recived = false; // to begin new reading
    if(BME680.gas_r_recived)
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    uint32_t cmd = BME680_SPI_ReadAddr(Gas_r_msb_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 2), 3);
    
    BME680.gas_r_recived = true;
    BME680.gas_r.gas_range_r = answer & 15;
    BME680.gas_r.heat_stab_r = (answer >> 4) & 1;
    BME680.gas_r.gas_valid_r = (answer >> 5) & 1;
    BME680.gas_r.gas_r_90 = (answer >> 6) & 1023;
}

void BME680_read_hum(){ //need manually reset BME680.hum_recived = false; // to begin new reading
    if(BME680.hum_recived)
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    uint32_t cmd = BME680_SPI_ReadAddr(hum_msb_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 2), 3);
    
    BME680.hum_recived = true;
    BME680.hum = answer;

}

void BME680_read_temp(){ //need manually reset BME680.hum_recived = false; // to begin new reading // first msb, then lsb and xlsb
    if(BME680.temp_recived)
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }    
    uint32_t cmd = BME680_SPI_ReadAddr(temp_msb_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data_simple(cmd, 4), 4) & 0xffffff;
    
    BME680.temp_recived = true;
    BME680.temp = answer >> 4;

}

void BME680_read_pres(){ //need manually reset BME680.hum_recived = false; // to begin new reading
    if(BME680.pres_recived)
        return;
    
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    uint32_t cmd = BME680_SPI_ReadAddr(pres_msb_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data_simple(cmd, 4), 4) & 0xffffff;
    
    BME680.pres_recived = true;
    BME680.pres = answer >> 4;

}

void BME680_eas_status0(){
    if(BME680.eas_status_received)
            return;
    if(BME680.STATUS_spi_mem_page != 1){ //if page 0 change it to 1
        BME680_change_page(BME680_page_1);
    }
    
    uint32_t cmd = BME680_SPI_ReadAddr(eas_status_0_ADD);
    uint32_t answer = swap_and_align(BME680_exchange_data(cmd, 1), 2);
    
    BME680.eas_status_received = true;
    BME680.eas_status_0.gas_maes_index_0 = answer & 15;
    BME680.eas_status_0.measuring = (answer >> 5) & 1;
    BME680.eas_status_0.gas_measuring = (answer >> 6) & 1;
    BME680.eas_status_0.new_data_0 = (answer >> 7) & 1;
}

void BME680_read_gas_calib(){
    
}

void BME680_read_hum_calib(){
   
}

void BME680_read_temp_calib(){ //first lsb then msb
    if(BME680.calibration_data.temp_calib_received) //if calibration data already received skip further code
        return;
    
    if(BME680.STATUS_spi_mem_page != 0){ //if page 1 change it to 0
        BME680_change_page(BME680_page_0);
    }   
    uint32_t cmd = par_t1_ADD;
    uint32_t answer = BME680_exchange_data_simple(cmd, 3);
    
    BME680.calibration_data.par_t1 = answer >> 8; //exmp. received ff8e65 but flipped 658eff >> 8 = 658e
    
    cmd = par_t2_ADD;
    answer = BME680_exchange_data_simple(cmd, 4);
    
    BME680.calibration_data.par_t2 = (answer >> 8) & 0xffff; //received fff56603 but flipped 0366f5ff >> 8 = 0366f5 & 0xffff = 66f5
    BME680.calibration_data.par_t3 = answer >> 24; //0366f5ff >> 24 = 03
    
    BME680.calibration_data.temp_calib_received = true;
}

void BME680_read_pres_calib(){
if(BME680.calibration_data.pres_calib_received) //if calibration data already received skip further code
        return;
    
    if(BME680.STATUS_spi_mem_page != 0){ //if page 1 change it to 0
        BME680_change_page(BME680_page_0);
    }   
    uint32_t cmd = par_p1_ADD;
    uint32_t answer = BME680_exchange_data_simple(cmd, 3);
    
    BME680.calibration_data.par_p1 = answer >> 8; //exmp. received ff8e65 but flipped 658eff >> 8 = 658e
    
    cmd = par_p2_ADD;
    answer = BME680_exchange_data_simple(cmd, 4);
    
    BME680.calibration_data.par_p2 = (answer >> 8) & 0xffff; //received fff56603 but flipped 0366f5ff >> 8 = 0366f5 & 0xffff = 66f5
    BME680.calibration_data.par_p3 = answer >> 24; //0366f5ff >> 24 = 03
    
    cmd = par_p4_ADD;
    BME680.calibration_data.par_p4 = BME680_exchange_data_simple(cmd, 3) >> 8;
    
    cmd = par_p5_ADD;
    BME680.calibration_data.par_p5 = BME680_exchange_data_simple(cmd, 3) >> 8;
    
    cmd = par_p7_ADD;
    answer = BME680_exchange_data_simple(cmd, 3) >> 8;
    
    BME680.calibration_data.par_p7 = answer & 0xff;
    BME680.calibration_data.par_p6 = answer >> 8;
    
    cmd = par_p8_ADD;
    BME680.calibration_data.par_p8 = BME680_exchange_data_simple(cmd, 3) >> 8;
       
    cmd = par_p9_ADD;
    answer = BME680_exchange_data_simple(cmd, 4);
    
    BME680.calibration_data.par_p9 = (answer >> 8) & 0xffff;
    BME680.calibration_data.par_p10 = answer >> 24;   
    
    BME680.calibration_data.pres_calib_received = true;    
}

void BME680_calculate_temperature(){
    BME680_read_temp_calib(); //reading temperature calibration for bme680 sensor, calibration data not changing, enough to read once
    BME680_read_temp(); //first read temperature value;
    int32_t var1 = ((int32_t)BME680.temp >> 3) - ((int32_t)BME680.calibration_data.par_t1 << 1); 
    int32_t var2 = (var1 * (int32_t)BME680.calibration_data.par_t2) >> 11; 
    int32_t var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * ((int32_t)BME680.calibration_data.par_t3 << 4)) >> 14; 
    BME680.t_fine = var2 + var3; 
    BME680.temperature = ((BME680.t_fine * 5) + 128) >> 8;
    
    BME680.temp_recived = false;
    BME680.Ctrl_meas.mode = sleep_mode; //reset forced mode to sleep. It means new measurement next time.
}

void BME680_calculate_pressure(){
    BME680_read_pres_calib(); //if already readed it will be skipped
    BME680_read_pres();
    int32_t var1 = ((int32_t)BME680.t_fine >> 1) - 64000; 
    int32_t var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t)BME680.calibration_data.par_p6) >> 2; 
    var2 = var2 + ((var1 * (int32_t)BME680.calibration_data.par_p5) << 1); 
    var2 = (var2 >> 2) + ((int32_t)BME680.calibration_data.par_p4 << 16); 
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * ((int32_t)BME680.calibration_data.par_p3 << 5)) >> 3) + (((int32_t)BME680.calibration_data.par_p2 * var1) >> 1);
    var1 = var1 >> 18; 
    var1 = ((32768 + var1) * (int32_t)BME680.calibration_data.par_p1) >> 15; 
    BME680.pressure = 1048576 - BME680.pres; 
    BME680.pressure = (uint32_t)((BME680.pressure - (var2 >> 12)) * ((uint32_t)3125)); 
    if (BME680.pressure >= (1 << 30)) BME680.pressure = ((BME680.pressure / (uint32_t)var1) << 1); 
    else BME680.pressure = ((BME680.pressure << 1) / (uint32_t)var1); 
    var1 = ((int32_t)BME680.calibration_data.par_p9 * (int32_t)(((BME680.pressure >> 3) * (BME680.pressure >> 3)) >> 13)) >> 12; 
    var2 = ((int32_t)(BME680.pressure >> 2) * (int32_t)BME680.calibration_data.par_p8) >> 13; 
    int32_t var3 = ((int32_t)(BME680.pressure >> 8) * (int32_t)(BME680.pressure >> 8) * (int32_t)(BME680.pressure >> 8) * (int32_t)BME680.calibration_data.par_p10) >> 17; 
    BME680.pressure = (int32_t)(BME680.pressure) + ((var1 + var2 + var3 + ((int32_t)BME680.calibration_data.par_p7 << 7)) >> 4);
    
    BME680.pres_recived = false; //reset for new measurement 
    BME680.Ctrl_meas.mode = sleep_mode;
}