#include "settings.h"
#include "BMP180Var.h"

void BMP180_ReadCalibration(bmp180_t *bmp){
    uint16_t *ptr = (uint16_t*)&bmp->calib;
    for(uint8_t i = 0; i < 11; i++){
        ptr[i] = I2C_write_and_read( bmp->address, bmp->reg.CALIB_START + (i << 1), 1, 2);//set bmp180 add, calculate register add, write 1 byte, read 2 bytes and store to its register
    }
}

void BMP180_ReadUTUP(bmp180_t *bmp, bmp180_parameters_t parameter){
    uint16_t bmp180_delay[4] = {4500, 7500, 13500, 25500}; //us 4.5ms, 7.5ms ...
    uint16_t cmd;
    uint16_t delay = bmp180_delay[bmp->oss];
    uint8_t len;

    if(parameter == BMP_T){
        cmd = 0x2E;
        delay = bmp180_delay[0];
        len = 2;
    }
    else{
        cmd = 0x34 + (bmp->oss << 6);
        len = 3;
    }
    I2C_write(bmp->address, (cmd<<8) | bmp->reg.CONTROL, 2, I2C_CMD_Stop); //write to register 0xf4

    TC0_ON(delay); //wait
    while(!(TC0_REGS->COUNT32.TC_INTFLAG & TC_INTFLAG_MC0_Msk));
    TC0_REGS->COUNT32.TC_INTFLAG = TC_INTFLAG_MC0_Msk;
    TC0_OFF();

    uint32_t val = I2C_write_and_read(bmp->address, bmp->reg.DATA, 1, len);//read data
    if(parameter == BMP_T)
        bmp->calib.UT = val;
    else
        bmp->calib.UP = val >> (8 - bmp->oss);
}

void BMP180_ReadUTUP_Task(bmp180_t *bmp, bmp180_parameters_t parameter){
    static uint16_t delay;
    static uint8_t len;
    static uint16_t cmd;

    switch(bmp_state){
        case BMP180_IDLE:
            bmp->cycle = false;
            current_param = parameter;
            uint16_t bmp180_delay[4] = {4500, 7500, 13500, 25500};
            if(parameter == BMP_T){
                cmd = 0x2E;
                delay = bmp180_delay[0];
                len = 2;
            }
            else{
                cmd = 0x34 + (bmp->oss << 6);
                delay = bmp180_delay[bmp->oss];
                len = 3;
            }
            I2C_write(bmp->address, (cmd<<8) | bmp->reg.CONTROL, 2, I2C_CMD_Stop);
            TC0_ON(delay);
            bmp_state = BMP180_WAIT;
        break;

        case BMP180_WAIT:
            if(TC0_timeout){
                //TC0_Expired = 0;
                bmp_state = BMP180_READ;
            }
        break;

        case BMP180_READ:{
            uint32_t val = I2C_write_and_read(bmp->address, bmp->reg.DATA, 1, len);
            if(current_param == BMP_T)
                bmp->calib.UT = val;
            else
                bmp->calib.UP = val >> (8 - bmp->oss);
            TC0_timeout = false;
            bmp->cycle = true;
            bmp_state = BMP180_IDLE;
        }
        break;
    }
}

void BMP180_CalcTrueTP(){
    int32_t X1t = ((BMP180.calib.UT - BMP180.calib.AC6)* BMP180.calib.AC5) >> 15,
            X2t =((int32_t)BMP180.calib.MC << 11) / (X1t + BMP180.calib.MD),
            B5 = X1t + X2t,  
            
            B6 = B5 - 4000,
            X1 = ((int32_t)BMP180.calib.B2 * ((int32_t)B6 *B6 >> 12)) >> 11,
            X2 = (BMP180.calib.AC2 * B6)>> 11,
            X3 = X1+ X2,
            B3 = (((((int32_t)BMP180.calib.AC1* 4)+ X3) << BMP180.oss)+2)/4;
            X1 = BMP180.calib.AC3 * B6>> 13;
            X2 = ((int32_t)BMP180.calib.B1 * ((int32_t)B6 *B6 >> 12)) >> 16;
            X3 = ((X1 + X2)+ 2) >> 2;
    uint32_t B4= BMP180.calib.AC4 * (uint32_t)(X3 + 32768) >> 15,
            B7= ((uint32_t)BMP180.calib.UP - B3) * (50000 >> BMP180.oss);

    BMP180.Temperature = (B5 + 8) >> 4;
 
    if(B7< 0x80000000){
        BMP180.Pressure = (B7 * 2) / B4;
    }
    else{
        BMP180.Pressure = (B7 / B4) * 2;
        }
        X1= (BMP180.Pressure >> 8) * (BMP180.Pressure >> 8);
        X1= (X1 * 3038) >> 16;
        X2= (-7357 * BMP180.Pressure) >> 16;
        BMP180.Pressure += (X1 + X2 + 3791) >> 4;
}

void BMP180_Task(){
    if(A7672E_init.status != WORK)//if not WORK mode 
        return; //skip further code
    if(BMP180.DataReady)// if p and t is received and calculated wait reset
        return;
    switch(BMP180.step){
        case 0:
            BMP180_ReadUTUP_Task(&BMP180, BMP_T); //read temp
            if(BMP180.cycle) BMP180.step = 1;
        break;
        case 1:
            BMP180_ReadUTUP_Task(&BMP180, BMP_P); //read pressure
            if(BMP180.cycle) BMP180.step = 2;
        break;
        case 2:
            BMP180_CalcTrueTP(); //calculate temperature and pressure
            BMP180.step = 0;
            BMP180.DataReady = true;
        break;
    }
}