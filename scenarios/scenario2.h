#ifndef SCENARIO2_H
#define SCENARIO2_H

// SCENARIO 2: Controllo termico di un processo sensibile: i serbatoi devono mantenere la temperatura entro un certo range
// I fluidi in ingresso sono più freddi, quindi i riscaldatori devono attivarsi per mantenere la temperatura
// L'obiettivo è sia evitare raffreddamento eccessivo sia sovratemperatura
#define ID 1
#define INITIAL_VOLUME 20.0
#define INITIAL_CONCENTRATION 0.3
#define CAPACITY 100.0
#define TANK_SCOPE 100.0
#define MIN_CONCENTRATION 0.2
#define MAX_CONCENTRATION 0.8
#define INITIAL_TEMPERATURE 26.0  // Bassa per vedere il riscaldamento
#define TEMP_MIN1 22.0
#define TEMP_MAX1 25.0

#define ID2 2
#define INITIAL_VOLUME2 20.0
#define INITIAL_CONCENTRATION2 0.4
#define CAPACITY2 100.0
#define TANK_SCOPE2 100.0
#define MIN_CONCENTRATION2 0.3
#define MAX_CONCENTRATION2 0.7
#define INITIAL_TEMPERATURE2 20.0
#define TEMP_MIN2 24.0
#define TEMP_MAX2 27.0

// Temperature degli ingressi (acqua fredda dall'esterno)
#define INLET_TEMPERATURE1 18.0      // Acqua fredda in ingresso al TANK1
#define INLET_TEMPERATURE2 17.0      // Acqua fredda in ingresso al TANK2

// Valvola disabilitata
#define VALVE_ID 1
#define VALVE_FROM_TANK 1
#define VALVE_TO_TANK 2
#define VALVE_MAX_FLOW 0.0
#define VALVE_IS_ON 0

// Pompe di riempimento ATTIVE
#define PUMP_ID1 1
#define PUMP_FROM_TANK1 -1
#define PUMP_TO_TANK1 1
#define PUMP_MAX_FLOW1 15.0  // Alto per riempimento veloce
#define PUMP_IS_ON1 1        // ATTIVA
#define CONCENTRATION_IN1 0.3

#define PUMP_ID2 2
#define PUMP_FROM_TANK2 -1
#define PUMP_TO_TANK2 1
#define PUMP_MAX_FLOW2 12.0
#define PUMP_IS_ON2 1        // ATTIVA
#define CONCENTRATION_IN2 0.4

// Pompe di collegamento DISABILITATE
#define PUMP_ID3 3
#define PUMP_FROM_TANK3 1
#define PUMP_TO_TANK3 2
#define PUMP_MAX_FLOW3 0.0   // DISABILITATA
#define PUMP_IS_ON3 0

#define PUMP_ID4 4
#define PUMP_FROM_TANK4 2
#define PUMP_TO_TANK4 1
#define PUMP_MAX_FLOW4 0.0   // DISABILITATA
#define PUMP_IS_ON4 0

// Pompe di scarico DISABILITATE
#define PUMP_ID5 5
#define PUMP_FROM_TANK5 1
#define PUMP_TO_TANK5 -1
#define PUMP_MAX_FLOW5 0.0   // DISABILITATA
#define PUMP_IS_ON5 0

#define PUMP_ID6 6
#define PUMP_FROM_TANK6 2
#define PUMP_TO_TANK6 -2
#define PUMP_MAX_FLOW6 0.0   // DISABILITATA
#define PUMP_IS_ON6 0

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
    #define CONCENTRATION_IN8 0.6  // Concentrazione ALTA della fonte 8

#define EVAP_COEFF 0.0005    // Ridotta

// Focus su temperatura
#define VOLUME_ENABLED 1
#define CONCENTRATION_ENABLED 1  // Abilitata
#define TEMPERATURE_ENABLED 1
#define EMPTYING_ENABLED 0       // Svuotamento disabilitato

#endif // SCENARIO2_H