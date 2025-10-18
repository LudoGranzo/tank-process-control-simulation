#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Per la funzione sleep

#include "types.h"
#include "scenario_config.h"   
#include "simulation.h"  

int main() {
    // Esempio di inizializzazione dei serbatoi e della valvola
    Tank tank1 = {ID,CAPACITY, INITIAL_VOLUME, INITIAL_TEMPERATURE, INITIAL_CONCENTRATION};
    Tank tank2 = {ID2, CAPACITY2, INITIAL_VOLUME2, INITIAL_TEMPERATURE2, INITIAL_CONCENTRATION2};    
    Tank tanks[] = {tank1, tank2}; // Array di serbatoi
    int num_tanks = sizeof(tanks) / sizeof(tanks[0]);
    
    // Esempio di inizializzazione della valvola
    Valve valve = {VALVE_ID, VALVE_FROM_TANK, VALVE_TO_TANK, VALVE_MAX_FLOW, VALVE_IS_ON};

    // Esempio di inizializzazione della pompa
    Pump pump1 = {PUMP_ID1, PUMP_FROM_TANK1, PUMP_TO_TANK1, PUMP_MAX_FLOW1, PUMP_IS_ON1};
    Pump pump2 = {PUMP_ID2, PUMP_FROM_TANK2, PUMP_TO_TANK2, PUMP_MAX_FLOW2, PUMP_IS_ON2}; // Pompe in ingresso.
    Pump pump3 = {PUMP_ID3, PUMP_FROM_TANK3, PUMP_TO_TANK3, PUMP_MAX_FLOW3, PUMP_IS_ON3}; // Pompa di collegamento tra i due serbatoi da 1 a 2.
    Pump pump4 = {PUMP_ID4, PUMP_FROM_TANK4, PUMP_TO_TANK4, PUMP_MAX_FLOW4, PUMP_IS_ON4}; // Pompa di collegamento tra i due serbatoi da 2 a 1.
    Pump pump5 = {PUMP_ID5, PUMP_FROM_TANK5, PUMP_TO_TANK5, PUMP_MAX_FLOW5, PUMP_IS_ON5}; // Pompa di uscita da 1 a esterno.
    Pump pump6 = {PUMP_ID6, PUMP_FROM_TANK6, PUMP_TO_TANK6, PUMP_MAX_FLOW6, PUMP_IS_ON6}; // Pompa di uscita da 2 a esterno.
    Pump pump7 = {PUMP_ID7, PUMP_FROM_TANK7, PUMP_TO_TANK7, PUMP_MAX_FLOW7, PUMP_IS_ON7}; // Pompa in entrata serbatoio 2
    Pump pump8 = {PUMP_ID8, PUMP_FROM_TANK8, PUMP_TO_TANK8, PUMP_MAX_FLOW8, PUMP_IS_ON8}; // Seconda pompa in ingresso serbatoio 2
    Pump pumps[] = {pump1, pump2, pump3, pump4, pump5, pump6, pump7, pump8}; // Array di pompe
    int num_pumps = sizeof(pumps) / sizeof(pumps[0]);


    // Esempio di inizializzazione del riscaldatore
    Heater heater1 = {1, 1, 200, 10, 0}; // {id, tank_id, power, watt_per_degree, is_on}
    Heater heater2 = {2, 2, 200, 10, 0};

    Heater heaters[] = {heater1, heater2};
    int num_heaters = sizeof(heaters) / sizeof(heaters[0]);

    // Inizializzazione della simulazione
    double deltaT = 1.0;     
    int t = 0;
    int volume_reached = 0;
    int concentration_reached = 0;
    int temperature_reached = 0;
    //while(1){
    while (VOLUME_ENABLED &&volume_reached == 0 || CONCENTRATION_ENABLED && concentration_reached == 0 || TEMPERATURE_ENABLED && temperature_reached == 0) {    // Itera fino a quando non si raggiungono i valori obiettivo di volume, concentrazione e temperatura
        // Stampa PRIMA degli aggiornamenti (mostra lo stato che sta per essere applicato)
        print_new_values(tanks, num_tanks, &valve, pumps, num_pumps, heaters, num_heaters, t);
        
        // Salva i valori precedenti PRIMA di aggiornare
        for (int i = 0; i < num_tanks; i++) {
            tanks[i].prev_volume = tanks[i].volume;
            tanks[i].prev_concentration = tanks[i].concentration;
        }
        
        // Aggiorna volume e concentrazione insieme
        update_tanks_volume(tanks, num_tanks, &valve, pumps, num_pumps, deltaT, &volume_reached);
        update_tank_concentration(tanks, num_tanks, &valve, pumps, num_pumps, deltaT, &concentration_reached, &volume_reached);
        update_tank_temperature(tanks, num_tanks, &valve, pumps, num_pumps, heaters, num_heaters, deltaT, &temperature_reached);
        
        t++;
        sleep(1);
    }  
    return 0;
}