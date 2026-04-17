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
} keyboard_t;

typedef struct {
    uint16_t background_color;   
} KeyBoard_data;

typedef enum { //windows names where presed keypad will pop up
    BTN_0,
    BTN_1,
    BTN_2
} ButtonID;

typedef struct {
    uint16_t X0;
    uint16_t X1;
    uint16_t Y0;
    uint16_t Y1;
    uint16_t Z0;
    keyboard_t action;
    ButtonID id;     
    uint8_t pressed;
} Source_data;

typedef enum {
    key0,
    key1,
    key2,
    key3,
    key4,
    key5,
    key6,
    key7,        
    key8,
    key9,
    keyback,
    keyok,
    keydelete,
    keyplus,
    keyminus,
    keydot
} KeyID;

typedef struct {
    uint16_t X0;
    uint16_t X1;
    uint16_t Y0;
    uint16_t Y1;
    uint16_t Z0;
    KeyID id;     
    uint8_t pressed;
    char value;
} key_data;


typedef struct {
    key_data keyboard_buttons[KEY_COUNT];
} keys_data;


extern KeyBoard_data keyboard;
extern keys_data keysMap;




#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

