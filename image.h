/* 
 * File:   image.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, vasaris 5, 14.59
 */

#ifndef IMAGE_H
#define	IMAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FULL_SCREEN_IMAGE_X0 0
#define FULL_SCREEN_IMAGE_Y0 0
#define FULL_SCREEN_IMAGE_X1 239
#define FULL_SCREEN_IMAGE_Y1 319
#define FULL_SCREEN_IMAGE_COUNT 76800
    
 extern const uint16_t windmill[76800]; //full screen
 extern const uint16_t sunflower[76800]; //full screen
 extern const uint16_t keypad_digits_240x130[31200]; //240x130 //xy macro in keyboard.h

#ifdef	__cplusplus
}
#endif

#endif	/* IMAGE_H */

