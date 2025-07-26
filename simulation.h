#include "types.h"
#ifndef SIMULATION_H
#define SIMULATION_H

// Funzione per simulare l'aggiornamento del volume del serbatoio 
void update_tank_volume(Tank *tank, int num_tanks, Valve *valve, double deltaT);

// Funzione per simulare l'aggionramento della temperatura del serbatoio
void update_tank_concentration(Tank *tank, Valve *valve, double deltaT);

//Funzione per simulare l'aggionramento della concentrazione del serbatoio

#endif // SIMULATION_H