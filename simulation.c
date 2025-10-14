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
            // Attiva TUTTE le pompe di ingresso per scenario volume-only
            if (VOLUME_ENABLED && !CONCENTRATION_ENABLED) {
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                        pumps[p].is_on = 1; // Attiva tutte le pompe di ingresso
                    }
                }
            }
            
            for (int p = 0; p < num_pumps; p++) { //Considera le pompe in ingresso per riempire il serbatoio
                if (pumps[p].to_tank ==  tanks[i].id && pumps[p].from_tank < 0){
                    if (pumps[p].is_on) {
                        inflow += pumps[p].max_flow; // Aggiungi il flusso della pompa di ingresso
                    }
                }
            }

            // Evita overshoot: se il volume previsto dovesse superare il volume obiettivo,
            // scala l'inflow così da arrivare esattamente a volume_scopes[i] nel passo corrente.
            {
                double predicted = tanks[i].volume + (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume;
                if (predicted > volume_scopes[i]) {
                    // inflow_down = ((scope - V_t + EVAP_TERM) / deltaT) + outflow
                    double allowed_inflow = ((volume_scopes[i] - tanks[i].volume + EVAP_COEFF * tanks[i].volume) / deltaT) + outflow;
                    if (allowed_inflow < 0.0) allowed_inflow = 0.0;
                    if (allowed_inflow < inflow) {
                        inflow = allowed_inflow;
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
            // Usa tolleranza più ampia per raggiungere il volume obiettivo
            if (tanks[i].volume >= volume_scopes[i] - 0.5) {
                tanks[i].volume = volume_scopes[i]; // Assicurati che il volume non superi il volume obiettivo
                target_reached[i] = 1; // Marca che ha raggiunto l'obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].to_tank == tanks[i].id && pumps[p].from_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di ingresso
                    }
                }
                // Se è abilitato lo svuotamento, attiva le pompe di scarico per questo serbatoio
                if (EMPTYING_ENABLED) {
                    for (int p = 0; p < num_pumps; p++) {
                        if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                            pumps[p].is_on = 1; // Attiva la pompa di scarico
                        }
                    }
                }
            }
        // **Caso 1.5: Volume iniziale superiore all'obiettivo - scarica verso obiettivo**
        } else if (tanks[i].volume > volume_scopes[i] && !target_reached[i]) {
            *volume_reached = 0;
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
                target_reached[i] = 1; // Marca che ha raggiunto l'obiettivo
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0; // Spegni la pompa di scarico
                    }
                }
            }
        
        // **Caso 2: Scarico dopo aver raggiunto l'obiettivo - attiva pompe di scarico**
        } else if (target_reached[i] && tanks[i].volume > 0 && EMPTYING_ENABLED) {
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
                // Spegni le pompe di scarico quando vuoto
                for (int p = 0; p < num_pumps; p++) {
                    if (pumps[p].from_tank == tanks[i].id && pumps[p].to_tank < 0) {
                        pumps[p].is_on = 0;
                    }
                }
            }
            // **Caso 2.5: Obiettivo raggiunto ma EMPTYING_ENABLED disabilitato - continua calcolo volume**
        } else if (target_reached[i] && !EMPTYING_ENABLED) {
            // Calcola inflow e outflow dalle pompe attualmente attive
            for (int p = 0; p < num_pumps; p++) {
                if (pumps[p].is_on) {
                    if (pumps[p].to_tank == tanks[i].id) {
                        inflow += pumps[p].max_flow;
                    }
                    if (pumps[p].from_tank == tanks[i].id) {
                        outflow += pumps[p].max_flow;
                    }
                }
            }

            // Evita overshoot anche quando il target è marcato come raggiunto ma le pompe
            // possono essere state riattivate dal controllore: scala inflow se il volume
            // previsto supererebbe il volume obiettivo.
            {
                double scope = volume_scopes[i];
                double predicted = tanks[i].volume + (inflow - outflow) * deltaT - EVAP_COEFF * tanks[i].volume;
                if (predicted > scope) {
                    double allowed_inflow = ((scope - tanks[i].volume + EVAP_COEFF * tanks[i].volume) / deltaT) + outflow;
                    if (allowed_inflow < 0.0) allowed_inflow = 0.0;
                    if (allowed_inflow < inflow) inflow = allowed_inflow;
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
    // Implementazione unica ON/OFF per due ingressi esterni per ogni serbatoio
    // Usa la formula di miscelazione perfetta: C(t+1) = [C(t)*V(t) + Σ Pin*Cin*Δt ] / V(t+1)
    // e accende/spegne le pompe esterne (da sorgenti negative) per portare C dentro [MIN,MAX].
    if (concentration_reached) *concentration_reached = 1;

    for (int i = 0; i < num_tanks; i++) {
        Tank *tank = &tanks[i];
        double V_t = tank->volume;
        double C_t = tank->concentration;

        // Se volume zero skip (niente da miscelare)
        if (V_t <= 0.0) continue;

        // Calcola contributo Σ Pin*Cin*Δt e V_in/V_out basandosi su pompe attualmente ON
        double sum_Pin_Cin_dt = 0.0;
        double V_in = 0.0, V_out = 0.0;
        for (int p = 0; p < num_pumps; p++) {
            if (!pumps[p].is_on) continue;
            if (pumps[p].to_tank == tank->id) {
                double Cin = 0.0;
                if (pumps[p].from_tank < 0) {
                    // fonte esterna: mappa ID -> concentrazione definita nello scenario
                    switch (pumps[p].id) {
                        case PUMP_ID1: Cin = CONCENTRATION_IN1; break;
                        case PUMP_ID2: Cin = CONCENTRATION_IN2; break;
                        case PUMP_ID7: Cin = CONCENTRATION_IN7; break;
                        case PUMP_ID8: Cin = CONCENTRATION_IN8; break;
                        default: Cin = CONCENTRATION_IN1; break;
                    }
                } else {
                    // provenienza da altro serbatoio
                    for (int j = 0; j < num_tanks; j++) if (tanks[j].id == pumps[p].from_tank) { Cin = tanks[j].concentration; break; }
                }
                sum_Pin_Cin_dt += pumps[p].max_flow * Cin * deltaT;
                V_in += pumps[p].max_flow * deltaT;
            }
            if (pumps[p].is_on && pumps[p].from_tank == tank->id) V_out += pumps[p].max_flow * deltaT;
        }

        double V_tp1 = V_t + V_in - V_out - (EVAP_COEFF * V_t * deltaT);
        if (V_tp1 <= 0.0) { tank->concentration = 0.0; continue; }

        double C_tp1 = (C_t * V_t + sum_Pin_Cin_dt) / V_tp1;
        if (C_tp1 < 0.0) C_tp1 = 0.0; if (C_tp1 > 1.0) C_tp1 = 1.0;
        tank->concentration = C_tp1;

        if (!CONCENTRATION_ENABLED) continue;

        // Determina i limiti per questo serbatoio
        double MIN_C = (tank->id == 1) ? MIN_CONCENTRATION : MIN_CONCENTRATION2;
        double MAX_C = (tank->id == 1) ? MAX_CONCENTRATION : MAX_CONCENTRATION2;

        // Il controllore ora può intervenire anche durante il riempimento: valutiamo sempre le combinazioni
        if (C_tp1 < MIN_C || C_tp1 > MAX_C) if (concentration_reached) *concentration_reached = 0;

        // Trova indici delle pompe esterne che alimentano questo serbatoio (al massimo 2 per scenario)
        int ext_idx[2] = {-1, -1}; int ext_count = 0;
        int discharge_idx = -1;
        for (int p = 0; p < num_pumps; p++) {
            if (pumps[p].to_tank == tank->id && pumps[p].from_tank < 0) {
                if (ext_count < 2) ext_idx[ext_count++] = p;
            }
            if (pumps[p].from_tank == tank->id && pumps[p].to_tank < 0 && discharge_idx < 0) discharge_idx = p;
        }

    // Decisione ON/OFF valutando combinazioni in modo da non superare il volume obiettivo
    double scope = (tank->id == 1) ? TANK_SCOPE : TANK_SCOPE2;
    // Isteresi di riempimento: non riempire di nuovo se il volume è solo
    // leggermente sotto lo scope a causa di evaporazione. Usa una soglia
    // relativa (es. 2% dello scope).
    double fill_hyst = 0.02 * scope;
        // Prepara array per le concentrazioni/portate delle fonti esterne (visibili in tutta la sezione)
        double srcC_arr[2] = {0.0, 0.0};
        double srcF_arr[2] = {0.0, 0.0};
        if (C_tp1 < MIN_C || C_tp1 > MAX_C) {
            if (concentration_reached) *concentration_reached = 0;
            // Riempi i dati sulle fonti esterne (srcC e flow)
            for (int k = 0; k < ext_count; k++) {
                int p = ext_idx[k];
                double srcC = CONCENTRATION_IN1;
                switch (pumps[p].id) { case PUMP_ID1: srcC = CONCENTRATION_IN1; break; case PUMP_ID2: srcC = CONCENTRATION_IN2; break; case PUMP_ID7: srcC = CONCENTRATION_IN7; break; case PUMP_ID8: srcC = CONCENTRATION_IN8; break; }
                srcC_arr[k] = srcC;
                srcF_arr[k] = pumps[p].max_flow;
            }

            // Recupera flusso di scarico massimo (se presente)
            double discharge_flow = 0.0;
            if (discharge_idx >= 0) discharge_flow = pumps[discharge_idx].max_flow;

            // PRIORITA': prova ogni singola fonte - scegli quella che DA SOLA porta C dentro il range
            int chosen_mask = -1;
            for (int k = 0; k < ext_count; k++) {
                double src = srcC_arr[k];
                double V_in_try = srcF_arr[k] * deltaT;
                double V_out_try = discharge_flow * deltaT; // assumiamo attivazione scarico
                double V_tp1_try = V_t + V_in_try - V_out_try - (EVAP_COEFF * V_t * deltaT);
                double scope = (tank->id == 1) ? TANK_SCOPE : TANK_SCOPE2;
                if (V_tp1_try <= 0.0 || V_tp1_try > scope + 1e-6) continue;
                double C_tp1_try = (C_t * V_t + src * V_in_try) / V_tp1_try;
                if (C_tp1_try < 0.0) C_tp1_try = 0.0; if (C_tp1_try > 1.0) C_tp1_try = 1.0;
                // se la concentrazione corrente è sotto il range, cerchiamo fonte con C_tp1_try >= MIN_C
                if (C_t < MIN_C && C_tp1_try >= MIN_C && C_tp1_try <= MAX_C) { chosen_mask = (1 << k); break; }
                // se la concentrazione corrente è sopra il range, cerchiamo fonte con C_tp1_try <= MAX_C
                if (C_t > MAX_C && C_tp1_try <= MAX_C && C_tp1_try >= MIN_C) { chosen_mask = (1 << k); break; }
                // se siamo dentro il range già, non serve
            }

            if (chosen_mask >= 0) {
                // Applica la singola fonte scelta
                for (int k = 0; k < ext_count; k++) {
                    int p = ext_idx[k];
                    pumps[p].is_on = ((chosen_mask & (1 << k)) != 0) ? 1 : 0;
                }
                if (discharge_idx >= 0) pumps[discharge_idx].is_on = (chosen_mask != 0) ? 1 : 0;
            } else {
                // Valuta tutte le combinazioni di accensione: 0..(2^ext_count-1)
            int best_mask = 0; // default: nessuna pompa
            double best_score = 1e9; // distanza dalla banda target (minimizzare)
            int found_valid = 0;

            int max_mask = (1 << ext_count);
            for (int mask = 0; mask < max_mask; mask++) {
                // calcola V_in e sum_Pin_Cin_dt per questa combinazione
                double V_in_c = 0.0;
                double sum_Pin_Cin_dt_c = 0.0;
                for (int k = 0; k < ext_count; k++) {
                    if (mask & (1 << k)) {
                        V_in_c += srcF_arr[k] * deltaT;
                        sum_Pin_Cin_dt_c += srcF_arr[k] * srcC_arr[k] * deltaT;
                    }
                }
                // assumiamo che se abilitiamo ingressi esterni, attiviamo la pompa di scarico
                double V_out_c = (mask == 0) ? 0.0 : (discharge_flow * deltaT);

                double V_tp1_c = V_t + V_in_c - V_out_c - (EVAP_COEFF * V_t * deltaT);
                // non considerare combinazioni che portano il volume oltre lo scope (o sotto 0)
                double scope = (tank->id == 1) ? TANK_SCOPE : TANK_SCOPE2;
                // non permettere overshoot: scarta combinazioni che portano il volume oltre lo scope
                if (V_tp1_c <= 0.0) continue;
                if (V_tp1_c > scope + 1e-6) continue;

                // calcola C(t+1) per questa combinazione
                double C_tp1_c = (C_t * V_t + sum_Pin_Cin_dt_c) / V_tp1_c;
                if (C_tp1_c < 0.0) C_tp1_c = 0.0; if (C_tp1_c > 1.0) C_tp1_c = 1.0;

                // preferiamo combinazioni che portino C dentro il range
                double score = 0.0;
                if (C_tp1_c < MIN_C) score = MIN_C - C_tp1_c;
                else if (C_tp1_c > MAX_C) score = C_tp1_c - MAX_C;
                else score = 0.0; // dentro il range -> perfetto

                // preferisci meno pompe attive se score equal (peso maggiore per favorire singola fonte)
                score += 0.2 * (__builtin_popcount(mask));

                // premia combinazioni che usano fonti la cui concentrazione sorgente è già dentro il range
                int inrange_sources = 0;
                for (int k = 0; k < ext_count; k++) {
                    if (mask & (1 << k)) {
                        double src = srcC_arr[k];
                        if (src >= MIN_C && src <= MAX_C) inrange_sources++;
                    }
                }
                score -= 0.1 * inrange_sources; // riduce il punteggio (migliore) se usa fonti in-range

                if (score < best_score) {
                    best_score = score;
                    best_mask = mask;
                    found_valid = 1;
                    if (score == 0.0) break; // soluzione ideale trovata
                }
            }

            // Applica la migliore combinazione trovata
            for (int k = 0; k < ext_count; k++) {
                int p = ext_idx[k];
                pumps[p].is_on = ((best_mask & (1 << k)) != 0) ? 1 : 0;
            }
            if (discharge_idx >= 0) pumps[discharge_idx].is_on = (best_mask != 0) ? 1 : 0;

            // Se non esiste nessuna combinazione valida che rispetti il volume
            // e il serbatoio non ha ancora raggiunto lo scope, abilitiamo una pompa minima
            double scope = (tank->id == 1) ? TANK_SCOPE : TANK_SCOPE2;
            if (!found_valid && V_t < scope - fill_hyst) {
                // scegli la fonte preferita: prima in-range, altrimenti quella con flusso minimo
                int pick = -1;
                for (int k = 0; k < ext_count; k++) {
                    double V_in_try = srcF_arr[k] * deltaT;
                    double V_out_try = 0.0; // durante il riempimento manteniamo discharge OFF
                    double V_tp1_try = V_t + V_in_try - V_out_try - (EVAP_COEFF * V_t * deltaT);
                    if (V_tp1_try > 0.0 && V_tp1_try <= scope + 1e-6) {
                        double src = srcC_arr[k];
                        if (src >= MIN_C && src <= MAX_C) { pick = k; break; }
                        if (pick < 0) pick = k;
                    }
                }
                if (pick < 0) {
                    // fallback: pick smallest flow
                    double best_flow = 1e12;
                    for (int k = 0; k < ext_count; k++) {
                        if (srcF_arr[k] < best_flow) { best_flow = srcF_arr[k]; pick = k; }
                    }
                }
                if (pick >= 0) {
                    for (int k = 0; k < ext_count; k++) {
                        int p = ext_idx[k]; pumps[p].is_on = (k == pick) ? 1 : 0;
                    }
                    if (discharge_idx >= 0) pumps[discharge_idx].is_on = 0; // no discharge during filling
                }
            } else if (!found_valid) {
                // se non abbiamo trovato combinazioni valide e il volume è >= scope, spegni tutto
                for (int k = 0; k < ext_count; k++) pumps[ext_idx[k]].is_on = 0;
                if (discharge_idx >= 0) pumps[discharge_idx].is_on = 0;
            }

            // Se il risultato è valido ma il serbatoio non ha raggiunto lo scope, assicurati di
            // mantenere almeno una pompa attiva (controller deve riempire fino al target)
            if (found_valid && V_t < scope - fill_hyst) {
                int inlet_active = 0;
                for (int k = 0; k < ext_count; k++) if (pumps[ext_idx[k]].is_on) inlet_active = 1;
                if (!inlet_active && ext_count > 0) {
                    // preferisci una fonte in-range
                    int pick = -1;
                    for (int k = 0; k < ext_count; k++) {
                        double src = srcC_arr[k];
                        if (src >= MIN_C && src <= MAX_C) { pick = k; break; }
                    }
                    if (pick < 0) pick = 0;
                    for (int k = 0; k < ext_count; k++) { int p = ext_idx[k]; pumps[p].is_on = (k == pick) ? 1 : 0; }
                    if (discharge_idx >= 0) pumps[discharge_idx].is_on = 0;
                }
            }
            }

        } else {
            // Dentro il range: se il serbatoio non ha ancora raggiunto lo scope, mantieni
            // le pompe attive (controller può commutarle) e tieni discharge OFF; solo se
            // il volume >= scope allora spegni le fonti esterne e la pompa di scarico.
            double scope = (tank->id == 1) ? TANK_SCOPE : TANK_SCOPE2;
            if (V_t < scope - fill_hyst) {
                // assicurati che almeno una pompa di ingresso sia attiva
                int inlet_active = 0;
                for (int k = 0; k < ext_count; k++) if (pumps[ext_idx[k]].is_on) inlet_active = 1;
                if (!inlet_active && ext_count > 0) {
                    // prefer in-range source
                    int pick = -1;
                    for (int k = 0; k < ext_count; k++) {
                        double src = srcC_arr[k]; if (src >= MIN_C && src <= MAX_C) { pick = k; break; }
                    }
                    if (pick < 0) pick = 0;
                    for (int k = 0; k < ext_count; k++) { int p = ext_idx[k]; pumps[p].is_on = (k == pick) ? 1 : 0; }
                }
                if (discharge_idx >= 0) pumps[discharge_idx].is_on = 0;
            } else {
                for (int k = 0; k < ext_count; k++) pumps[ext_idx[k]].is_on = 0;
                if (discharge_idx >= 0) pumps[discharge_idx].is_on = 0;
            }
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