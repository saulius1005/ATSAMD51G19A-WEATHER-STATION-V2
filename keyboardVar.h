/* 
 * File:   keyboardVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, vasaris 13, 15.37
 */

#ifndef KEYBOARDVAR_H
#define	KEYBOARDVAR_H

#include "keyboard.h"


#ifdef	__cplusplus
extern "C" {
#endif

    KeyBoard_data keyboard = {
        .background_color = RED,
    };
    
    keys_data keysMap = {
    .keyboard_buttons = {
        {1150, 1920, 385, 672, 100,  key0, 0, 0, '0'},//0
        {200, 1024, 736, 1024, 100,  key1, 0, 1, '1'},//1
        {1150, 1920, 736, 1024, 100,  key2, 0, 2, '2'},//2
        {2100, 2850, 736, 1024, 100,  key3, 0, 3, '3'},//3

        {200, 1024, 1120, 1408, 100, key4, 0, 4, '4'},//4
        {1150, 1920, 1120, 1408, 100, key5, 0, 5, '5'},//5
        {2100, 2850,  1120, 1408, 100, key6, 0, 6, '6'},//6
        {200, 1024, 1472, 1792, 100, key7, 0, 7, '7'},//7

        {1150, 1920, 1472, 1792, 100, key8, 0, 8, '8'},//8
        {2100, 2850, 1472, 1792, 100, key9,0, 9, '9'},//9
        {3040, 3850, 385, 672, 100, keyback,0, 10, '>'},//>
        {3040, 3850, 736, 1024, 100, keyok,0, 11, '<'},//<

        {200,  1024, 385, 672, 100, keydelete,0, 12, 'x'},//x
        {3040, 3850, 1120, 1408, 100, keyplus,0, 13, '+'},//+
        {3040, 3850, 1472, 1792, 100, keyminus,0, 14, '-'},//-
        {2100, 2850, 385, 672, 100, keydot,0, 15, ','}//,
    }
};


#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARDVAR_H */

