#include "settings.h"
#include "keyboardVar.h"
#include "imageVar.h"


void draw_keyboard(){
    LCD_Transfer_t t;
    t.pixel_count = KEYBOARD_PX_COUNT;    
    static bool is_open = false;

    if (keyboard.status == OPEN && !is_open) { //draw image
        t.is_solid_color = 0;
         
        t.source.image_data = keyboard.type == digits ? keypad_digits_240x130 : keypad_letters_240x130;
        SPI_DMA_LCD_send_area(&t, KEYBOARD_X0, KEYBOARD_Y0, KEYBOARD_X1, KEYBOARD_Y1); //draw keyboard or fill its place with background
        is_open = true;
    } 
    else if(keyboard.status == CLOSE && is_open){ //fill keyboard screen part with background color
        t.is_solid_color = 1;
        t.source.color_val = __builtin_bswap16(keyboard.background_color);
        SPI_DMA_LCD_send_area(&t, KEYBOARD_X0, KEYBOARD_Y0, KEYBOARD_X1, KEYBOARD_Y1); //draw keyboard or fill its place with background        
        is_open = false;
    }
}

void Keyboard_SetType(keyboard_type_t type) { //select touch map according to keyboard type and sets key count in keyboard
    keyboard.type = type;

    switch (type) {
        case digits:
            keyboard.keys = keyboard.digits_keyboard_buttons;
            keyboard.key_count = KEY_COUNT_DIGITS_KEYBOARD;
            break;

        case letters:
            keyboard.keys = keyboard.letters_keyboard_buttons;
            keyboard.key_count = KEY_COUNT_LETTERS_KEYBOARD;
            break;

        default:
            keyboard.keys = NULL;
            keyboard.key_count = 0;
            break;
    }
}