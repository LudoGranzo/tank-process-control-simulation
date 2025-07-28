#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"
#include "types.h"
#include "constants.h"

//Funzione per aggiornare il volume del serbatoio 
void update_tank_volume(Tank *tanks, int num_tanks, Valve *valve, double deltaT) {      
    for(int i = 0; i < num_tanks; i++) {
        if(tanks[i].volume != TANK_SCOPE)
            valve->is_on = 1; 
        tanks[i].volume = tanks[i].volume + valve->max_flow * deltaT - EVAP_COEFF * tanks[i].volume;  //manca la portata in uscita
        if (tanks[i].volume >TANK_SCOPE){
            tanks[i].volume = TANK_SCOPE;
            valve->is_on = 0;           // Spegne la valvola 
        } else if (tanks[i].volume < 0) {   
            tanks[i].volume = 0;
            valve->is_on = 0;           // Spegne la valvola
        }           
    }
}

void update_tank_concentration(Tank *tanks,int num_tanks, Valve *valve, double deltaT) {
    if (!valve->is_on) //guarda se la valvola è spenta
        return;
    Tank *from = NULL;
    Tank *to = NULL;
    for(int i = 0; i < num_tanks; i++){         //si cercano i due serbatoi connessi alla valvola: il from e il to
        if (tanks[i].id == valve->from_tank) 
            from = &tanks[i];
        if (tanks[i].id == valve->to_tank)
            to = &tanks[i];
    }
    if (from == NULL || to == NULL)             //se non si trovano serbatoi collegati si esce dalla funzione
        return;
   
    double volume_in = valve->max_flow * deltaT; //quanto volume arriva in deltaT tempo
    if (volume_in > from->volume)
    volume_in = from->volume; 
    double V_old = to->volume;
    double V_new = V_old + volume_in - EVAP_COEFF * V_old; //calcolo nuovo volume del serbatoio di destinazione
    if (V_new <= 0) //se il volume è negativo (non realistico)
    return;
    double C_old = to->concentration;
    double C_in = from->concentration;
    double C_new = (C_old * V_old + C_in * volume_in) / V_new; //formula miscelazione perfetta
    to->concentration = C_new;
    

}