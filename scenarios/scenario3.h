#ifndef SCENARIO3_H
#define SCENARIO3_H

// SCENARIO 3: Controllo della concentrazione con due fonti
// Un serbatoio viene alimentato costantemente da due fonti con diverse concentrazioni.
// L'obiettivo è mantenere la concentrazione nel serbatoio entro un range specifico.

// Serbatoio principale (Tank 1) - serbatoio di uscita
#define ID 1
#define INITIAL_VOLUME 0
#define INITIAL_CONCENTRATION 0  // Concentrazione iniziale nel range target
#define CAPACITY 150.0
#define TANK_SCOPE 100.0  // Volume obiettivo 
#define MIN_CONCENTRATION 0.3   // Range di concentrazione target
#define MAX_CONCENTRATION 0.4   // Range di concentrazione target
#define INITIAL_TEMPERATURE 22.0
#define TEMP_MIN1 20.0
#define TEMP_MAX1 25.0

// Serbatoio secondario (Tank 2) - anche con controllo concentrazione
#define ID2 2
#define INITIAL_VOLUME2 0
#define INITIAL_CONCENTRATION2 0  // Concentrazione iniziale nel range target
#define CAPACITY2 150.0
#define TANK_SCOPE2 80.0  // Volume obiettivo serbatoio 2
#define MIN_CONCENTRATION2 0.4   // Range di concentrazione target serbatoio 2
#define MAX_CONCENTRATION2 0.6   // Range di concentrazione target serbatoio 2
#define INITIAL_TEMPERATURE2 20.0
#define TEMP_MIN2 20.0
#define TEMP_MAX2 25.0

// Temperature dei fluidi in ingresso
#define INLET_TEMPERATURE1 22.0  // Temperatura fonte 1
#define INLET_TEMPERATURE2 22.0  // Temperatura fonte 2

// Valvola (non utilizzata in questo scenario)
#define VALVE_ID 1
#define VALVE_FROM_TANK 1
#define VALVE_TO_TANK 2
#define VALVE_MAX_FLOW 0.0  // Disabilitata
#define VALVE_IS_ON 0

// FONTE 1: Pompa con concentrazione bassa (P1-1)
#define PUMP_ID1 1
#define PUMP_FROM_TANK1 -1  // Fonte esterna
#define PUMP_TO_TANK1 1     // Verso serbatoio principale
#define PUMP_MAX_FLOW1 8.0  // Flusso massimo
#define PUMP_IS_ON1 1       // Inizialmente attiva
#define CONCENTRATION_IN1 0.3  // Concentrazione BASSA della fonte 1

// FONTE 2: Pompa con concentrazione alta (P2-1)
#define PUMP_ID2 2
#define PUMP_FROM_TANK2 -1  // Fonte esterna diversa
#define PUMP_TO_TANK2 1     // Verso serbatoio principale
#define PUMP_MAX_FLOW2 5.0  // Flusso massimo
#define PUMP_IS_ON2 1       // Inizialmente attiva
#define CONCENTRATION_IN2 0.8  // Concentrazione ALTA della fonte 2

//Pompa in entrata serbatoio 2
#define PUMP_ID7 7
#define PUMP_FROM_TANK7 -2  // Fonte esterna diversa
#define PUMP_TO_TANK7 2     // Verso serbatoio 2
#define PUMP_MAX_FLOW7 4.0  // Flusso massimo
#define PUMP_IS_ON7 1       // Inizialmente attiva
#define CONCENTRATION_IN7 0.5  // Concentrazione MEDIA della fonte 7

//Seconda pompa in ingresso serbatoio 2
#define PUMP_ID8 8
#define PUMP_FROM_TANK8 -2  // Fonte esterna diversa
#define PUMP_TO_TANK8 2     // Verso serbatoio 2
#define PUMP_MAX_FLOW8 3.0  // Flusso massimo
#define PUMP_IS_ON8 1       // Inizialmente attiva
#define CONCENTRATION_IN8 0.8  // Concentrazione ALTA della fonte 8

// Pompe di collegamento (non utilizzate)
#define PUMP_ID3 3
#define PUMP_FROM_TANK3 1
#define PUMP_TO_TANK3 2
#define PUMP_MAX_FLOW3 0.0
#define PUMP_IS_ON3 0

#define PUMP_ID4 4
#define PUMP_FROM_TANK4 2
#define PUMP_TO_TANK4 1
#define PUMP_MAX_FLOW4 0.0
#define PUMP_IS_ON4 0

// POMPA DI SCARICO del serbatoio principale (P1-0)
#define PUMP_ID5 5
#define PUMP_FROM_TANK5 1   // Dal serbatoio principale
#define PUMP_TO_TANK5 -1    // Verso l'esterno
#define PUMP_MAX_FLOW5 6.0  // Flusso di scarico
#define PUMP_IS_ON5 1       // Inizialmente attiva per mantenere equilibrio

// Pompa di scarico serbatoio 2 (non utilizzata)
#define PUMP_ID6 6
#define PUMP_FROM_TANK6 2
#define PUMP_TO_TANK6 -2
#define PUMP_MAX_FLOW6 0.0
#define PUMP_IS_ON6 0

// Parametri del sistema
#define EVAP_COEFF 0.001  // Coefficiente di evaporazione
#define HEAT_CAPACITY_PER_L 4184.0

// Controlli scenario 3 - Focus sulla concentrazione
#define VOLUME_ENABLED 1          // Controllo volume abilitato
#define CONCENTRATION_ENABLED 1   // CONTROLLO CONCENTRAZIONE PRINCIPALE
#define TEMPERATURE_ENABLED 0     // Temperatura non critica in questo scenario
#define EMPTYING_ENABLED 0        // Svuotamento disabilitato

#endif // SCENARIO3_H