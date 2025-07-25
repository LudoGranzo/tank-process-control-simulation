#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"
#include "types.h"
#include "constants.h"

//Funzione per aggiornare il volume del serbatoio 
void update_tank_volume(Tank *tank, Valve *valve, double deltaT) {
    if (tank == NULL || valve == NULL){
        return;
    }
    double nuovoVolume = tank->volume + valve->max_flow * deltaT - EVAP_COEFF * tank->volume * deltaT;  //manca la portata in uscita
    if (nuovoVolume >TANK_SCOPE){
        nuovoVolume = TANK_SCOPE;
        valve->is_on = 0;           // Spegne la valvola 
    } else if (nuovoVolume < 0) {
        nuovoVolume = 0;
        valve->is_on = 0;           // Spegne la valvola
    }
    tank->volume = nuovoVolume;               
}
