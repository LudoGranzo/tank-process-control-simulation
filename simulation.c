#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "simulation.h"
#include "types.h"
#include "scenario_config.h"
    
void update_tanks_volume(Tank *tanks, int num_tanks, Valve *valve, Pump *pumps, int num_pumps, double deltaT, int *volume_reached) {   
    double volume_scopes[] = {TANK_SCOPE, TANK_SCOPE2}; // Array di volumi obiettivo dei serbatoi 
    static int initialized = 0; // Flag per inizializzazione una sola volta
    static int concentration_pumps_opened = 0; // Apri pompe d'ingresso al primo passo se concentration abilitato
    static int start = 0;
    
    // NOTE: STEP 0 (controllo pompe per la concentrazione) è stato spostato
    // in update_tank_concentration per separare i calcoli di volume da quelli
    // di concentrazione. Qui non rimane logica relativa alle pompe di
    // concentrazione.
    // Inizializzazione: controlla se i serbatoi hanno già raggiunto il volume obiettivo all'inizio
    if (!initialized) {
        for (int i = 0; i < num_tanks; i++) {
            // Un serbatoio ha "raggiunto" l'obiettivo se il suo volume è uguale al volume obiettivo.
            if (tanks[i].volume == volume_scopes[i]) {
                tanks[i].target_reached = 1;
            }
        }
        initialized = 1;
    }

    // Se il controllo della concentrazione è abilitato, al primo passo apri
    // le pompe di ingresso esterne verso i serbatoi così il sistema ha flusso
    // disponibile per i calcoli successivi (al "secondo 0").
#ifdef MIN_CONCENTRATION
    if (CONCENTRATION_ENABLED && !concentration_pumps_opened) {
        for (int p = 0; p < num_pumps; p++) {
            if (pumps[p].from_tank < 0 && pumps[p].to_tank >= 0) {
                pumps[p].is_on = 1;
            }
        }
        concentration_pumps_opened = 1;
    }
#endif

    *volume_reached = 1; // Inizialmente si assume che il ciclo sia completato
    for (int i = 0; i < num_tanks; i++) {
        double inflow = 0.0;
        double outflow = 0.0;

        // **STEP 1.1: Riempimento con volume inferiore al volume obiettivo**
        if (tanks[i].volume < volume_scopes[i] && !tanks[i].target_reached) {
            *volume_reached = 0;
            // Attiva TUTTE le pompe di ingresso quando il controllo del volume è abilitato per raggiungere il volume obiettivo nel più breve tempo possibile.   
            if (VOLUME_ENABLED && !CONCENTRATION_ENABLED) {
                for (int p = 0; p < num_pumps; p++) {
                    if ((pumps[p].to_tank == tanks[i].id) && (pumps[p].from_tank < 0)) {
                        pumps[p].is_on = 1; // Attiva tutte le pompe di ingresso
                    }
                }
            }
            
            if (VOLUME_ENABLED && CONCENTRATION_ENABLED && start) {
                for (int p = 0; p < num_pumps; p++) {
                    if ((pumps[p].to_tank == tanks[i].id) && (pumps[p].from_tank < 0)) {
                        pumps[p].is_on = 1; // Attiva tutte le pompe di ingresso
                    }
                }
                start = 1;
            }
            
            for (int p = 0; p < num_pumps; p++) { //Considera le pompe in ingresso per riempire verso l'obiettivo
                if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                    if (pumps[p].is_on) {
                        inflow += pumps[p].max_flow; // Aggiungi il flusso della pompa di ingresso solo se attiva
                    }
                }
            }
            //Spegni le pompe di scarico.
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    pumps[p].is_on = 0; // Spegni la pompa di scarico
                }
            }
            tanks[i].volume += (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume; // Calcola il nuovo volume del serbatoio
            // Se il volume supera il volume obiettivo, impostalo uguale al volume obiettivo.
            if (tanks[i].volume >= volume_scopes[i]) {
                tanks[i].volume = volume_scopes[i]; // Assicurati che il volume non superi il volume obiettivo
                tanks[i].target_reached = 1; // Marca che ha raggiunto l'obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di ingresso
                    }
                }
            }

        // **STEP 1.2: Volume iniziale superiore all'obiettivo - scarica per raggiungere il volume obiettivo**
        } else if (tanks[i].volume > volume_scopes[i] && !tanks[i].target_reached) {
            *volume_reached = 0;
            // Attiva TUTTE le pompe di scarico quando il controllo del volume è abilitato per raggiungere il volume obiettivo nel più breve tempo possibile.   
            if (VOLUME_ENABLED) {
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 1; // Attiva tutte le pompe di scarico
                    }
                }
            }
            for (int p = 0; p < num_pumps; p++) { //Considera le pompe in uscita per scaricare verso l'obiettivo
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    if (pumps[p].is_on) {
                        outflow += pumps[p].max_flow; // Aggiungi il flusso della pompa di uscita solo se attiva
                    }
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
                tanks[i].target_reached = 1; // Marca che ha raggiunto l'obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di scarico
                    }
                }
            }

        // **STEP 2: Scarico dopo aver raggiunto l'obiettivo - attiva pompe di scarico**
        } else if (tanks[i].target_reached && tanks[i].volume > 0 && EMPTYING_ENABLED) {
            *volume_reached = 0;
            // Attiva le pompe di scarico per questo serbatoio
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                    pumps[p].is_on = 1;
                    outflow += pumps[p].max_flow;
                }
            }
            // Spegni le pompe di ingresso
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                    pumps[p].is_on = 0;
                }
            }
            tanks[i].volume += (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume;
            if (tanks[i].volume <= 0) {
                tanks[i].volume = 0;
                tanks[i].concentration = 0.0; // Azzera concentrazione quando serbatoio vuoto
                // Spegni le pompe di scarico quando vuoto
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0;
                    }
                }
            }
            // **Caso 2.5: Obiettivo raggiunto ma EMPTYING_ENABLED disabilitato - continua calcolo volume**
        } else if (tanks[i].target_reached && !EMPTYING_ENABLED) {
           // Spegni tutte le pompe di ingresso e uscita.
           for (int p = 0; p < num_pumps; p++) {
               if (pumps[p].to_tank == tanks[i].id || pumps[p].from_tank == tanks[i].id) {
                   pumps[p].is_on = 0;
               }
           }
            tanks[i].volume += - EVAP_COEFF * tanks[i].volume; // Il volume del serbatoio rimane uguale a meno dell'evaporazione.
            if (tanks[i].volume <= 0) {
                tanks[i].volume = 0; // Assicurati che il volume non scenda sotto 0
                tanks[i].concentration = 0.0; // Azzera concentrazione quando serbatoio vuoto
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di scarico quando raggiunge 0
                    }
                }
            }
        }
    }

}

void update_tank_concentration(Tank *tanks, int num_tanks, Valve *valves, Pump *pumps, int num_pumps, double deltaT, int *concentration_reached, int *volume_reached) {
    if (concentration_reached) *concentration_reached = 1;

    // **STEP 0: CONTROLLO POMPE PER LA CONCENTRAZIONE (spostato qui)**
    // Quando CONCENTRATION_ENABLED è attivo, alla prima chiamata di questa
    // funzione apriamo le pompe di ingresso (quelle con from_tank < 0) in modo
    // che i calcoli di concentrazione possano usare immediatamente tali flussi.
    // Successivamente applichiamo la logica di selezione pompe basata sulla
    // concentrazione corrente per bilanciare le sorgenti.
    
#ifdef MIN_CONCENTRATION
    if (CONCENTRATION_ENABLED) {
        

        // Logica di selezione pompe (come in precedenza) per ciascun serbatoio.
        for (int i = 0; i < num_tanks; i++) {
            // Se il serbatoio ha già raggiunto l'obiettivo di volume, non
            // riattivare pompe di ingresso per motivi di concentrazione.
            if (tanks[i].target_reached) continue;

            double MIN_C = (tanks[i].id == 1) ? MIN_CONCENTRATION : MIN_CONCENTRATION2;
            double MAX_C = (tanks[i].id == 1) ? MAX_CONCENTRATION : MAX_CONCENTRATION2;
            double C_current = tanks[i].concentration;

            // Trova le pompe esterne per questo serbatoio
            int external_pumps[2] = {-1, -1};
            int ext_count = 0;

            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                    if (ext_count < 2) {
                        external_pumps[ext_count++] = p;
                    }
                }
            }

            if (ext_count == 0) continue;

            // Concentrazioni delle fonti
            double source_concentrations[2] = {0.0, 0.0};
            for (int k = 0; k < ext_count; k++) {
                int p_idx = external_pumps[k];
                switch (pumps[p_idx].id) {
                    case PUMP_ID1: source_concentrations[k] = CONCENTRATION_IN1; break;
                    case PUMP_ID2: source_concentrations[k] = CONCENTRATION_IN2; break;
                    case PUMP_ID7: source_concentrations[k] = CONCENTRATION_IN7; break;
                    case PUMP_ID8: source_concentrations[k] = CONCENTRATION_IN8; break;
                    default: source_concentrations[k] = CONCENTRATION_IN1; break;
                }
            }

            // Strategia di controllo
            if (C_current < (MIN_C + MAX_C) / 2) {
                // Concentrazione troppo bassa: attiva pompa con concentrazione PIÙ ALTA
                int best_pump = -1;
                double highest_conc = -1.0;
                for (int k = 0; k < ext_count; k++) {
                    if (source_concentrations[k] > highest_conc) {
                        highest_conc = source_concentrations[k];
                        best_pump = k;
                    }
                }
                if (best_pump >= 0) {
                    for (int k = 0; k < ext_count; k++) {
                        pumps[external_pumps[k]].is_on = (k == best_pump) ? 1 : 0;
                    }
                }
            } else if (C_current > (MIN_C + MAX_C) / 2) {
                // Concentrazione troppo alta: attiva pompa con concentrazione PIÙ BASSA
                int best_pump = -1;
                double lowest_conc = 2.0;
                for (int k = 0; k < ext_count; k++) {
                    if (source_concentrations[k] < lowest_conc) {
                        lowest_conc = source_concentrations[k];
                        best_pump = k;
                    }
                }
                if (best_pump >= 0) {
                    for (int k = 0; k < ext_count; k++) {
                        pumps[external_pumps[k]].is_on = (k == best_pump) ? 1 : 0;
                    }
                }
            } else {
                // Nel range: usa entrambe le pompe
                for (int k = 0; k < ext_count; k++) {
                    pumps[external_pumps[k]].is_on = 1;
                }
            }
        }
    }
#endif

    for (int i = 0; i < num_tanks; i++) {
        // **FASE 1: CALCOLA SEMPRE LA CONCENTRAZIONE CON LA FORMULA FISICA**
        // Calcola contributo Σ Pin*Cin*Δt basandosi su pompe attualmente ON
        double sum_Pin_Cin_dt = 0.0;
        double V_in = 0.0, V_out = 0.0;
        
        for (int p = 0; p < num_pumps; p++) {
            if (!pumps[p].is_on) continue;
            if (pumps[p].to_tank == tanks[i].id) {
                double Cin = 0.0;
                if (pumps[p].from_tank < 0) {
                    switch (pumps[p].id) {
                        case PUMP_ID1: Cin = CONCENTRATION_IN1; break;
                        case PUMP_ID2: Cin = CONCENTRATION_IN2; break;
                        case PUMP_ID7: Cin = CONCENTRATION_IN7; break;
                        case PUMP_ID8: Cin = CONCENTRATION_IN8; break;
                        default: Cin = CONCENTRATION_IN1; break;
                    }
                } else {
                    for (int j = 0; j < num_tanks; j++) {
                        if (tanks[j].id == pumps[p].from_tank) { 
                            Cin = tanks[j].concentration; 
                            break; 
                        }
                    }
                }
                sum_Pin_Cin_dt += pumps[p].max_flow * Cin * deltaT;
                V_in += pumps[p].max_flow * deltaT;
            }
            if (pumps[p].is_on && pumps[p].from_tank == tanks[i].id) {
                V_out += pumps[p].max_flow * deltaT;
            }
        }

        // APPLICA SEMPRE LA FORMULA FISICA PER IL CALCOLO DELLA CONCENTRAZIONE
        // Ma SOLO se il serbatoio ha già del volume
        if (tanks[i].volume > 0.0) {
            // Caso semplice richiesto: se siamo in fase di scarico puro (nessun inflow)
            // e lo scaricamento è abilitato, manteniamo la concentrazione costante
            // uguale alla concentrazione precedente.
            if (EMPTYING_ENABLED && V_in == 0.0 && V_out > 0.0) {
                tanks[i].concentration = tanks[i].prev_concentration;
                // Clamp tra 0 e 1
                if (tanks[i].concentration < 0.0) tanks[i].concentration = 0.0;
                if (tanks[i].concentration > 1.0) tanks[i].concentration = 1.0;
            } else {
                // Formula di miscelazione originale: C(t+1) = [C(t)*V(t) + Σ Pin*Cin*Δt] / V(t+1)
                tanks[i].concentration = (tanks[i].prev_concentration * tanks[i].prev_volume + sum_Pin_Cin_dt) / tanks[i].volume;
            }
        } else {
            // Serbatoio vuoto: concentrazione = 0
            tanks[i].concentration = 0.0;
        }

        // **FASE 2: SE CONTROLLO CONCENTRAZIONE DISABILITATO, STOP QUI**
        if (!CONCENTRATION_ENABLED) {
            continue; // Vai al prossimo serbatoio - la concentrazione varia naturalmente
        }

        // **FASE 3: VERIFICA SE SIAMO NEL RANGE TARGET**
        #ifdef MIN_CONCENTRATION
        double MIN_C = (tanks[i].id == 1) ? MIN_CONCENTRATION : MIN_CONCENTRATION2;
        double MAX_C = (tanks[i].id == 1) ? MAX_CONCENTRATION : MAX_CONCENTRATION2;

        double C_current = tanks[i].concentration;
        
        // Verifica se siamo fuori range per il flag concentration_reached
        if (C_current < MIN_C || C_current > MAX_C) {
            if (concentration_reached) *concentration_reached = 0;
        }
        #endif // MIN_CONCENTRATION
    }

    // Se il controllo del volume ha stabilito che tutti i serbatoi hanno
    // raggiunto il volume obiettivo, assicuriamoci che TUTTE le pompe siano
    // spente per evitare ulteriori cambiamenti (e consentire la terminazione).
    if (volume_reached && *volume_reached) {
        for (int p = 0; p < num_pumps; p++) {
            pumps[p].is_on = 0;
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
