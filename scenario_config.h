#ifndef SCENARIO_CONFIG_H
#define SCENARIO_CONFIG_H

// Inclusione condizionale degli scenari basata su flag di compilazione
#ifdef SCENARIO_1
    #include "scenarios/scenario1.h"
#else
    #define ID 1
    #define INITIAL_VOLUME 0.0
    #define INITIAL_CONCENTRATION 0.2
    #define INITIAL_TEMPERATURE 25.0
    #define CAPACITY 100.0
    #define TANK_SCOPE 100.0 //Volume obiettivo del serbatoio 
    #define MIN_CONCENTRATION 0.3
    #define MAX_CONCENTRATION 0.5
    #define TEMPERATURE_SCOPE 30.0

    //Valori inziali serbatoio 2
    #define ID2 2
    #define INITIAL_VOLUME2 50.0
    #define INITIAL_CONCENTRATION2 0.6
    #define INITIAL_TEMPERATURE2 20.0
    #define CAPACITY2 200.0
    #define TANK_SCOPE2 100.0 //Volume obiettivo del serbatoio
    #define MIN_CONCENTRATION2 0.4
    #define MAX_CONCENTRATION2 0.6
    #define TEMPERATURE_SCOPE2 25.0

    //Valori di simulazione iniziali valvola 1
    #define VALVE_ID 1
    #define VALVE_FROM_TANK 1 // Serbatoio di partenza
    #define VALVE_TO_TANK 2 // Serbatoio di arrivo
    #define VALVE_MAX_FLOW 10.0 // Flusso massimo della valvola
    #define VALVE_IS_ON 0 // Stato iniziale della valvola (spenta)

    // Valori di simulazione iniziali pompa in ingresso1
    #define PUMP_ID1 1
    #define PUMP_FROM_TANK1 -1 // Serbatoio di partenza
    #define PUMP_TO_TANK1 1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW1 10.0 // Flusso massimo della pompa
    #define PUMP_IS_ON1 0 // Stato iniziale della pompa (spenta)
    #define CONCENTRATION_IN1 0.4  // concentrazione del fluido che entra in TANK 1 (P11)

    // Valori di simulazione iniziali pompa in ingresso2
    #define PUMP_ID2 2
    #define PUMP_FROM_TANK2 -2 // Serbatoio di partenza
    #define PUMP_TO_TANK2 2 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW2 7.0 // Flusso massimo della pompa
    #define PUMP_IS_ON2 0 // Stato iniziale della pompa (spenta)
    #define CONCENTRATION_IN2 0.4  // concentrazione del fluido che entra in TANK 2 (P22)

    // Valori di simulazione iniziali pompa di collegamento tra tank1 e tank2
    #define PUMP_ID3 3
    #define PUMP_FROM_TANK3 1 // Serbatoio di partenza
    #define PUMP_TO_TANK3 2 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW3 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON3 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione iniziali pompa di collegamento tra tank2 e tank1
    #define PUMP_ID4 4
    #define PUMP_FROM_TANK4 2 // Serbatoio di partenza
    #define PUMP_TO_TANK4 1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW4 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON4 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione di scarico pompa 1
    #define PUMP_ID5 5
    #define PUMP_FROM_TANK5 1 // Serbatoio di partenza
    #define PUMP_TO_TANK5 -1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW5 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON5 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione di scarico pompa 2
    #define PUMP_ID6 6
    #define PUMP_FROM_TANK6 2 // Serbatoio di partenza
    #define PUMP_TO_TANK6 -2 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW6 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON6 0 // Stato iniziale della pompa (spenta)


    // Parametri del sistema
    #define EVAP_COEFF 0.001
    
    //Controlli scenario default
    #define VOLUME_ENABLED 1      // Abilitato il controllo del volume
    #define CONCENTRATION_ENABLED 1 // Abilitato il controllo della concentrazione
    #define TEMPERATURE_ENABLED 1   // Abilitato il controllo della temperatura
    #define EMPTYING_ENABLED 0      // Svuotamento non abilitato
    #endif

    #endif // SCENARIO_CONFIG_H


