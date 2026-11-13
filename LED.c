#include "settings.h"

void all_LED_init(){ //set all led to be off after start up
    A7672E_SERVER_LED_OFF(); //server led off
    
    T_TX_LED_OFF(); //tower tx led off
    T_RX_LED_OFF(); //tower rx led off
    
    S_TX_LED_OFF(); //sensor tx led off
    S_RX_LED_OFF(); //sensor rx led off
    
}
