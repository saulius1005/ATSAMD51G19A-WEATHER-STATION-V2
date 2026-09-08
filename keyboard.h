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

#define KEY_COUNT_DIGITS_KEYBOARD 16
#define KEY_COUNT_LETTERS_KEYBOARD 55
#define KEYBOARD_X0 0
#define KEYBOARD_Y0 190
#define KEYBOARD_X1 239
#define KEYBOARD_Y1 319
#define KEYBOARD_PX_COUNT 31200
    
typedef enum {
    CLOSE,    //0
    OPEN,       //1
} keyboard_status_t;

typedef enum {
    digits = 0,
    letters,
    none,
}keyboard_type_t;

typedef struct {
    uint16_t background_color;   
} KeyBoard_data;

typedef struct {
    uint16_t X0;
    uint16_t X1;
    uint16_t Y0;
    uint16_t Y1;
    uint8_t ASCII_value;
} key_data;

typedef struct {
    bool shift;
    keyboard_status_t status;
    keyboard_type_t type;
    uint16_t background_color;
    key_data digits_keyboard_buttons[KEY_COUNT_DIGITS_KEYBOARD];
    key_data letters_keyboard_buttons[KEY_COUNT_LETTERS_KEYBOARD];
}keyboard_t;

extern keyboard_t keyboard;

void draw_keyboard();


#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

