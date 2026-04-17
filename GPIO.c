#include "settings.h"

// Initialize GPIOs for ILI9341 + SERCOM SPI interface
void GPIO_init() {

    /* 
     * PA05 -> SCK  SPI (SERCOM0)
     * PA06 -> CS  ILI9341 SPI (SERCOM0)
     * PA07 -> MOSI SPI (SERCOM0)
     * PA08 -> RESET (ILI9341) SPI (SERCOM0)
     * PA09 -> DC    (Data/Command) SPI (SERCOM0)
     * PA10 -> XPT4026 CS SPI (SERCOM0)
     * PA17 -> A767E USART TX (SERCOM3)
     */
    PORT_REGS->GROUP[0].PORT_DIRSET = PORT_PA05 | PORT_PA06 | PORT_PA07 | PORT_PA08 | PORT_PA09 | PORT_PA10 | PORT_PA17;
    /* 
     * PB09 -> SCK BME680 SPI (SERCOM4)
     * PB10 -> CS BME680 SPI (SERCOM4)
     * PB11 -> MOSI BME680 SPI (SERCOM4)
     */
    PORT_REGS->GROUP[1].PORT_DIRSET = PORT_PB09 | PORT_PB10 | PORT_PB11;
     /* 
     * PA04 -> MISO SPI (SERCOM0)
     * PA18 -> A767E USART RX (SERCOM3)
     */
    PORT_REGS->GROUP[0].PORT_DIRCLR = PORT_PA04 | PORT_PA18;
    /* 
     * PB08 -> MISO BME680 SPI (SERCOM4)
     */
    PORT_REGS->GROUP[1].PORT_DIRCLR = PORT_PB08;
    
    /* --- Set pins as output for I2C control lines ---
     * PA12 -> SDA I2C (SERCOM2) PAD0
     * PA13 -> SCL I2C (SERCOM2) PAD1
     * no need to declarate
     */   
    PORT_REGS->GROUP[0].PORT_PINCFG[4] = PORT_PINCFG_PMUXEN_Msk; // PA04 SERCOM0 SPI PAD0 MISO
    PORT_REGS->GROUP[0].PORT_PINCFG[5] = PORT_PINCFG_PMUXEN_Msk; // PA05 SERCOM0 SPI PAD1 SCK
    PORT_REGS->GROUP[0].PORT_PINCFG[7] = PORT_PINCFG_PMUXEN_Msk; // PA07 SERCOM0 SPI PAD3 MOSI
    PORT_REGS->GROUP[0].PORT_PINCFG[12] = PORT_PINCFG_PMUXEN_Msk; // PA12 SERCOM2 I2C PAD0
    PORT_REGS->GROUP[0].PORT_PINCFG[13] = PORT_PINCFG_PMUXEN_Msk; // PA13 SERCOM2 I2C PAD1   
    PORT_REGS->GROUP[0].PORT_PINCFG[17] = PORT_PINCFG_PMUXEN_Msk; // PA17 SERCOM3 PAD0 USART TX    
    PORT_REGS->GROUP[0].PORT_PINCFG[18] = PORT_PINCFG_PMUXEN_Msk; // PA18 SERCOM3 PAD2 USART RX
    
    PORT_REGS->GROUP[1].PORT_PINCFG[8] = PORT_PINCFG_PMUXEN_Msk; // PB08 SERCOM4 SPI PAD0 MISO
    PORT_REGS->GROUP[1].PORT_PINCFG[9] = PORT_PINCFG_PMUXEN_Msk; // PB09 SERCOM4 SPI PAD1 SCK
    PORT_REGS->GROUP[1].PORT_PINCFG[11] = PORT_PINCFG_PMUXEN_Msk; // PB11 SERCOM4 SPI PAD3 MOSI


    /* --- Configure PMUX (peripheral multiplexing) ---
     * PMUX pairs pins as even+odd: 0+1, 2+3, 4+5, ...
     * Function D = SERCOM (SPI in this case)
     */
    PORT_REGS->GROUP[0].PORT_PMUX[4 >> 1] = PORT_PMUX_PMUXE_D | PORT_PMUX_PMUXO_D; // PA04 even, PA05 (odd) SERCOM0 function D
    PORT_REGS->GROUP[0].PORT_PMUX[6 >> 1] = /*PORT_PMUX_PMUXE_D |*/ PORT_PMUX_PMUXO_D; // PA06 even, PA07 odd //uncomment if need to use HW SS (for single device) SERCOM0 function D
    
    PORT_REGS->GROUP[1].PORT_PMUX[8 >> 1] = PORT_PMUX_PMUXE_D | PORT_PMUX_PMUXO_D; // PB08 even, PB09 (odd) SERCOM4 function D
    PORT_REGS->GROUP[1].PORT_PMUX[10 >> 1] = /*PORT_PMUX_PMUXE_D |*/ PORT_PMUX_PMUXO_D; // PB10 even, PB11 odd //uncomment if need to use HW SS (for single device) SERCOM4 function D
    
    /* --- Configure PMUX (peripheral multiplexing) ---
     * PMUX pairs pins as even+odd: 12+13, ...
     * Function D = SERCOM (I2C in this case)
     */
    PORT_REGS->GROUP[0].PORT_PMUX[12 >> 1] = PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C; // PA12 even, PA13 odd (use SERCOM2 pins)
    
    PORT_REGS->GROUP[0].PORT_PMUX[17 >> 1] =  PORT_PMUX_PMUXO_D; //only odd pin (17) uses d function (sercom 3)
    PORT_REGS->GROUP[0].PORT_PMUX[18 >> 1] =  PORT_PMUX_PMUXE_D; //only odd pin (18) uses d function (sercom 3)
    
}

