#include "settings.h"

void GPIO_init() {

    /* PORT A Outputs: (first +) (* meaning pin settings are in external files)
     * 
     * PA03 -> XPT4026 CS SPI OUT+*
     * PA05 -> SCK SPI OUT (SERCOM0) P1+++
     * PA07 -> MOSI SPI OUT(SERCOM0) P3+++
     * PA08 -> ILI9341 DC SPI OUT+*
     * PA09 -> ILI9341 RESET  SPI OUT+*
     * PA10 -> ILI9341 CS SPI OUT+*
     * 
     * PA12 -> A767E USART TX OUT (SERCOM2) P0+++
     * PA13 -> A7672E MODULE Enable+*
     * 
     * PA16 -> Towers RS485 TX LED OUT+*
     * PA17 -> Towers RS485 TX OUT (SERCOM3) P0+++
     * PA18 -> Towers RS485 TE OUT (SERCOM3) P2+++
     * PA20 -> Towers RS485 RX LED OUT+*
     * 
     * PA22 -> Sensors RS485 TX LED OUT+*
     * PA23 -> Sensors RS485 TX (SERCOM5) P0+++
     * PA24 -> Sensors RS485 TE (SERCOM5) P2+++
     */
    PORT_REGS->GROUP[0].PORT_DIRSET = PORT_PA03 | PORT_PA05 | PORT_PA07 | PORT_PA08 | PORT_PA09 | PORT_PA10 | PORT_PA12 | PORT_PA13 | PORT_PA16  | PORT_PA17 | PORT_PA18 | PORT_PA20 | PORT_PA22 | PORT_PA23 | PORT_PA24;
    
    /* PORT B Outputs:
     * 
     * PB09 -> LCD backlight LED Out+*
     * PB10 -> Server LED OUT+*
     * PB23 -> Sensors RS485 RX LED OUT+*
     * 
     */
    PORT_REGS->GROUP[1].PORT_DIRSET = PORT_PB09 | PORT_PB10 | PORT_PB23 ;
    
     /*PORT A Inputs:
     *  
     * PA04 -> MISO SPI IN (SERCOM0) P0+++
     * PA14 -> A767E USART RX IN (SERCOM2) P2+++  
     * PA19 -> Towers RS485 RX IN (SERCOM3) P3+++
     * PA25 -> Sensors RS485 RX (SERCOM5) P3+++
     * 
     */
    PORT_REGS->GROUP[0].PORT_DIRCLR =   PORT_PA04 | PORT_PA14 | PORT_PA19 | PORT_PA25;
    
    
    // PORT A Multiplexer enabling for each pin (SERCOM 0, 2, 3, 5) to enable pin function selection (second +)
    
    PORT_REGS->GROUP[0].PORT_PINCFG[4] = PORT_PINCFG_PMUXEN_Msk; // PA04 SERCOM0 SPI P0 MISO
    PORT_REGS->GROUP[0].PORT_PINCFG[5] = PORT_PINCFG_PMUXEN_Msk; // PA05 SERCOM0 SPI P1 SCK
    PORT_REGS->GROUP[0].PORT_PINCFG[7] = PORT_PINCFG_PMUXEN_Msk; // PA07 SERCOM0 SPI P3 MOSI
    
    PORT_REGS->GROUP[0].PORT_PINCFG[12] = PORT_PINCFG_PMUXEN_Msk; // PA12 SERCOM2 USART P0 TX
    PORT_REGS->GROUP[0].PORT_PINCFG[14] = PORT_PINCFG_PMUXEN_Msk; // PA14 SERCOM2 USART P2 TE 
    
    PORT_REGS->GROUP[0].PORT_PINCFG[17] = PORT_PINCFG_PMUXEN_Msk; // PA17 SERCOM3 RS485 P0 TX    
    PORT_REGS->GROUP[0].PORT_PINCFG[18] = PORT_PINCFG_PMUXEN_Msk; // PA18 SERCOM3 RS485 P2 TE    
    PORT_REGS->GROUP[0].PORT_PINCFG[19] = PORT_PINCFG_PMUXEN_Msk; // PA19 SERCOM3 RS485 P3 RX       
    
    PORT_REGS->GROUP[0].PORT_PINCFG[23] = PORT_PINCFG_PMUXEN_Msk; // PA23 SERCOM5 RS485 P0 TX
    PORT_REGS->GROUP[0].PORT_PINCFG[24] = PORT_PINCFG_PMUXEN_Msk; // PA24 SERCOM5 RS485 P2 TE 
    PORT_REGS->GROUP[0].PORT_PINCFG[25] = PORT_PINCFG_PMUXEN_Msk; // PA25 SERCOM5 RS485 P3 RX
    
    // PORT A functions selection for each pin (third +)

    PORT_REGS->GROUP[0].PORT_PMUX[4 >> 1] = PORT_PMUX_PMUXE_D | PORT_PMUX_PMUXO_D; // PA04, PA05 Function D pin control takes SERCOM0
    PORT_REGS->GROUP[0].PORT_PMUX[7 >> 1] = PORT_PMUX_PMUXO_D; // PA07 Function D pin control takes SERCOM0
    
    PORT_REGS->GROUP[0].PORT_PMUX[12 >> 1] =  PORT_PMUX_PMUXE_C; //PA12 Function C pin control takes SERCOM2
    PORT_REGS->GROUP[0].PORT_PMUX[14 >> 1] =  PORT_PMUX_PMUXE_C; //PA14 Function C pin control takes SERCOM2  
    
    PORT_REGS->GROUP[0].PORT_PMUX[17 >> 1] = PORT_PMUX_PMUXO_D; //PA17 Function D pin control takes SERCOM3
    PORT_REGS->GROUP[0].PORT_PMUX[18 >> 1] = PORT_PMUX_PMUXE_D | PORT_PMUX_PMUXO_D; //PA18, PA19 Function D pin control takes SERCOM3

    PORT_REGS->GROUP[0].PORT_PMUX[23 >> 1] = PORT_PMUX_PMUXO_D; //PA23 Function D pin control takes SERCOM5
    PORT_REGS->GROUP[0].PORT_PMUX[24 >> 1] = PORT_PMUX_PMUXE_D | PORT_PMUX_PMUXO_D; //PA24, PA25 Function C pin control takes SERCOM5
         
}

