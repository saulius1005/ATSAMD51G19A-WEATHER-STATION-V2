#include "settings.h"

bool button(uint16_t bcgclr){
    ili9341_draw_rect(0, 0, 48, 20, BLACK, 1); //and buttons frames
            uint16_t y = 6;
            ILI9341_draw_formatted_line(18, &y, WHITE, BLACK,  "<");
}
