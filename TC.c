#include "settings.h"
#include "TCVar.h"

void GCLK3_SERCOM_TC_core_init(){ //used for 1us and for 1ms
    GCLK_REGS->GCLK_GENCTRL[3] = GCLK_GENCTRL_SRC_XOSC1 | GCLK_GENCTRL_DIV(24) | GCLK_GENCTRL_GENEN_Msk; //GCLK3 base clock speed is 24Mhz/24 = 1Mhz
    while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK3);
}

void TC0_init(){ //TC0 as us counter used for A7672E initialization and after it used in xpt2046 touch screen data reading delay
    
    GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] = GCLK_PCHCTRL_CHEN(0);//turn off before all changes
    while (GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk);
    
    TC0_REGS->COUNT32.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk; //disable timmer
    while(TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk); //wait sync
    
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_TC0_Msk | MCLK_APBAMASK_TC1_Msk; //turn on module    
    
    GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK3 | GCLK_PCHCTRL_CHEN(1);//conecting peripheral to gclk3 (1Mhz)
    while (!(GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk));
    
    TC0_REGS->COUNT32.TC_WAVE = TC_WAVE_WAVEGEN_MFRQ_Val; // 0x1

    TC0_REGS->COUNT32.TC_CTRLA = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_PRESCSYNC_GCLK; //MFRQ in WAve register is default as 0 and presync also 0 (GCLK)
}

void TC0_OFF(){
    TC0_REGS->COUNT32.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while(TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk);
}

void TC0_ON(uint32_t period_us){ //for 

    TC0_OFF();
    
    TC0_timeout = false;

    TC0_REGS->COUNT32.TC_CC[0] = period_us;
    while(TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk);

    TC0_REGS->COUNT32.TC_COUNT = 0;
    while(TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_COUNT_Msk);

    TC0_REGS->COUNT32.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while(TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk);
}

void TC0_CHECKER(){
    if (TC0_REGS->COUNT32.TC_INTFLAG & TC_INTFLAG_MC0_Msk){
        TC0_REGS->COUNT32.TC_INTFLAG = TC_INTFLAG_MC0_Msk;
        TC0_timeout = true;
        TC0_OFF();
    }
}

void TC2_init(){ //used for a7672e gnss and gsm time messages
    
    GCLK_REGS->GCLK_PCHCTRL[TC2_GCLK_ID] = GCLK_PCHCTRL_CHEN(0);//turn off before all changes
    while (GCLK_REGS->GCLK_PCHCTRL[TC2_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk);
    
    TC2_REGS->COUNT32.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk; //disable timmer
    while(TC2_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk); //wait sync    
    
    MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_TC2_Msk | MCLK_APBBMASK_TC3_Msk; //turn on TC3 also for 32bit usage make 2 16bit to 1 32bit   
    
    GCLK_REGS->GCLK_PCHCTRL[TC2_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK3 | GCLK_PCHCTRL_CHEN(1);//conecting peripheral to gclk3 (1Mhz)
    while (!(GCLK_REGS->GCLK_PCHCTRL[TC2_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk));
    
   
    TC2_REGS->COUNT32.TC_WAVE = TC_WAVE_WAVEGEN_MFRQ_Val; // 0x1

    TC2_REGS->COUNT32.TC_CTRLA = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_PRESCSYNC_GCLK;
    
}

void TC2_OFF(){
    TC2_REGS->COUNT32.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while(TC2_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk);
}

void TC2_ON(uint32_t period_us){

    TC2_OFF();
    
    TC2_timeout = false;

    TC2_REGS->COUNT32.TC_CC[0] = period_us;
    while(TC2_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk);

    TC2_REGS->COUNT32.TC_COUNT = 0;
    while(TC2_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_COUNT_Msk);

    TC2_REGS->COUNT32.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while(TC2_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk);
}

void TC2_CHECKER(){
    if (TC2_REGS->COUNT32.TC_INTFLAG & TC_INTFLAG_MC0_Msk){
        TC2_REGS->COUNT32.TC_INTFLAG = TC_INTFLAG_MC0_Msk;
        TC2_timeout = true;
        TC2_OFF();
    }
}