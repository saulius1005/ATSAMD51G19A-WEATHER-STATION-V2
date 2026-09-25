#include "settings.h"
#include "ili9341Var.h"
#include "font.h"


void ili9341_CMD(uint32_t cmd, uint8_t length){
    ILI9341_DC_CMD();
    ILI9341_CS_LOW();
    SPI0_Transfer_32b_HW(cmd,length);
    ILI9341_CS_HIGH();
}

void ili9341_DATA(uint32_t data, uint8_t length){
    ILI9341_DC_DATA();
    ILI9341_CS_LOW();
    SPI0_Transfer_32b_HW(data,length);
    ILI9341_CS_HIGH();
}

// Minimal ILI9341 initialization using 32-bit SPI transfer helper
void ILI9341_init(){
 
    ILI9341_RST_LOW();
    TC0_ON(5000);
    while(!TC0_timeout) TC0_CHECKER();

    ILI9341_RST_HIGH();
    TC0_ON(10000);
    while(!TC0_timeout) TC0_CHECKER();

    ili9341_CMD(0x11,1); // SLPOUT
    TC0_ON(10000);
    while(!TC0_timeout)TC0_CHECKER();

    ili9341_CMD(0x3A,1); // COLMOD
    ili9341_DATA(0x55,1); // 16-bit color
    ili9341_CMD(0x36,1); // MADCTL  
    ili9341_DATA(0x48,1); // MX | MV | RGB
    ili9341_CMD(0x29,1); // DISPON
    
     A7672E_LCD_BCKL_ON(); //turn on lcd backlight      
}

void ILI9341_draw_image_DMA(const uint16_t *fb){
    LCD_Transfer_t t;
    t.pixel_count = FULL_SCREEN_IMAGE_COUNT;
    t.is_solid_color = 0;
    t.source.image_data = fb;
    DMA_SPI_LCD_send_area(&t, FULL_SCREEN_IMAGE_X0, FULL_SCREEN_IMAGE_Y0, FULL_SCREEN_IMAGE_X1, FULL_SCREEN_IMAGE_Y1); 
}

void ILI9341_fill_ALL_color_DMA(uint16_t color){
    LCD_Transfer_t t;
    t.pixel_count = FULL_SCREEN_IMAGE_COUNT;
    t.is_solid_color = 1;
    t.source.color_val = __builtin_bswap16(color);
    DMA_SPI_LCD_send_area(&t, FULL_SCREEN_IMAGE_X0, FULL_SCREEN_IMAGE_Y0, FULL_SCREEN_IMAGE_X1, FULL_SCREEN_IMAGE_Y1); 
}

void ILI9341_fill_PART_color_DMA(uint16_t color, uint16_t X0, uint16_t X1, uint16_t Y0,uint16_t Y1){
    LCD_Transfer_t t;
    t.pixel_count = FULL_SCREEN_IMAGE_COUNT;
    t.is_solid_color = 1;
    t.source.color_val = __builtin_bswap16(color);
    DMA_SPI_LCD_send_area(&t, X0, Y0, X1, Y1); 
}

void ili9341_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        
    ili9341_CMD(0x2A, 1);  
    ili9341_DATA(((uint32_t)x0<<16) | x1, 4);
    ili9341_CMD(0x2B, 1);   
    ili9341_DATA(((uint32_t)y0<<16) | y1, 4);   
    ili9341_CMD(0x2C, 1);
     
 }

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
	 if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;  
	 ili9341_set_address_window(x, y, x, y);
     
     ili9341_DATA(color,2);
 }

void ili9341_draw_char(int x, int y, char c, uint16_t fg, uint16_t bg) { //new function
    uint8_t idx;

    if (c >= 32 && c <= 127) {
        idx = c - 32;
    } 
    else if (c == 176) {
        idx = c - 80;
    } 
    else if (c >= 192) {
        idx = c - 95;
    } 
    else {
        idx = 0; // space
    }
	const uint8_t *chr_data = font5x7[idx];
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = chr_data[i];
        for (uint8_t j = 0; j < 7; j++) {
            ili9341_draw_pixel(x + i, y + j, (line & (1 << j)) ? fg : bg);
        }
    }
 }

void ili9341_draw_text(int x, int y, const char *str, uint16_t fg, uint16_t bg) {
	 while (*str) {
		 ili9341_draw_char(x, y, *str++, fg, bg);
		 x += 6; // Character width + spacing
	 }
 }

void ili9341_draw_text_wrap(int x, int y, const char *str, uint16_t fg, uint16_t bg) {
    int start_x = x;

    while (*str) {
        if (*str == '\r') { //go to line beginning
            x = start_x;
            str++;
            continue;
        }

        if (*str == '\n') { //go to new line
            x = start_x;
            y += 12;
            str++;
            continue;
        }
        if (x + 6 > LCD_WIDTH) { //fit to screen
            x = start_x;
            y += 12;
        }

        ili9341_draw_char(x, y, *str++, fg, bg);
        x += 6;
    }
}


void normalize_newlines(char *str) {
    char *dst = str;

    while (*str) {
        // CRLF ? vienas \n
        if (*str == '\r' && *(str + 1) == '\n') {
            *dst++ = '\n';
            str += 2;
        }
        // pavienis CR ? ignoruojam arba laikom kaip \n
        else if (*str == '\r') {
            // pasirink:
            *dst++ = '\n';  // jei nori rodyti kaip naujà eilutæ
            str++;
        }
        else {
            *dst++ = *str++;
        }
    }

    *dst = '\0';
}

void ILI9341_draw_formatted_line(uint16_t x, uint16_t *y, uint16_t fg, uint16_t bg, const char *fmt, ...) {
    char line[256];
    va_list args;
    
    va_start(args, fmt);
    vsnprintf(line, sizeof(line), fmt, args);
    va_end(args);
    normalize_newlines(line);
    ili9341_draw_text_wrap(x, *y, line, fg, bg);

    uint16_t chars_per_line = LCD_WIDTH / 6;
    uint16_t lines = 1;
    uint16_t count = 0;

    for (char *p = line; *p; p++) {
        if (*p == '\r') {
            count = 0; //back to line beginning
        }
        else if (*p == '\n') { //start new line
            lines++;
            count = 0;
        } 
        else {
            count++;
            if (count >= chars_per_line) {
                lines++;
                count = 0;
            }
        }
    }

    *y += lines * 12;
}

void ILI9341_draw_colored_line(uint16_t x, uint16_t *y, color_segment_t *segments, size_t count) {
    uint16_t chars_per_line = LCD_WIDTH / 6;
    uint16_t lines = 1;
    uint16_t cursor_x = x;

    for (size_t i = 0; i < count; i++) {
        char *s = (char*)segments[i].text;
        uint16_t fg = segments[i].fg;
        uint16_t bg = segments[i].bg;

        while (*s) {
            if (*s == '\n') {
                lines++;
                cursor_x = x;
                s++;
                continue;
            }
            ili9341_draw_char(cursor_x, *y + (lines-1)*12, *s, fg, bg);
            cursor_x += 6; // plotis vieno simbolio
            if (cursor_x - x >= LCD_WIDTH) { // wrap
                lines++;
                cursor_x = x;
            }
            s++;
        }
    }

    *y += lines * 12;
}

void ili9341_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint8_t filled){
    if (w == 0 || h == 0) return;

    // === Filled rectangle ===
    if (filled) {
        for (uint16_t j = 0; j < h; j++) {
            for (uint16_t i = 0; i < w; i++) {
                ili9341_draw_pixel(x + i, y + j, color);
            }
        }
        return;
    }

    // === Outline rectangle ===

    // virðus + apaèia
    for (uint16_t i = 0; i < w; i++) {
        ili9341_draw_pixel(x + i, y, color);
        ili9341_draw_pixel(x + i, y + h - 1, color);
    }

    // kairë + deðinë
    for (uint16_t j = 0; j < h; j++) {
        ili9341_draw_pixel(x, y + j, color);
        ili9341_draw_pixel(x + w - 1, y + j, color);
    }
}

void ili9341_sleep(){
    if (A7672E_init.status != WORK)//if not WORK mode 
        return; //skip further code
    
    if(!screen_sleep.sleep){ //if not sleeping check when its time to sleep 
        if ( Periodic_Checker_Devices.period_counter >= (screen_sleep.start_at + LCD_GO_SLEEP_AFTER) ) {
            screen_sleep.sleep = true;
            ili9341_CMD(0x28, 1); // DISPOFF      
            A7672E_LCD_BCKL_OFF(); //Turn Off lcd backlight
            /// ZZZ... ZZZ... ZZZ...
        }        
    }
    else{ //and if sleeping check if need to wake up
        if(XPT2046_switch(64, 4000, 64, 4000)){//wakey wakey
            screen_sleep.start_at = Periodic_Checker_Devices.period_counter;      
            if(screen_sleep.sleep){
                Windows.once_per_second_update = 0;
                Windows.background_updater = false;
                ili9341_CMD(0x29,1); //DISPON
                A7672E_LCD_BCKL_ON();// backlight on
            }
            screen_sleep.sleep = false; //GO to WORK !
        }        
    }
}

