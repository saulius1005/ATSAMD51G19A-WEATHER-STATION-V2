#include "settings.h"
#include "keyboardVar.h"
#include "imageVar.h"


void draw_keyboard(keyboard_t action){
    LCD_Transfer_t t;

    t.pixel_count = KEYBOARD_PX_COUNT;

    if (action == OPEN) { //draw image
        t.is_solid_color = 0;
        t.source.image_data = keypad_240x130;
    } else { //fill keyboard screen part with background color
        t.is_solid_color = 1;
        t.source.color_val = __builtin_bswap16(keyboard.background_color);
    }
    SPI_DMA_LCD_send_area(&t, KEYBOARD_X0, KEYBOARD_Y0, KEYBOARD_X1, KEYBOARD_Y1); //draw keyboard or fill its place with background
}

void button_toggle(Source_data *btn)
{
    btn->action ^= 1;
    draw_keyboard(btn->action);
}

void source(Source_data *btn){
    
    Read_XPT2046.X = XPT2046_Read(XPT_CMD_X);
    Read_XPT2046.Y = XPT2046_Read(XPT_CMD_Y); 
    Read_XPT2046.Z1 = XPT2046_Read(XPT_CMD_Z1);
    Read_XPT2046.Z2 = XPT2046_Read(XPT_CMD_Z2);
    
    bool is_touched = (Read_XPT2046.X >= btn->X0) && (Read_XPT2046.X <= btn->X1) && (Read_XPT2046.Y >= btn->Y0) && (Read_XPT2046.Y <= btn->Y1) && (Read_XPT2046.Z1 >  btn->Z0);

    if(is_touched){
        btn->pressed = 1;
    }
    else if(btn->pressed){
        btn->pressed = 0;
        button_toggle(btn);   // trigger on release
    }
    if(btn->action == OPEN){ 
        uint16_t y = 60;
        for(uint8_t i = 0; i<16; i++){
            bool is_selected = (Read_XPT2046.X >= keysMap.keyboard_buttons[i].X0) && (Read_XPT2046.X <= keysMap.keyboard_buttons[i].X1) && (Read_XPT2046.Y >= keysMap.keyboard_buttons[i].Y0) && (Read_XPT2046.Y <= keysMap.keyboard_buttons[i].Y1) && (Read_XPT2046.Z1 >  keysMap.keyboard_buttons[i].Z0);
            if(is_selected){
                draw_formatted_line(130, &y, WHITE, RED, "key: %c", keysMap.keyboard_buttons[i].value);
            }           
        }

    }
}
