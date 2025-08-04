#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"
#include "types.h"
#include "constants.h"
    
void update_tank_volume(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, double deltaT, int *volume_reached) {    
    double volume_scopes[] = {TANK_SCOPE, TANK_SCOPE2}; // Array di volumi obiettivo dei serbatoi 
    *volume_reached = 1; // Inizialmente si assume che il volume obiettivo sia raggiunto
    double volume_changes[num_tanks];
    for (int i = 0; i < num_tanks; i++) {
        double inflow = 0.0;
        double outflow = 0.0;

        // **Caso 1: Riempimento**
        if (tanks[i].volume < volume_scopes[i]) {
            *volume_reached = 0;
            for (int p = 0; p < num_pumps; p++) { //Attiva le pompe in ingresso per riempire il serbatoio
                if (pumps[p].to_tank ==  tanks[i].id && pumps[p].from_tank < 0){
                    if (!pumps[p].is_on) {
                        pumps[p].is_on = 1;
                    }
                    inflow += pumps[p].max_flow; // Aggiungi il flusso della pompa di ingresso
                }
            }

            //Spegni le pompe di scarico.
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    pumps[p].is_on = 0; // Spegni la pompa di scarico
                }
            }

            tanks[i].volume += (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume; // Calcola il nuovo volume del serbatoio
            if (tanks[i].volume > volume_scopes[i]) {
                tanks[i].volume = volume_scopes[i]; // Assicurati che il volume non superi il volume obiettivo
            }
            // **Caso 2: Scarico**
        } else if (tanks[i].volume > volume_scopes[i]) {
            *volume_reached = 0;
            for (int p = 0; p < num_pumps; p++) { //Attiva le pompe in uscita per svuotare il serbatoio
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    if (!pumps[p].is_on) {
                        pumps[p].is_on = 1;
                    }
                    outflow += pumps[p].max_flow; // Aggiungi il flusso della pompa di uscita
                }
            }

            //Spegni le pompe di ingresso.
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                    pumps[p].is_on = 0; // Spegni la pompa di ingresso
                }
            }

            tanks[i].volume -= (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume; // Calcola il nuovo volume del serbatoio
            if (tanks[i].volume < volume_scopes[i]) {
                tanks[i].volume = volume_scopes[i]; // Assicurati che il volume non scenda sotto il volume obiettivo
            }
        } else {    // **Caso 3: Volume Obiettivo Raggiunto**
            inflow = 0.0;
            outflow = 0.0;
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                    pumps[p].is_on = 0; // Spegni la pompa di ingresso
                }
            }
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    pumps[p].is_on = 0; // Spegni la pompa di uscita
                }
            }
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

void update_tank_temperature(Tank *tanks, int num_tanks, Valve *valve, Heater *heaters, int num_heaters, double deltaT) {
    if (!valve->is_on)
        return;

    Tank *from = NULL;
    Tank *to = NULL;

    for (int i = 0; i < num_tanks; i++) {
        if (tanks[i].id == valve->from_tank) from = &tanks[i];
        if (tanks[i].id == valve->to_tank) to = &tanks[i];
    }

    if (from == NULL || to == NULL) return;

    double volume_in = valve->max_flow * deltaT;
    if (volume_in > from->volume)
        volume_in = from->volume;

    double V_old = to->volume;
    double V_new = V_old + volume_in - EVAP_COEFF * V_old;
    if (V_new <= 0) return;

    double T_old = to->temperature;
    double T_in = from->temperature;
    double Q_heat = 0.0;

    for (int i = 0; i < num_heaters; i++) {
        if (heaters[i].tank_id == to->id && heaters[i].is_on) {
            Q_heat = heaters[i].power / heaters[i].watt_per_degree;
            break;
        }
    }

    double T_new = (T_old * V_old + T_in * volume_in + Q_heat * deltaT) / V_new;
    to->temperature = T_new;
}

void print_new_values(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, int t) {
    printf("Time: %ds\n", t);
    for (int i = 0; i < num_tanks; i++) {
        printf("TANK %d: V= %.2f L, C= %.2f, T= %.2f°C\n", tanks[i].id, tanks[i].volume, tanks[i].concentration, tanks[i].temperature);
    }
    if (valve->is_on) 
        printf("VALVE%d%d: OPEN\n", valve->from_tank, valve->to_tank);
    else
        printf("VALVE%d%d: CLOSE\n", valve->from_tank, valve->to_tank);
    for (int i = 0; i < num_pumps; i++) {
        printf("PUMP P%d%d:%s\n", 
                pumps[i].from_tank, pumps[i].to_tank, 
                pumps[i].is_on ? "ON" : "OFF");
    }
        
    printf("\n");
}
