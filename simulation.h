#include "types.h"
#ifndef SIMULATION_H
#define SIMULATION_H

// Funzione per simulare l'aggiornamento dei volumi dei serbatoi
void update_tanks_volume(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, double deltaT, int *volume_reached);

// Funzione per simulare l'aggionramento della concentrazione del serbatoio
void update_tank_concentration(Tank *tanks, int num_tanks,Valve *valve, Pump *pumps, int num_pumps, double deltaT, int *concentration_reached, int *volume_reached);

//Funzione per simulare l'aggionramento della temperatura del serbatoio
void update_tank_temperature(Tank *tanks, int num_tanks, Valve *valve,Pump *pumps, int num_pumps, Heater *heaters, int num_heaters, double deltaT, int *temperature_reached);

//Funzione per stampare i nuovi valori dei serbatoi e valvole 
void print_new_values(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, Heater *heaters, int num_heaters, int t);

// Funzione per svuotare i serbatoio
void emptying_tank(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, Heater *heaters, int num_heaters, double deltaT);

#endif // SIMULATION_H