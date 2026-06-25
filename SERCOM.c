#include "settings.h"
#include "I2C.h"
#include "USART.h"


void GCLK1_SERCOM_SPIM_core_init(){
        OSCCTRL_REGS->DPLL[1].OSCCTRL_DPLLCTRLB = OSCCTRL_DPLLCTRLB_REFCLK_XOSC1 | OSCCTRL_DPLLCTRLB_DIV(8);//3Mhz

        OSCCTRL_REGS->DPLL[1].OSCCTRL_DPLLRATIO = OSCCTRL_DPLLRATIO_LDR(39);//120Mhz
        while (OSCCTRL_REGS->DPLL[1].OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk);

        OSCCTRL_REGS->DPLL[1].OSCCTRL_DPLLCTRLA = OSCCTRL_DPLLCTRLA_ENABLE_Msk;
        while (OSCCTRL_REGS->DPLL[1].OSCCTRL_DPLLSYNCBUSY &  OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk);
        while (!(OSCCTRL_REGS->DPLL[1].OSCCTRL_DPLLSTATUS & OSCCTRL_DPLLSTATUS_LOCK_Msk));

        GCLK_REGS->GCLK_GENCTRL[1] = GCLK_GENCTRL_SRC_DPLL1 | GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN_Msk; //GCLK1 Max speed is 200Mhz
        while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK1);
}

void GCLK2_SERCOM_I2CM_USARTM_core_init(){
    GCLK_REGS->GCLK_GENCTRL[2] = GCLK_GENCTRL_SRC_XOSC1 | GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN_Msk; //GCLK2 speed is 24Mhz
    while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK2);
}

// Generic SERCOM initialization depending on selected interface
void SERCOM_init(sercom_init_t interface){
      
    switch(interface){
        case USART_RS485:// SERCOM2 RS485 Master
            
            GCLK_REGS->GCLK_PCHCTRL[SERCOM2_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(0); // Disable channel before reconfiguration
            while (GCLK_REGS->GCLK_PCHCTRL[SERCOM2_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk);

            GCLK_REGS->GCLK_PCHCTRL[SERCOM2_GCLK_ID_CORE] = GCLK_PCHCTRL_GEN_GCLK2 | GCLK_PCHCTRL_CHEN(1);
                while (!(GCLK_REGS->GCLK_PCHCTRL[SERCOM2_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk)); //use the same core clock as i2c - 24Mhz
            
            MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_SERCOM3_Msk; //enable module functions
            SERCOM2_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_ENABLE(0); //disable
                while(SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk); //wait sync
            
            SERCOM2_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT | SERCOM_USART_INT_CTRLB_RXEN_Msk | SERCOM_USART_INT_CTRLB_TXEN_Msk | SERCOM_USART_INT_CTRLB_SFDE_Msk;//8bit character, 1 stop bit, rx, tx enabled, start frame detection on
            SERCOM2_REGS->USART_INT.SERCOM_CTRLC = SERCOM_USART_INT_CTRLC_DATA32B_DATA_READ_WRITE_32BIT;            
            SERCOM2_REGS->USART_INT.SERCOM_BAUD = SERCOM_USART_ASYNC_ARITH_BAUD(USART_BAUD);
            SERCOM2_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_INT_CTRLA_RXPO_PAD2 | SERCOM_USART_INT_CTRLA_TXPO_PAD0 | SERCOM_USART_INT_CTRLA_CMODE_ASYNC | SERCOM_USART_INT_CTRLA_DORD_LSB | SERCOM_USART_INT_CTRLA_ENABLE(1); // rx- pad2, tx- pad0, xck pad1
                while(SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk); //wait sync
        break;

        case SPI_SCREEN: // SERCOM0 SPI- LCD and Touch

            GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(0); // Disable channel before reconfiguration
            while (GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk);

            GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_CORE] = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN(1); //connect sercom0 core to GCLK1
            while (!(GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk));

            MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_SERCOM0_Msk;

            SERCOM0_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE(0); // Disable before configuration
            while(SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk);

            SERCOM0_REGS->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_CHSIZE_8_BIT | SERCOM_SPIM_CTRLB_RXEN_Msk; //mssen is used when hw controls SS (single device)
            while (SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_CTRLB_Msk);

            SERCOM0_REGS->SPIM.SERCOM_CTRLC = SERCOM_SPIM_CTRLC_DATA32B_DATA_TRANS_32BIT;

            SERCOM0_REGS->SPIM.SERCOM_BAUD = SERCOM_SPI_BAUD(30000000);//~30Mhz (Core speed 60Mhz)

            SERCOM0_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE(1) | SERCOM_SPIM_CTRLA_MODE_SPI_MASTER | SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH | SERCOM_SPIM_CTRLA_CPHA_TRAILING_EDGE| SERCOM_SPIM_CTRLA_DIPO_PAD0 | SERCOM_SPIM_CTRLA_DOPO_PAD2 | SERCOM_SPIM_CTRLA_DORD_MSB;
            while(SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk);

        break;
        
        case SPI_SENSOR: // SERCOM4 SPI BME680

            GCLK_REGS->GCLK_PCHCTRL[SERCOM4_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(0); // Disable channel before reconfiguration
            while (GCLK_REGS->GCLK_PCHCTRL[SERCOM4_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk);

            GCLK_REGS->GCLK_PCHCTRL[SERCOM4_GCLK_ID_CORE] = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN(1); //connect sercom4 core to GCLK1
            while (!(GCLK_REGS->GCLK_PCHCTRL[SERCOM4_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk));

            MCLK_REGS->MCLK_APBDMASK |= MCLK_APBDMASK_SERCOM4_Msk;

            SERCOM4_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE(0); // Disable before configuration
            while(SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk);

            SERCOM4_REGS->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_CHSIZE_8_BIT | SERCOM_SPIM_CTRLB_RXEN_Msk; //mssen is used when hw controls SS (single device)
            while (SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_CTRLB_Msk);

            SERCOM4_REGS->SPIM.SERCOM_CTRLC = SERCOM_SPIM_CTRLC_DATA32B_DATA_TRANS_32BIT;

            SERCOM4_REGS->SPIM.SERCOM_BAUD = SERCOM_SPI_BAUD(10000000);//~10Mhz (Core speed 60Mhz)

            SERCOM4_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE(1) | SERCOM_SPIM_CTRLA_MODE_SPI_MASTER | SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH | SERCOM_SPIM_CTRLA_CPHA_TRAILING_EDGE| SERCOM_SPIM_CTRLA_DIPO_PAD0 | SERCOM_SPIM_CTRLA_DOPO_PAD2 | SERCOM_SPIM_CTRLA_DORD_MSB;
            while(SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk);
            
            BME680_CS_HIGH();

        break;        

        case USART_GSM: //A7672E GSM module 
            
            GCLK_REGS->GCLK_PCHCTRL[SERCOM3_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(0); // Disable channel before reconfiguration
            while (GCLK_REGS->GCLK_PCHCTRL[SERCOM3_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk);

            GCLK_REGS->GCLK_PCHCTRL[SERCOM3_GCLK_ID_CORE] = GCLK_PCHCTRL_GEN_GCLK2 | GCLK_PCHCTRL_CHEN(1);
                while (!(GCLK_REGS->GCLK_PCHCTRL[SERCOM3_GCLK_ID_CORE] & GCLK_PCHCTRL_CHEN_Msk)); //use the same core clock as i2c - 24Mhz
            
            MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_SERCOM3_Msk; //enable module functions
            SERCOM3_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_ENABLE(0); //disable
                while(SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk); //wait sync
            
            SERCOM3_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT | SERCOM_USART_INT_CTRLB_RXEN_Msk | SERCOM_USART_INT_CTRLB_TXEN_Msk | SERCOM_USART_INT_CTRLB_SFDE_Msk;//8bit character, 1 stop bit, rx, tx enabled, start frame detection on
            SERCOM3_REGS->USART_INT.SERCOM_CTRLC = SERCOM_USART_INT_CTRLC_DATA32B_DATA_READ_WRITE_32BIT;            
            SERCOM3_REGS->USART_INT.SERCOM_BAUD = SERCOM_USART_ASYNC_ARITH_BAUD(USART_BAUD);
            SERCOM3_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_INT_CTRLA_RXPO_PAD2 | SERCOM_USART_INT_CTRLA_TXPO_PAD0 | SERCOM_USART_INT_CTRLA_CMODE_ASYNC | SERCOM_USART_INT_CTRLA_DORD_LSB | SERCOM_USART_INT_CTRLA_ENABLE(1); // rx- pad2, tx- pad0, xck pad1
                while(SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk); //wait sync
            
        break;
    }
}

//SPI

void SPI0_Transfer_32b_HW(uint32_t data, uint8_t length){
    // Configure transfer length in bytes (1?4) using hardware length register
    SERCOM0_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(length) | SERCOM_SPIM_LENGTH_LENEN(1);
    while (SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);

    // Wait until data register empty
    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));

    // Write packed data word
    SERCOM0_REGS->SPIM.SERCOM_DATA = swap_and_align(data, length);

    // Wait until transmission is fully complete
    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));
}

void SPI0_Transfer_set_length(uint8_t length){ //once set transfer data length
    // Configure transfer length in bytes (1?4) using hardware length register
    SERCOM0_REGS->SPIM.SERCOM_LENGTH = SERCOM_SPIM_LENGTH_LEN(length) | SERCOM_SPIM_LENGTH_LENEN_Msk;
    while (SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_LENGTH_Msk);
}

void SPI0_Transfer_32b_HW_cycle(uint32_t data){ //use this function to cycle write to spi
    // Wait until data register empty
    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk));
    // Write packed data word
    SERCOM0_REGS->SPIM.SERCOM_DATA = data;
    // Wait until transmission is fully complete
    while (!(SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk));
}

void SPI0_Baud_Switch(uint32_t baud){
    SERCOM0_REGS->SPIM.SERCOM_CTRLA &= ~SERCOM_SPIM_CTRLA_ENABLE_Msk; // Disable before configuration
        while(SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk);
    SERCOM0_REGS->SPIM.SERCOM_BAUD = SERCOM_SPI_BAUD(baud);// 2.5Mhz SPI
    
    SERCOM0_REGS->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE_Msk;
        while(SERCOM0_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk);
}

//I2C

bool I2C_have_error(){
  if(SERCOM2_REGS->I2CM.SERCOM_INTFLAG & SERCOM_I2CM_INTFLAG_ERROR_Msk){ //only if error 
        SERCOM2_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_ERROR_Msk; //clear error flag
        I2C_SUCK.FaultCode = SERCOM2_REGS->I2CM.SERCOM_STATUS; //return all fault codes
        return true;
  }
  return false;
}

static inline void I2C_CMD(i2c_cmd_t cmd){
    SERCOM2_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_CMD(cmd);
    while (SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP_Msk);
}

uint32_t swap_and_align(uint32_t data, uint8_t length){
    uint32_t swapped = __builtin_bswap32(data);
    uint32_t shift = (4 - length) * 8;
    return swapped >> shift;
}

void I2C_write(uint8_t addr, uint32_t data, uint8_t length, i2c_cmd_t endaction){

    uint32_t timeout = 100000; //waiting timout protection    
    
    while ((SERCOM2_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) != SERCOM_I2CM_STATUS_BUSSTATE(1)){//wait till bus state will be idle   
        if (--timeout == 0) {
                I2C_SUCK.FaultCode = 4000;//bus state never gets to IDLE
                return;
        }
    }
    SERCOM2_REGS->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR(addr | 0) | SERCOM_I2CM_ADDR_LEN(length) | SERCOM_I2CM_ADDR_LENEN_Msk;// START + address + length    
    timeout = 100000;       
    while (!(SERCOM2_REGS->I2CM.SERCOM_INTFLAG & (SERCOM_I2CM_INTFLAG_MB_Msk | SERCOM_I2CM_INTFLAG_ERROR_Msk))){// wait MB or error
        if (--timeout == 0) {
            I2C_SUCK.FaultCode = 5000;//error after second byte transfering timeout
            I2C_CMD(I2C_CMD_Stop); //stop
            return;
        }
    } 
    if(I2C_have_error() == true){ //if error receiving after add initiate stop and go back
        I2C_CMD(I2C_CMD_Stop); //stop
        return;         
    }
    SERCOM2_REGS->I2CM.SERCOM_DATA = data;  

    if(endaction == I2C_CMD_Stop){
        timeout = 100000;                   
        I2C_CMD(I2C_CMD_Stop);         
        while ((SERCOM2_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) != SERCOM_I2CM_STATUS_BUSSTATE_IDLE){//go to idle (STOP)
            if (--timeout == 0) {
                I2C_SUCK.FaultCode = 6000;           
                return;
            }
        }             
    }
    else
        I2C_CMD(I2C_CMD_RepeatStart);      
 }

uint32_t I2C_read(uint8_t addr, uint8_t readlen){
    uint32_t timeout = 100000;
    uint32_t result = 0;

    //I2C_write(addr, reg, writelen, I2C_CMD_Stop);

    SERCOM2_REGS->I2CM.SERCOM_ADDR =  SERCOM_I2CM_ADDR_ADDR(addr | 1) | SERCOM_I2CM_ADDR_LEN(readlen) | SERCOM_I2CM_ADDR_LENEN_Msk;         
    while (!(SERCOM2_REGS->I2CM.SERCOM_INTFLAG & (SERCOM_I2CM_INTFLAG_SB_Msk | SERCOM_I2CM_INTFLAG_ERROR_Msk))){// wait second byte
        if (--timeout == 0) {
            I2C_SUCK.FaultCode = 7000;//error after second byte transfering timeout
            I2C_CMD(I2C_CMD_Stop); //stop
            return 0;
        }
    } 
    if(I2C_have_error() == true){ //if error receiving after add initiate stop and go back
        I2C_CMD(I2C_CMD_Stop); //stop
        return 0;         
    }
    result = swap_and_align(SERCOM2_REGS->I2CM.SERCOM_DATA, readlen); //after everything complete reading data packet and swap bytes
    while (SERCOM2_REGS->I2CM.SERCOM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP_Msk);//smen make all ack, nack and stop
    return result;
}

uint32_t I2C_write_and_read(uint8_t addr, uint32_t reg, uint8_t writelen, uint8_t readlen){
    I2C_write(addr, reg, writelen, I2C_CMD_Stop);
    return I2C_read(addr, readlen);
}

//USART

void USART_set_read_length(uint8_t length){ //how much bytes we need to read
    SERCOM3_REGS->USART_INT.SERCOM_LENGTH = SERCOM_USART_INT_LENGTH_LEN(length) | SERCOM_USART_INT_LENGTH_LENEN_Msk;
    while(SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_LENGTH_Msk); //wait sync
}

void USART_write_str(char *str){
    uint8_t length = 0;
    while(str[length]) length++;//calculate how much bytes in total

    SERCOM3_REGS->USART_INT.SERCOM_LENGTH = SERCOM_USART_INT_LENGTH_LEN(length) | SERCOM_USART_INT_LENGTH_LENEN_Msk;//set length to usart hardware once
    while(SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_LENGTH_Msk);//wait sync
    
    const uint32_t* arr = (const uint32_t*) str; //create pointer of 32bit length
    
    length = (length+3)>>2;//how much it will be of 32bits
    SERCOM3_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_TXC_Msk;//clear last flag 
    while(length--){
        while(!(SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk));
        SERCOM3_REGS->USART_INT.SERCOM_DATA = *arr++;
    }
    while(!(SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk)); //stop bid set and shift register is empty and no new data  
    SERCOM3_REGS->USART_INT.SERCOM_LENGTH &= ~SERCOM_USART_INT_LENGTH_LENEN_Msk;   
}

void USART_printf(const char *fmt, ...){
    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    USART_write_str(buffer);
    //USART_set_read_length(1);//set back to one byte    
}