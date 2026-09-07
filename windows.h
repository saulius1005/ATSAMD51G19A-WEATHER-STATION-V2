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
    LOCATION_WINDOW,
    TOWER_WINDOW,
    NETWORK_WINDOW,
}Windows_names_t;

typedef struct {
    bool background_updater;
    uint32_t once_per_second_update;
    Windows_names_t Window;   
} Windows_t;

typedef struct { //for location data cheange
    int32_t max_value;
    int32_t *target;
} param_limit_t;

typedef struct {
    const char *name;
    uint16_t x;
} param_text_t;

typedef struct {
    char *edit_buffer;
    char *target_buffer;
    uint16_t eeprom_offset;
    uint8_t size;
    char name[30];
} GSM_NET_param_t;


extern Windows_t Windows;
extern param_limit_t limits[];
extern param_text_t location_param_names[];
extern GSM_NET_param_t win_params[];

void UserInterface(Windows_names_t window); //show all windows on the screen


#ifdef	__cplusplus
}
#endif

#endif	/* WINDOWS_H */

