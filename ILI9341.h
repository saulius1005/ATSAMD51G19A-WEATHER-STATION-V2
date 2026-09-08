/* 
 * File:   ILI9341.h
 * Author: Saulius
 *
 * Description:
 *   Minimal hardware control definitions for ILI9341 LCD driver.
 *   Provides display geometry constants and fast GPIO macros
 *   for DC (data/command) and hardware reset control.
 *
 * Created on: Friday, 6 February 2026, 20:38
 */

#ifndef ILI9341_H
#define	ILI9341_H

#ifdef	__cplusplus
extern "C" {
#endif

/* --- Display resolution --- */
#define LCD_HEIGHT      320U
#define LCD_WIDTH       240U
#define LCD_TOTAL_PX    (uint32_t)(LCD_HEIGHT * LCD_WIDTH)   // Total pixel count (used for full-frame transfers)


/* --- ILI9341 control pins (direct register access for maximum speed) ---
 * NOTE:
 * These macros assume:
 *   DC  -> PA09
 *   RST -> PA08
 * GPIO must be configured as OUTPUT before usage.
 */
    
/*SS control */
#define ILI9341_CS_LOW()    (PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA06) // CS LOW
#define ILI9341_CS_HIGH()   (PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA06) // CS HIGH

/* Data/Command control */
#define ILI9341_DC_CMD()    (PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA09) // Select command mode (DC = 0)
#define ILI9341_DC_DATA()   (PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA09) // Select data mode (DC = 1)

/* Hardware reset control */
#define ILI9341_RST_LOW()   (PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA08) // Assert hardware reset
#define ILI9341_RST_HIGH()  (PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA08) // Release hardware reset
    
#define CHAR_WIDTH     6
#define CHAR_HEIGHT    8
    

#define ALICE_BLUE  0xefbf
#define AZURE		0xefff
#define BURLYWOOD	0xddb0
#define CORAL		0xfbea
#define CRIMSON		0xd8a7
#define MISTYROSE	0xff1b
#define LIGHT_BLUE	0xaebc
#define DARK_BLUE   0x000b
#define BLACK       0x0000
#define WHITE       0xFFFF
#define LIGHT_GRAY  0x7BEF
#define DARK_GRAY   0x2945
#define DARK_GREEN	0x0320
#define RED         0xF800
#define GREEN       0x07E0
#define BLUE        0x001F
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define ORANGE      0xFD20
#define BROWN       0xA145
#define PURPLE      0x780F
#define NAVY        0x000F
#define TEAL        0x0410
    
typedef struct {  //for draw_colored_line text coloring function
    const char *text; //text array
    uint16_t fg; //text color
    uint16_t bg; //background color
} color_segment_t;

void ILI9341_init_simple_32b();// Basic LCD initialization using SPI transfers

void ILI9341_fill_color_CPU(uint16_t color);// Fill entire display with color using CPU-driven SPI transfers

void ILI9341_draw_image_CPU(const uint16_t *fb);// Draw framebuffer using CPU-only SPI transfers

void ILI9341_fill_color_32b_DMA(uint16_t color);// Fill entire display with color using DMA-assisted SPI transfers

void ILI9341_draw_framebuffer_DMA(const uint16_t *fb);// Draw framebuffer using DMA-assisted SPI transfers

void ili9341_fill_screen(uint16_t color); //Fill screen with color

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

void ili9341_draw_char(int x, int y, char c, uint16_t fg, uint16_t bg);

void ili9341_draw_text(int x, int y, const char *str, uint16_t fg, uint16_t bg);

void ili9341_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint8_t filled);

void ILI9341_draw_keyboard(uint16_t *fb, uint16_t x0, uint16_t y0, uint8_t state, uint16_t color);

void ili9341_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void ili9341_CMD(uint32_t cmd, uint8_t length);

void ili9341_DATA(uint32_t data, uint8_t length);

void ILI9341_draw_image_DMA(const uint16_t *fb);

void ILI9341_fill_ALL_color_DMA(uint16_t color); //fill all of screen with selected color using DMA

void ILI9341_fill_PART_color_DMA(uint16_t color, uint16_t X0, uint16_t X1, uint16_t Y0,uint16_t Y1); //fill part of screen with selected color using DMA

void ILI9341_draw_formatted_line(uint16_t x, uint16_t *y, uint16_t fg, uint16_t bg, const char *fmt, ...);

void ILI9341_draw_colored_line(uint16_t x, uint16_t *y, color_segment_t *segments, size_t count);


#ifdef	__cplusplus
}
#endif

#endif	/* ILI9341_H */
