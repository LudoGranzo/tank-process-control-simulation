#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Per la funzione sleep

#include "types.h"
#include "constants.h"   
#include "simulation.h"  

int main() {
    Tank tank = {ID,CAPACITY, INITIAL_VOLUME, INITIAL_TEMPERATURE, INITIAL_CONCENTRATION};
    Valve valve = {1, 0, 0, 10.0, 1}; // Esempio di inizializzazione della valvola
    double deltaT = 1.0; // Intervallo di tempo in secondi
    for (int t = 0; tank.volume != TANK_SCOPE;t += deltaT) {
        update_tank_volume(&tank, &valve, deltaT);
        printf("Tempo: %.2f s\n", (double)t);
        printf("TANK %d: V: %.2f L, C: %.2f, T: %.2f °C\n", tank.id, tank.volume, tank.concentration, tank.temperature);
        if (valve.is_on) {
            printf("Valve %d: ON\n\n", valve.id);
        } else {
            printf("Valve %d: OFF\n\n", valve.id);
        }
        // Simula una pausa per visualizzare i risultati
        sleep(1); // Pausa di 1 secondo tra ogni passo temporale
    }
    return 0;
}