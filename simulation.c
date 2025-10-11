#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "simulation.h"
#include "types.h"
#include "scenario_config.h"
    
void update_tanks_volume(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, double deltaT, int *volume_reached) {    
    double volume_scopes[] = {TANK_SCOPE, TANK_SCOPE2}; // Array di volumi obiettivo dei serbatoi 
    static int target_reached[2] = {0, 0}; // Array per tracciare se ogni serbatoio ha raggiunto il volume obiettivo
    *volume_reached = 1; // Inizialmente si assume che il ciclo sia completato
    
    for (int i = 0; i < num_tanks; i++) {
        double inflow = 0.0;
        double outflow = 0.0;

        // **Caso 1: Riempimento (solo se non ha ancora raggiunto l'obiettivo)**
        if (tanks[i].volume < volume_scopes[i] && !target_reached[i]) {
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
            if (tanks[i].volume >= volume_scopes[i]) {
                tanks[i].volume = volume_scopes[i]; // Assicurati che il volume non superi il volume obiettivo
                target_reached[i] = 1; // Marca che ha raggiunto l'obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di ingresso
                    }
                }
            }
        // **Caso 1.5: Volume iniziale superiore all'obiettivo - scarica verso obiettivo**
        } else if (tanks[i].volume > volume_scopes[i] && !target_reached[i]) {
            *volume_reached = 0;
            for (int p = 0; p < num_pumps; p++) { //Attiva le pompe in uscita per scaricare verso l'obiettivo
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

            tanks[i].volume += (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume; // Calcola il nuovo volume del serbatoio
            if (tanks[i].volume <= volume_scopes[i]) {
                tanks[i].volume = volume_scopes[i]; // Assicurati che il volume non scenda sotto il volume obiettivo
                target_reached[i] = 1; // Marca che ha raggiunto l'obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di scarico
                    }
                }
            }
        // **Caso 2: Scarico dopo aver raggiunto l'obiettivo (continua fino a volume 0)**
        } else if (target_reached[i] && tanks[i].volume > 0 && EMPTYING_ENABLED) {
            *volume_reached = 0;
            for (int p = 0; p < num_pumps; p++) { //Attiva le pompe in uscita per svuotare completamente il serbatoio
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

            tanks[i].volume += (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume; // Calcola il nuovo volume del serbatoio
            if (tanks[i].volume <= 0) {
                tanks[i].volume = 0; // Assicurati che il volume non scenda sotto 0
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di scarico quando raggiunge 0
                    }
                }
            }
        }
        // **Caso 3: Ciclo completato (volume = 0 dopo aver raggiunto obiettivo)**
        // Non fa nulla, il serbatoio ha finito il suo ciclo
    }
}

void update_tank_concentration(Tank *tanks, int num_tanks, Valve *valves, Pump *pumps, int num_pumps, double deltaT, int *concentration_reached, int *volume_reached) {
     *concentration_reached = 1;

    for (int i = 0; i < num_tanks; i++) {
        Tank *to = &tanks[i];
        double V_old = to->volume;
        if (V_old <= 0) continue;

        double total_volume_in = 0.0;
        double weighted_conc_sum = 0.0;

        for (int p = 0; p < num_pumps; p++) {
            if (pumps[p].is_on && pumps[p].to_tank == to->id) {
                for (int j = 0; j < num_tanks; j++) {
                    if (tanks[j].id == pumps[p].from_tank || pumps[p].from_tank < 0) {
                        double conc_in;
                        if (pumps[p].from_tank < 0) {
                            conc_in = (to->id == 1) ? CONCENTRATION_IN1 : CONCENTRATION_IN2;
                        } else {
                            conc_in = tanks[j].concentration;
                        }
                        double V_in = pumps[p].max_flow * deltaT;
                        if (pumps[p].from_tank >= 0 && V_in > tanks[j].volume) V_in = tanks[j].volume;
                        total_volume_in += V_in;
                        weighted_conc_sum += conc_in * V_in;
                        break;
                    }
                }
            }
        }

        double V_new = V_old + total_volume_in - EVAP_COEFF * V_old;
        if (V_new <= 0) continue;

        double C_old = to->concentration;
        double C_new = (C_old * V_old + weighted_conc_sum) / V_new;
        to->concentration = C_new;

        double MIN_C = (to->id == 1) ? MIN_CONCENTRATION : MIN_CONCENTRATION2;
        double MAX_C = (to->id == 1) ? MAX_CONCENTRATION : MAX_CONCENTRATION2;
        double CAP = (to->id == 1) ? CAPACITY : CAPACITY2;

        if (!*volume_reached) {
            // Durante la fase di riempimento, solo aggiornamento formula
            continue;
        }

        // Controllo concentrazione solo a volume raggiunto
        if (C_new < MIN_C || C_new > MAX_C) {
            *concentration_reached = 0;
        }
    }
}


void update_tank_temperature(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, Heater *heaters, int num_heaters, double deltaT, int *temperature_reached) {
    int all_temps_ok = 1;

    for (int i = 0; i < num_tanks; i++) {
        Tank *t = &tanks[i];
        
        // Se il serbatoio è vuoto, salta
        if (t->volume <= 0) continue;
        
        // Trova il riscaldatore corrispondente
        Heater *current_heater = NULL;
        for (int h = 0; h < num_heaters; h++) {
            if (heaters[h].tank_id == t->id) {
                current_heater = &heaters[h];
                break;
            }
        }
        
        if (current_heater == NULL) continue;
        
        // CONTROLLO DEL RISCALDATORE
        double tmin = (t->id == 1) ? TEMP_MIN1 : TEMP_MIN2;
        double tmax = (t->id == 1) ? TEMP_MAX1 : TEMP_MAX2;
        double tmid = (tmin + tmax) / 2.0;
        
        if (t->temperature < tmin) {
            current_heater->is_on = 1;
        } else if (t->temperature >= tmid) {
            current_heater->is_on = 0;
        }
        
        // CALCOLA SEMPRE IL MIXING TERMICO (anche con heater OFF)
        double V_prev = t->volume;
        
        // Calcola flussi in ingresso e temperatura media
        double total_volume_in = 0.0;
        double weighted_temp_sum = 0.0;
        
        for (int p = 0; p < num_pumps; p++) {
            if (pumps[p].is_on && pumps[p].to_tank == t->id) {
                double temp_in;
                if (pumps[p].from_tank < 0) {
                    temp_in = (t->id == 1) ? INLET_TEMPERATURE1 : INLET_TEMPERATURE2;
                } else {
                    for (int j = 0; j < num_tanks; j++) {
                        if (tanks[j].id == pumps[p].from_tank) {
                            temp_in = tanks[j].temperature;
                            break;
                        }
                    }
                }
                
                double V_in = pumps[p].max_flow * deltaT;
                if (pumps[p].from_tank >= 0) {
                    for (int j = 0; j < num_tanks; j++) {
                        if (tanks[j].id == pumps[p].from_tank && V_in > tanks[j].volume) {
                            V_in = tanks[j].volume;
                            break;
                        }
                    }
                }
                
                total_volume_in += V_in;
                weighted_temp_sum += temp_in * V_in;
            }
        }
        
        // Calcola V(t+1)
        double V_new = V_prev + total_volume_in - EVAP_COEFF * V_prev;
        if (V_new <= 0) {
            t->volume = 0;
            continue;
        }
        
        // APPLICA SEMPRE LA FORMULA: T(t+1) = [T(t)*V(t) + Σ P_in*T_in*Δt + Q_heat*Δt] / V(t+1)
        double numerator = t->temperature * V_prev + weighted_temp_sum;
        
        // Aggiungi Q_heat SOLO se il riscaldatore è ON
        if (current_heater->is_on) {
            double Q_heat = current_heater->power / current_heater->watt_per_degree;
            numerator += Q_heat * deltaT;
        }
        
        // Calcola la nuova temperatura
        t->temperature = numerator / V_new;
        
        // Verifica condizione di terminazione
        if (t->temperature < tmin || t->temperature > tmax || current_heater->is_on) {
            all_temps_ok = 0;
        }
    }
    
    if (temperature_reached) *temperature_reached = all_temps_ok;
}

void print_new_values(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, Heater *heaters, int num_heaters, int t) {
    printf("Time: %ds\n", t);
    for (int i = 0; i < num_tanks; i++) {
            if (tanks[i].volume > 0) {
                printf("TANK %d: V= %.2f L, C= %.2f, T= %.2f°C\n", tanks[i].id, tanks[i].volume, tanks[i].concentration, tanks[i].temperature);
            } else {
                printf("TANK %d: V= %.2f L, C= %.2f\n", tanks[i].id, tanks[i].volume, tanks[i].concentration);
            }
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
    for (int h = 0; h < num_heaters; h++) {
        printf("HEATER%d:%s\n",
               heaters[h].id,
               heaters[h].is_on ? "ON" : "OFF");
    }
        
    printf("\n");
}

void emptying_tank(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, Heater *heaters, int num_heaters, double deltaT) {
   for (int i = 0; i < num_tanks; i++) {
        int outflow = 0.0;
        while (tanks[i].volume != 0){
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    pumps[p].is_on = 1; // Attiva la pompa di scarico
                }
                outflow += pumps[p].max_flow; // Aggiungi il flusso della pompa di uscita
            }
            tanks[i].volume += (0 - outflow) * deltaT - EVAP_COEFF * tanks[i].volume; // Calcola il nuovo volume del serbatoio
        }
        if (tanks[i].volume < 0) {
                tanks[i].volume = 0; // Assicurati che il volume non scenda sotto il volume obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di scarico
                    }
                }
            }
   }
   print_new_values(tanks, num_tanks, valve, pumps, num_pumps, heaters, num_heaters, 0); // Stampa i valori finali dopo lo svuotamento
}