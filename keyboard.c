#include "settings.h"
#include "keyboardVar.h"
#include "imageVar.h"


void draw_keyboard(){
    LCD_Transfer_t t;
    t.pixel_count = KEYBOARD_PX_COUNT;    
    static bool is_open = false;

    if (keyboard.status == OPEN && !is_open) { //draw image
        t.is_solid_color = 0;
        t.source.image_data = keypad_240x130;
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