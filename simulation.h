#include "types.h"
#ifndef SIMULATION_H
#define SIMULATION_H

// Funzione per simulare l'aggiornamento del volume del serbatoio 
void update_tank_volume(Tank *tank, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, double deltaT, int *volume_reached);

// Funzione per simulare l'aggionramento della concentrazione del serbatoio
void update_tank_concentration(Tank *tank, int num_tanks, Valve *valve, double deltaT);

//Funzione per simulare l'aggionramento della temperatura del serbatoio
void update_tank_temperature(Tank *tanks, int num_tanks, Valve *valve, Heater *heaters, int num_heaters, double deltaT);

//Funzione per stampare i nuovi valori dei serbatoi e valvole 
void print_new_values(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, int t);
#endif // SIMULATION_H