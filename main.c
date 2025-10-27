#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Per la funzione sleep

#include "types.h"
#include "scenario_config.h"   
#include "simulation.h"  

int main() {
    // Inizializzazione serbatoi
    // Inizializza tutti i campi della struct Tank 
    Tank tank1 = { .id = ID, .capacity = CAPACITY, .volume = INITIAL_VOLUME, .temperature = INITIAL_TEMPERATURE, .concentration = INITIAL_CONCENTRATION, .prev_volume = INITIAL_VOLUME, .prev_concentration = INITIAL_CONCENTRATION, .prev_temperature = INITIAL_TEMPERATURE, .target_reached = 0 };
    Tank tank2 = { .id = ID2, .capacity = CAPACITY2, .volume = INITIAL_VOLUME2, .temperature = INITIAL_TEMPERATURE2, .concentration = INITIAL_CONCENTRATION2, .prev_volume = INITIAL_VOLUME2, .prev_concentration = INITIAL_CONCENTRATION2, .prev_temperature = INITIAL_TEMPERATURE2, .target_reached = 0 };
    Tank tanks[] = {tank1, tank2}; // Array di serbatoi
    int num_tanks = sizeof(tanks) / sizeof(tanks[0]);
    // Inizializza i campi "prev_"
    for (int i = 0; i < num_tanks; i++) {
        tanks[i].prev_volume = tanks[i].volume;
        tanks[i].prev_concentration = tanks[i].concentration;
        tanks[i].prev_temperature = tanks[i].temperature;
    }

    // Inizializzazione delle pompe
    Pump pump1 = {PUMP_ID1, PUMP_FROM_TANK1, PUMP_TO_TANK1, PUMP_MAX_FLOW1, PUMP_IS_ON1}; // Pompa in ingresso serbatoio 1.
    Pump pump2 = {PUMP_ID2, PUMP_FROM_TANK2, PUMP_TO_TANK2, PUMP_MAX_FLOW2, PUMP_IS_ON2}; // Pompa in ingresso serbatoio 1.
    Pump pump3 = {PUMP_ID3, PUMP_FROM_TANK3, PUMP_TO_TANK3, PUMP_MAX_FLOW3, PUMP_IS_ON3}; // Pompa di collegamento tra i due serbatoi da 1 a 2.
    Pump pump4 = {PUMP_ID4, PUMP_FROM_TANK4, PUMP_TO_TANK4, PUMP_MAX_FLOW4, PUMP_IS_ON4}; // Pompa di collegamento tra i due serbatoi da 2 a 1.
    Pump pump5 = {PUMP_ID5, PUMP_FROM_TANK5, PUMP_TO_TANK5, PUMP_MAX_FLOW5, PUMP_IS_ON5}; // Pompa di uscita da 1 a esterno.
    Pump pump6 = {PUMP_ID6, PUMP_FROM_TANK6, PUMP_TO_TANK6, PUMP_MAX_FLOW6, PUMP_IS_ON6}; // Pompa di uscita da 2 a esterno.
    Pump pump7 = {PUMP_ID7, PUMP_FROM_TANK7, PUMP_TO_TANK7, PUMP_MAX_FLOW7, PUMP_IS_ON7}; // Pompa in entrata serbatoio 2.
    Pump pump8 = {PUMP_ID8, PUMP_FROM_TANK8, PUMP_TO_TANK8, PUMP_MAX_FLOW8, PUMP_IS_ON8}; // Seconda pompa in ingresso serbatoio 2.
    Pump pumps[] = {pump1, pump2, pump3, pump4, pump5, pump6, pump7, pump8}; // Array di pompe.
    int num_pumps = sizeof(pumps) / sizeof(pumps[0]); // Numero di pompe.

    // Crea una valvola associata a ciascuna pompa (pump-valves) e la inizializza.
    Valve pump_valves[sizeof(pumps) / sizeof(pumps[0])]; // Array di valvole corrispondenti alle pompe.
    for (int i = 0; i < num_pumps; i++) {
        // Inizializziamo la valvola sulla base dei campi della pompa
        pump_valves[i].id = pumps[i].id;
        pump_valves[i].from_tank = pumps[i].from_tank;
        pump_valves[i].to_tank = pumps[i].to_tank;
        pump_valves[i].max_flow = pumps[i].max_flow;
        pump_valves[i].is_on = pumps[i].is_on;
    }

    // Inizializzazione riscaldatori
    Heater heater1 = {HEATER_ID1, HEATER_TANK_ID1, HEATER_POWER1, HEATER_WATT_PER_DEGREE1, HEATER_IS_ON1}; // Heater associato al serbatoio 1
    Heater heater2 = {HEATER_ID2, HEATER_TANK_ID2, HEATER_POWER2, HEATER_WATT_PER_DEGREE2, HEATER_IS_ON2}; // Heater associato al serbatoio 2
    Heater heaters[] = {heater1, heater2}; // Array di heaters.
    int num_heaters = sizeof(heaters) / sizeof(heaters[0]); // Numero di riscaldatori.

    double deltaT = 1.0;     
    int t = 0;
    int volume_reached = 0;
    int concentration_reached = 0;
    int temperature_reached = 0;
    // Itera fino a quando non si raggiungono i valori obiettivo di volume, concentrazione e temperatura, se i controlli sono abilitati.
    while ((VOLUME_ENABLED && volume_reached == 0) || (CONCENTRATION_ENABLED && concentration_reached == 0) || (TEMPERATURE_ENABLED && temperature_reached == 0)) {
        // Sincronizza lo stato delle pump-valves con lo stato delle pompe prima di stampare
        for (int i = 0; i < num_pumps; i++) {
            pump_valves[i].is_on = pumps[i].is_on;
        }
        // Stampa prima degli aggiornamenti
        print_new_values(tanks, num_tanks, pumps, num_pumps, pump_valves, heaters, num_heaters, t);
        
        // Salva i valori precedenti PRIMA di aggiornare
        for (int i = 0; i < num_tanks; i++) {
            tanks[i].prev_volume = tanks[i].volume;
            tanks[i].prev_concentration = tanks[i].concentration;
            tanks[i].prev_temperature = tanks[i].temperature;
        }
        // Aggiornamento volume, concentrazione e temperatura dei serbatoi
        update_tanks_volume(tanks, num_tanks, pumps, num_pumps, deltaT, &volume_reached);
        update_tank_concentration(tanks, num_tanks, pumps, num_pumps, deltaT, &concentration_reached, &volume_reached);
        update_tank_temperature(tanks, num_tanks, pumps, num_pumps, heaters, num_heaters, deltaT, &temperature_reached);
        t++; // Incrementa il contatore del tempo
        sleep(1); // Pausa di 1 secondo tra le iterazioni
    }  
    return 0;
}