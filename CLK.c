#include "settings.h"

// Configure CPU clock using external TCXO connected to XOSC1
// Generates ~2 MHz clock via GCLK0 divider
void cpu_2MHz_TCXO_init(){ 
    OSCCTRL_REGS->OSCCTRL_XOSCCTRL[1] = OSCCTRL_XOSCCTRL_ENABLE_Msk; // Enable external oscillator (XOSC1)
    while (!(OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_XOSCRDY1_Msk)); // Wait until oscillator is stable

    // Route XOSC1 to GCLK0 with divider (frequency depends on TCXO input frequency)
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_SRC_XOSC1 | GCLK_GENCTRL_DIV(12) | GCLK_GENCTRL_GENEN_Msk;

    while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK0); // Wait for clock synchronization
    
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_AUTOWS_Msk;
    while (NVMCTRL_REGS->NVMCTRL_STATUS & NVMCTRL_STATUS_READY(0)); // Wait until NVM controller is ready
}


// Configure CPU to run at ~128 MHz using DPLL0 with XOSC1 as reference clock
void cpu_120Mhz_DPLL0_XOSC1_init(){
    // Enable external oscillator XOSC1
    OSCCTRL_REGS->OSCCTRL_XOSCCTRL[1] = OSCCTRL_XOSCCTRL_ENABLE_Msk;
    while (!(OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_XOSCRDY1_Msk)); // Wait until stable

    // Configure DPLL0 reference clock source and prescaler
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLB = OSCCTRL_DPLLCTRLB_REFCLK_XOSC1 | OSCCTRL_DPLLCTRLB_DIV(3); // Reference divider before multiplication

    // Set DPLL multiplication ratio:
    // Fout = Fref * (LDR + 1 + LDRFRAC/32)
    // Effective multiplier here: (39 + 1 + 21/32)
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLRATIO = OSCCTRL_DPLLRATIO_LDR(39) | OSCCTRL_DPLLRATIO_LDRFRAC(0);
    while (OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk); // Wait for ratio sync

    // Enable DPLL0
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLA = OSCCTRL_DPLLCTRLA_ENABLE_Msk;
    while (OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk); // Wait until enabled

    // Route DPLL0 output to GCLK0 (main CPU clock)
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_SRC_DPLL0 | GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN_Msk;
    while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK0); // Wait for clock switch

    // Configure NVM wait states for high-frequency CPU operation
    // AUTOWS allows automatic adjustment, RWS sets base wait states
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_AUTOWS_Msk;
    while (NVMCTRL_REGS->NVMCTRL_STATUS & NVMCTRL_STATUS_READY(0)); // Wait until NVM controller is ready
}


// Simple blocking delay loop based on CPU frequency
// NOTE: Timing is approximate and depends on compiler optimization and F_CPU accuracy
void delay_ms(uint32_t ms){
    uint32_t cycles = (F_CPU / 2000UL) * ms; // Empirical scaling factor

    for(uint32_t i = 0; i < cycles; i++){
        __asm__("nop"); // Prevent loop optimization
    }
}


