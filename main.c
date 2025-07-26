#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Per la funzione sleep

#include "types.h"
#include "constants.h"   
#include "simulation.h"  

int main() {
    Tank tank1 = {ID,CAPACITY, INITIAL_VOLUME, INITIAL_TEMPERATURE, INITIAL_CONCENTRATION};
    Tank tank2 = {ID2, CAPACITY2, INITIAL_VOLUME2, INITIAL_TEMPERATURE2, INITIAL_CONCENTRATION2};
    Tank tanks[] = {tank1, tank2}; // Array di serbatoi
    int num_tanks = sizeof(tanks) / sizeof(tanks[0]);
    
    Valve valve = {VALVE_ID, VALVE_FROM_TANK, VALVE_TO_TANK, VALVE_MAX_FLOW, VALVE_IS_ON}; // Esempio di inizializzazione della valvola

    double deltaT = 1.0; // Intervallo di tempo in secondi
    for (int i = 0; i < num_tanks; i ++){ 
        printf("Time: 0s\nTANK %d: V= %.2f L, C= %.2f, T= %.2f°C\nVALVE%d%d:CLOSE\n\n", tanks[i].id, tanks[i].volume, tanks[i].concentration, tanks[i].temperature, valve.from_tank, valve.to_tank);
        for (int t = 1; tanks[i].volume != TANK_SCOPE; t += deltaT) {
            update_tank_volume(&tanks[i], num_tanks, &valve, deltaT);
            printf("Time: %ds\nTANK %d: V= %.2f L, C= %.2f, T= %.2f°C\n", t, tanks[i].id, tanks[i].volume, tanks[i].concentration, tanks[i].temperature);
            if(valve.is_on == 1) {
                printf("VALVE%d%d: OPEN\n\n", valve.from_tank, valve.to_tank);
            } else {
                printf("VALVE%d%d: CLOSE\n\n", valve.from_tank, valve.to_tank);
            }
            sleep(1); // Pausa di 1 secondo tra gli aggiornamenti
        }
    }
    return 0;
}