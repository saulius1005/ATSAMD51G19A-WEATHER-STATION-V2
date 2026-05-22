/* 
 * File:   windows.h
 * Author: Saulius
 *
 * Created on Treèiadienis, 2026, balandis 29, 21.09
 */

#ifndef WINDOWS_H
#define	WINDOWS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef enum {
    INIT_WINDOW = 0,
    MAIN_WINDOW,
    TIME_WINDOW,
    SENSOR_WINDOW,
    TOWER_WINDOW,
    SETTINGS_WINDOW,
}Windows_names_t;

typedef struct {
    bool background_updater;
    uint32_t once_per_second_update;
    Windows_names_t Window;
    keyboard_t keyboardAction;
    
} Windows_t;


extern Windows_t Windows;

void UserInterface(Windows_names_t window); //show all windows on the screen


#ifdef	__cplusplus
}
#endif

#endif	/* WINDOWS_H */

