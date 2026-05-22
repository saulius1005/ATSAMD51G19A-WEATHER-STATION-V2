/* 
 * File:   XPT2046.h
 * Author: Saulius
 *
 * Created on Antradienis, 2026, vasaris 10, 18.35
 */

#ifndef XPT2046_H
#define	XPT2046_H

#ifdef	__cplusplus
extern "C" {
#endif

#define XPT_CMD_X 0xD0 //0xD8- 8bit
#define XPT_CMD_Y 0x90 //0x98- 8bit
#define XPT_CMD_Z1 0xB0
#define XPT_CMD_Z2 0xC0
#define XPT_PRES_STRENGTH_LVL 100 //how hard need to press
    
#define XPT2046_CS_LOW()    (PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA10) // CS LOW
#define XPT2046_CS_HIGH()   (PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA10) // CS HIGH
        
typedef struct {
    uint16_t X;
    uint16_t Y;
    uint16_t Z1;
    uint16_t Z2;
    uint8_t step;
    A7672states_t state;
    bool pressed;
            
} TuchScreen;

extern TuchScreen Read_XPT2046;

uint16_t XPT2046_Read(uint32_t cmd); //read touch screen data X,Y,Z coordinates
bool XPT2046_switch(uint16_t X0, uint16_t X1, uint16_t Y0, uint16_t Y1); //return true if pressed in this area

void XPT2046_Read_All();


#ifdef	__cplusplus
}
#endif

#endif	/* XPT2046_H */

