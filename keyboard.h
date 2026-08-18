/* 
 * File:   keyboard.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, vasaris 13, 15.37
 */

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define KEY_COUNT 16
#define KEYBOARD_X0 0
#define KEYBOARD_Y0 190
#define KEYBOARD_X1 239
#define KEYBOARD_Y1 319
#define KEYBOARD_PX_COUNT 31200
    
typedef enum {
    CLOSE,    //0
    OPEN       //1
} keyboard_status_t;

typedef struct {
    uint16_t background_color;   
} KeyBoard_data;

typedef struct {
    uint16_t X0;
    uint16_t X1;
    uint16_t Y0;
    uint16_t Y1;
    uint16_t Z0;
    uint8_t digit;
    char value;
} key_data;

typedef struct {
    keyboard_status_t status;
    uint16_t background_color;
    key_data keyboard_buttons[KEY_COUNT];
}keyboard_t;

extern keyboard_t keyboard;

void draw_keyboard();


#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

