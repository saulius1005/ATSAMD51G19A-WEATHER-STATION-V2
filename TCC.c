#include "settings.h"
#include "TCCVar.h"

void TCC0_init(){
    // Disable generic clock
    GCLK_REGS->GCLK_PCHCTRL[TCC0_GCLK_ID] = GCLK_PCHCTRL_CHEN(0);
    while (GCLK_REGS->GCLK_PCHCTRL[TCC0_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk);

    // Disable TCC0
    TCC0_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while (TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk);

    // Enable peripheral clock
    MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_TCC0_Msk;

    // Connect GCLK3 (1 MHz)
    GCLK_REGS->GCLK_PCHCTRL[TCC0_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK3 | GCLK_PCHCTRL_CHEN_Msk;

    while (!(GCLK_REGS->GCLK_PCHCTRL[TCC0_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk));

    // Prescaler = 1
    TCC0_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1 | TCC_CTRLA_PRESCSYNC_GCLK;

    while (TCC0_REGS->TCC_SYNCBUSY);

    // Normal PWM mode (counter simply counts 0->PER)
    TCC0_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;
    while (TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_WAVE_Msk);
}

void TCC0_OFF(){
    TCC0_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;

    while (TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk);
}

void TCC0_ON(uint32_t period_us){
    TCC0_OFF();

    //TCC0_timeout = false;

    // Clear old overflow flag
    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_OVF_Msk;

    // Reset counter
    TCC0_REGS->TCC_COUNT = 0;
    while (TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_COUNT_Msk);

    // Set period
    TCC0_REGS->TCC_PER = period_us;
    while (TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_PER_Msk);

    // Enable timer
    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while (TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk);
}

void TCC0_CHECKER(){
    if (A7672E_init.status != WORK)//if not WORK mode do not count
        return;
    if (TCC0_REGS->TCC_INTFLAG & TCC_INTFLAG_OVF_Msk)
    {
        TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_OVF_Msk;
        
        Periodic_Checker_Devices.period_counter++; //dont care if it overfill :D and if so it starts from 0

        
        //data check for A7672E GSM/GNSS module (SERCOM3)
        if(Periodic_Checker_Devices.GSM.update_stat == UPDATED && !(Periodic_Checker_Devices.period_counter % Periodic_Checker_Devices.GSM.update_time)){
            Periodic_Checker_Devices.GSM.update_stat = PREPARED;
            //Periodic_Checker_Devices.GSM.start_at = Periodic_Checker_Devices.period_counter;           
        }        
        if(Periodic_Checker_Devices.TIME.update_stat == UPDATED && !(Periodic_Checker_Devices.period_counter % Periodic_Checker_Devices.TIME.update_time)){
            Periodic_Checker_Devices.TIME.update_stat = PREPARED;
            //Periodic_Checker_Devices.TIME.start_at = Periodic_Checker_Devices.period_counter;
        }
        
        if(Periodic_Checker_Devices.SERVER.update_stat == UPDATED && !(Periodic_Checker_Devices.period_counter % Periodic_Checker_Devices.SERVER.update_time)){
            Periodic_Checker_Devices.SERVER.update_stat = PREPARED;
            //Periodic_Checker_Devices.SERVER.start_at = Periodic_Checker_Devices.period_counter;
        }
        
        //data check for Towers using RS485 (SERCOM2)
        if(Periodic_Checker_Devices.TOWERS.update_stat == UPDATED && !(Periodic_Checker_Devices.period_counter % (Periodic_Checker_Devices.TOWERS.update_time / A7672E_NET.towers_in_total))){
            Periodic_Checker_Devices.TOWERS.update_stat = PREPARED;
            Periodic_Checker_Devices.TOWERS.start_at = Periodic_Checker_Devices.period_counter;
        }
        
    }
}

void TCC1_init(){
    // Disable generic clock
    GCLK_REGS->GCLK_PCHCTRL[TCC1_GCLK_ID] = GCLK_PCHCTRL_CHEN(0);
    while (GCLK_REGS->GCLK_PCHCTRL[TCC1_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk);

    // Disable TCC1
    TCC1_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while (TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk);

    // Enable peripheral clock
    MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_TCC1_Msk;

    // Connect GCLK3 (1 MHz)
    GCLK_REGS->GCLK_PCHCTRL[TCC1_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK3 | GCLK_PCHCTRL_CHEN_Msk;

    while (!(GCLK_REGS->GCLK_PCHCTRL[TCC1_GCLK_ID] & GCLK_PCHCTRL_CHEN_Msk));

    // Prescaler = 1
    TCC1_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1 | TCC_CTRLA_PRESCSYNC_GCLK;

    while (TCC1_REGS->TCC_SYNCBUSY);

    // Normal PWM mode (counter simply counts 0->PER)
    TCC1_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;
    while (TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_WAVE_Msk);
}

void TCC1_OFF(){
    TCC1_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;

    while (TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk);
}

void TCC1_ON(uint32_t period_us){
    TCC1_OFF();

    TCC1_timeout = false;

    // Clear old overflow flag
    TCC1_REGS->TCC_INTFLAG = TCC_INTFLAG_OVF_Msk;

    // Reset counter
    TCC1_REGS->TCC_COUNT = 0;
    while (TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_COUNT_Msk);

    // Set period
    TCC1_REGS->TCC_PER = period_us;
    while (TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_PER_Msk);

    // Enable timer
    TCC1_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while (TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk);
}

void TCC1_CHECKER(){
    if (TCC1_REGS->TCC_INTFLAG & TCC_INTFLAG_OVF_Msk) {
        TCC1_REGS->TCC_INTFLAG = TCC_INTFLAG_OVF_Msk;
        TCC1_timeout = true;
        TCC1_OFF();
    }
}