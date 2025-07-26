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

void update_tank_concentration(Tank *tank, Valve *valve, double deltaT) {
    if (tank == NULL || valve == NULL) {
        return;
    }
    double nuovaConcentrazione = tank->concentration * tank->volume + valve->max_flow * deltaT; // Esempio di incremento
    if (nuovaConcentrazione <= MIN_CONCENTRATION){
        nuovaConcentrazione = MIN_CONCENTRATION; // Limita la concentrazione massima o minima
        valve->is_on = 0; // Spegne la valvola se la concentrazione è fuori dai limiti
        tank->concentration = nuovaConcentrazione; // Aggiorna la concentrazione del serbatoio
    } else if (nuovaConcentrazione >= MAX_CONCENTRATION) {
        valve->is_on = 0; // Mantiene la valvola accesa se la concentrazione è nei limiti
        tank->concentration = MAX_CONCENTRATION; // Limita la concentrazione massima
    } else {
        valve->is_on = 1; // Mantiene la valvola accesa se la concentrazione è nei limiti
    }
    tank->concentration = nuovaConcentrazione;
}