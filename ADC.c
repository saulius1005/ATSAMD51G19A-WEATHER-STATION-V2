#include "settings.h"
#include "ADCVar.h"

void GCLK4_SERCOM_ADC_core_init(){
    GCLK_REGS->GCLK_GENCTRL[4] = GCLK_GENCTRL_SRC_XOSC1 | GCLK_GENCTRL_DIV(12) | GCLK_GENCTRL_GENEN_Msk; //GCLK4 speed is 2Mhz
    while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK4);
}



void ADC0_init(){
    
    GCLK_REGS->GCLK_PCHCTRL[ADC0_GCLK_ID] = GCLK_PCHCTRL_CHEN(0); // Disable channel before reconfiguration
        while (GCLK_REGS->GCLK_PCHCTRL[ADC0_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk);

    GCLK_REGS->GCLK_PCHCTRL[ADC0_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK4 | GCLK_PCHCTRL_CHEN(1);
        while (!(GCLK_REGS->GCLK_PCHCTRL[ADC0_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk));

    MCLK_REGS->MCLK_APBDMASK |= MCLK_APBDMASK_ADC0_Msk; //enable ADC0 module functions
    
    ADC0_REGS->ADC_CTRLA &= ~ ADC_CTRLA_ENABLE_Msk; //disable adc before changing
        while (ADC0_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_ENABLE_Msk);
    
    ADC0_REGS->ADC_CTRLB = ADC_CTRLB_RESSEL_12BIT;
    ADC0_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC1; //use reference voltage connected to VDDANA pin (3.3V)
    ADC0_REGS->ADC_AVGCTRL = ADC_AVGCTRL_SAMPLENUM_1; //use 1 sample
    ADC0_REGS->ADC_SAMPCTRL = 5; //for now just random forget formula and i am too lazy check it on datasheet
    
    ADC0_REGS->ADC_CTRLA |= ADC_CTRLA_ENABLE_Msk; //enable adc0
        while (ADC0_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_ENABLE_Msk); //wait sync
        
}

void ADC0_read(wind_measure_t wind_ch){
   // ADC0_REGS->ADC_CTRLA &= ~ ADC_CTRLA_ENABLE_Msk; //disable adc before changing
    //    while (ADC0_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_ENABLE_Msk);
        
    ADC0_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND | ( wind_ch == 1 ? ADC_INPUTCTRL_MUXPOS_AIN1 : ADC_INPUTCTRL_MUXPOS_AIN0 ); //connect negative channel part to internal gnd
    while (ADC0_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_INPUTCTRL_Msk); //sync changed channel
    
    //ADC0_REGS->ADC_CTRLA |= ADC_CTRLA_ENABLE_Msk; //enable adc0
    //    while (ADC0_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_ENABLE_Msk); //wait sync
    ADC0_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk; //start conversion
    while (!(ADC0_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); //wait result
        ADC0_REGS->ADC_INTFLAG |= ADC_INTFLAG_RESRDY_Msk; //clear result  mask
        
    if(wind_ch == 0)
        WIND.speed = ADC0_REGS->ADC_RESULT;
    else
        WIND.direction = ADC0_REGS->ADC_RESULT;
}
