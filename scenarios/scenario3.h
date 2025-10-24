#ifndef SCENARIO3_H
#define SCENARIO3_H

// SCENARIO 3: Controllo della concentrazione con due fonti
// Un serbatoio viene alimentato costantemente da due fonti con diverse concentrazioni.
// L'obiettivo è raggiungere il volume scopo, mantenere il volume (a meno di perdite dovute al coefficiente di evaporazione) e nel frattempo mantenere la concentrazione entro il range specificato.
// Il sistema deve decidere quale pompa attivare in modo da mantenere la concentrazione entro il range desiderato.

// Valori di simulazione iniziali serbatoio 1
#define ID 1 // Identificativo serbatoio
#define INITIAL_VOLUME 0.0 // Volume iniziale
#define INITIAL_CONCENTRATION 0.0 // Concentrazione iniziale del fluido già presente nel serbatoio 1 (0 perchè non c'è fluido).
#define CAPACITY 150.0 // Capacità massima
#define TANK_SCOPE 100.0 // Volume obiettivo
#define MIN_CONCENTRATION 0.4 // Valore MINIMO del range di concentrazione
#define MAX_CONCENTRATION 0.6 // Valore MASSIMO del range di concentrazione
#define INITIAL_TEMPERATURE 23 // Temperatura iniziale del fluido già presente nel serbatoio (se non c'e fluido non c'è temperatura)
#define TEMP_MIN1 20.0 // Valore MINIMO del range di temperatura
#define TEMP_MAX1 25.0 // Valore MASSIMO del range di temperatura

// Valori di simulazione iniziali serbatoio 2
#define ID2 2 // Identificativo serbatoio
#define INITIAL_VOLUME2 1.0 // Volume iniziale
#define INITIAL_CONCENTRATION2 0.5  // Concentrazione iniziale del fluido già presente nel serbatoio 2
#define CAPACITY2 150.0 // Capacità massima
#define TANK_SCOPE2 80.0  // Volume obiettivo serbatoio 2
#define MIN_CONCENTRATION2 0.4 // Range di concentrazione target serbatoio 2
#define MAX_CONCENTRATION2 0.6 // Range di concentrazione target serbatoio 2
#define INITIAL_TEMPERATURE2 20.0 // Temperatura iniziale del fluido già presente nel serbatoio
#define TEMP_MIN2 20.0 // Valore MINIMO del range di temperatura
#define TEMP_MAX2 25.0 // Valore MASSIMO del range di temperatura

// Temperature dei fluidi in ingresso
#define INLET_TEMPERATURE1 22.0  // Temperatura dei fluidi in ingresso al serbatoio 1
#define INLET_TEMPERATURE2 22.0  // Temperatura dei fluidi in ingresso al serbatoio 2

// Valvola (non utilizzata in questo scenario)
#define VALVE_ID 1
#define VALVE_FROM_TANK 1
#define VALVE_TO_TANK 2
#define VALVE_MAX_FLOW 0.0  // Disabilitata
#define VALVE_IS_ON 0

// Valori di simulazione iniziali prima pompa in ingresso al serbatoio 1
#define PUMP_ID1 1 // Identificativo pompa
#define PUMP_FROM_TANK1 -1 // Fonte esterna
#define PUMP_TO_TANK1 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW1 7.0 // Flusso massimo
#define PUMP_IS_ON1 0 // Inizialmente spenta
#define CONCENTRATION_IN1 0.9 // Concentrazione del primo fluido in ingresso al serbatoio 1

// Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 1
#define PUMP_ID2 2 // Identificativo pompa
#define PUMP_FROM_TANK2 -1 // Fonte esterna
#define PUMP_TO_TANK2 1 // Verso serbatoio principale
#define PUMP_MAX_FLOW2 8.0 // Flusso massimo
#define PUMP_IS_ON2 0 // Inizialmente spenta
#define CONCENTRATION_IN2 0.3 // Concentrazione del secondo fluido in ingresso al serbatoio 1

// Valori di simulazione iniziali della prima pompa in ingresso al serbatoio 2
#define PUMP_ID7 7 // Identificativo pompa
#define PUMP_FROM_TANK7 -2 // Fonte esterna
#define PUMP_TO_TANK7 2 // Verso serbatoio 2
#define PUMP_MAX_FLOW7 10.0 // Flusso massimo
#define PUMP_IS_ON7 0 // Inizialmente spenta
#define CONCENTRATION_IN7 0.8 // Concentrazione del primo fluido in ingresso al serbatoio 2

// Valori di simulazione iniziali della seconda pompa in ingresso al serbatoio 2
#define PUMP_ID8 8 // Identificativo pompa
#define PUMP_FROM_TANK8 -2 // Fonte esterna
#define PUMP_TO_TANK8 2 // Verso serbatoio 2
#define PUMP_MAX_FLOW8 3.0 // Flusso massimo
#define PUMP_IS_ON8 0 // Inizialmente spenta
#define CONCENTRATION_IN8 0.4 // Concentrazione del secondo fluido in ingresso al serbatoio 2

// Valori di simulazione pompa di collegamento tra tank1 e tank2 (disabilitata)
#define PUMP_ID3 3 // Identificativo pompa
#define PUMP_FROM_TANK3 1 // Serbatoio di partenza
#define PUMP_TO_TANK3 2 // Serbatoio di arrivo
#define PUMP_MAX_FLOW3 0.0 // Disabilitata
#define PUMP_IS_ON3 0 // Inizialmente spenta

// Valori di simulazione pompa di collegamento tra tank2 e tank1 (disabilitata)
#define PUMP_ID4 4 // Identificativo pompa
#define PUMP_FROM_TANK4 2 // Serbatoio di partenza
#define PUMP_TO_TANK4 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW4 0.0 // Disabilitata
#define PUMP_IS_ON4 0 // Inizialmente spenta    

// Valori di simulazione iniziali pompa di scarico serbatoio 1 (non utilizzata)
#define PUMP_ID5 5 // Identificativo pompa
#define PUMP_FROM_TANK5 1 // Serbatoio di partenza
#define PUMP_TO_TANK5 -1 // Verso l'esterno
#define PUMP_MAX_FLOW5 0 // Disabilitata
#define PUMP_IS_ON5 0 // Inizialmente spenta

// Pompa di scarico serbatoio 2 (non utilizzata)
#define PUMP_ID6 6 // Identificativo pompa
#define PUMP_FROM_TANK6 2 // Serbatoio di partenza
#define PUMP_TO_TANK6 -2 // Verso l'esterno
#define PUMP_MAX_FLOW6 0.0 // Disabilitata
#define PUMP_IS_ON6 0 // Inizialmente spenta

// Parametri del sistema
#define EVAP_COEFF 0.001  // Coefficiente di evaporazione
#define HEAT_CAPACITY_PER_L 4184.0

// Controlli scenario 3 - Focus sulla concentrazione
#define VOLUME_ENABLED 1          // Controllo volume abilitato
#define CONCENTRATION_ENABLED 1   // CONTROLLO CONCENTRAZIONE PRINCIPALE
#define TEMPERATURE_ENABLED 0     // Temperatura non critica in questo scenario
#define EMPTYING_ENABLED 0        // Svuotamento disabilitato
#define DIVISION_ENABLED 0    // Disabilitato il bilanciamento dei volumi

#endif // SCENARIO3_H