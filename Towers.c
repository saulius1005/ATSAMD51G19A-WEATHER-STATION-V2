#include "settings.h"
#include "TowersVar.h"

void Towers_init(){
    for (uint8_t i = 0; i < TOWER_COUNT; i++){
        towers[i].state = SEND;
        towers[i].id = i + 1;
    }
}

void Tower_COM_sequence(uint8_t id){
    switch(towers[id].state){
        case SEND:
            RS485_printf("test to id %02x\r\n",id);
            towers[id].state = WAIT_RESPOND;
            TCC1_ON(2000000); //send test every 2 seconds?
            //start reading using DMA
        break;
        case WAIT_RESPOND:
            if(TCC1_timeout){ //after timeout 
                //stop reading using DMA
                TCC1_timeout = false; //reset timer flag
                towers[id].state = PROCESS;
            }           
        break;
        case PROCESS:
            //Do process if data received- ALL ok, move on
            //if not continue next 3 times? if no respond exp. id 2 set fault
        break;
            
    }
}

void Tower_COM(){
    static uint8_t switcher = 0;

    if(A7672E_init.status != WORK)//wait all GSM module initialization and start data sending after that (when main window is start showing up
        return;

    if(towers[switcher].state == PROCESS){// if data processed send request to next tower
        switcher = (switcher + 1) % TOWER_COUNT;
        towers[switcher].state = SEND;
    }
    else {//if tower state is SEND or WAIT
        Tower_COM_sequence(switcher);
    }
}