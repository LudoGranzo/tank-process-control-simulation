#ifndef SCENARIO_CONFIG_H
#define SCENARIO_CONFIG_H

// Inclusione condizionale degli scenari basata su flag di compilazione
#ifdef SCENARIO_1
    #include "scenarios/scenario1.h"
#elif defined SCENARIO_2
    #include "scenarios/scenario2.h"
#elif defined SCENARIO_3
    #include "scenarios/scenario3.h"
#elif defined SCENARIO_4
    #include "scenarios/scenario4.h"
#else
    //Scenario di default: Solamente riempimento, senza alcuna limitazione di temperatura e concentrazione.

    // Valori iniziali serbatoio 1
    #define ID 1 // Identificativo serbatoio 1
    #define INITIAL_VOLUME 0.0 // Volume iniziale
    #define INITIAL_CONCENTRATION 0.0 // Concentrazione iniziale
    #define CAPACITY 100.0 // Capacità massima
    #define TANK_SCOPE 100.0 // Volume obiettivo del serbatoio
    #define MIN_CONCENTRATION 0.3 // Concentrazione minima
    #define MAX_CONCENTRATION 0.5 // Concentrazione massima
    #define INITIAL_TEMPERATURE 20.0 // Temperatura iniziale
    #define TEMP_MIN1 22.0 // Temperatura minima serbatoio 1
    #define TEMP_MAX1 25.0 // Temperatura massima serbatoio 1

    //Valori inziali serbatoio 2
    #define ID2 2 // Identificativo serbatoio 2
    #define INITIAL_VOLUME2 50.0 // Volume iniziale
    #define INITIAL_CONCENTRATION2 0.6 // Concentrazione iniziale
    #define CAPACITY2 200.0 // Capacità massima
    #define TANK_SCOPE2 100.0 // Volume obiettivo del serbatoio
    #define MIN_CONCENTRATION2 0.4 // Valore MINIMO del range di concentrazione
    #define MAX_CONCENTRATION2 0.6 // Valore MASSIMO del range di concentrazione
    #define INITIAL_TEMPERATURE2 22.0 // Temperatura iniziale
    #define TEMP_MIN2 25.0 // Temperatura minima serbatoio 2
    #define TEMP_MAX2 28.0 // Temperatura massima serbatoio 2   

    //Valori di temperature fluidi in ingresso
    #define INLET_TEMPERATURE1 18.0 // Acqua fredda in ingresso al TANK1
    #define INLET_TEMPERATURE2 20.0 // Acqua fredda in ingresso al TANK2

    // Valori di simulazione iniziali prima pompa in ingresso al serbatoio 1
    #define PUMP_ID1 1 // Identificativo pompa numero 1
    #define PUMP_FROM_TANK1 -1 // Serbatoio di partenza
    #define PUMP_TO_TANK1 1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW1 10.0 // Flusso massimo della pompa
    #define PUMP_IS_ON1 0 // Stato iniziale della pompa (spenta)
    #define CONCENTRATION_IN1 0.4  // Concentrazione del fluido che entra in TANK 1 dall'esterno

    // Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 1
    #define PUMP_ID2 2 // Identificativo pompa numero 2 
    #define PUMP_FROM_TANK2 -1 // Serbatoio di partenza
    #define PUMP_TO_TANK2 1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW2 7.0 // Flusso massimo della pompa
    #define PUMP_IS_ON2 0 // Stato iniziale della pompa (spenta)
    #define CONCENTRATION_IN2 0.4  // Concentrazione del fluido che entra in TANK 1 dall'esterno

    // Valori di simulazione iniziali pompa di collegamento tra tank1 e tank2
    #define PUMP_ID3 3 // Identificativo pompa numero 3
    #define PUMP_FROM_TANK3 1 // Serbatoio di partenza
    #define PUMP_TO_TANK3 2 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW3 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON3 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione iniziali pompa di collegamento tra tank2 e tank1
    #define PUMP_ID4 4 // Identificativo pompa numero 4
    #define PUMP_FROM_TANK4 2 // Serbatoio di partenza
    #define PUMP_TO_TANK4 1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW4 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON4 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione di scarico pompa 1
    #define PUMP_ID5 5 // Identificativo pompa numero 5
    #define PUMP_FROM_TANK5 1 // Serbatoio di partenza
    #define PUMP_TO_TANK5 -1 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW5 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON5 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione di scarico pompa 2
    #define PUMP_ID6 6 // Identificativo pompa numero 6
    #define PUMP_FROM_TANK6 2 // Serbatoio di partenza
    #define PUMP_TO_TANK6 -2 // Serbatoio di arrivo
    #define PUMP_MAX_FLOW6 8.0 // Flusso massimo della pompa
    #define PUMP_IS_ON6 0 // Stato iniziale della pompa (spenta)

    // Valori di simulazione iniziali prima pompa in ingresso al serbatoio 2
    #define PUMP_ID7 7 // Identificativo pompa numero 7
    #define PUMP_FROM_TANK7 -2  // Fonte esterna 
    #define PUMP_TO_TANK7 2     // Verso serbatoio 2
    #define PUMP_MAX_FLOW7 4.0  // Flusso massimo
    #define PUMP_IS_ON7 0       // Stato iniziale della pompa (spenta)
    #define CONCENTRATION_IN7 0.5  // Concentrazione del fluido che entra in TANK 2 dall'esterno

    // Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 2
    #define PUMP_ID8 8 // Identificativo pompa numero 8
    #define PUMP_FROM_TANK8 -2  // Fonte esterna 
    #define PUMP_TO_TANK8 2     // Verso serbatoio 2
    #define PUMP_MAX_FLOW8 3.0  // Flusso massimo
    #define PUMP_IS_ON8 0      // Stato iniziale della pompa (spenta)
    #define CONCENTRATION_IN8 0.6  // Concentrazione del fluido che entra in TANK 2 dall'esterno

    // Valori di simulazione iniziali riscaldatore serbatoio 1
    #define HEATER_ID1 1 // Identificativo riscaldatore
    #define HEATER_TANK_ID1 1 // Serbatoio associato
    #define HEATER_POWER1 200 // Potenza del riscaldatore in Watt
    #define HEATER_WATT_PER_DEGREE1 10 // Watt necessari per aumentare di 1 grado la temperatura del liquido
    #define HEATER_IS_ON1 0 // Stato iniziale del riscaldatore (spento)

    // Valori di simulazione iniziali riscaldatore serbatoio 2
    #define HEATER_ID2 2 // Identificativo riscaldatore
    #define HEATER_TANK_ID2 2 // Serbatoio associato
    #define HEATER_POWER2 200 // Potenza del riscaldatore in Watt
    #define HEATER_WATT_PER_DEGREE2 10 // Watt necessari per aumentare di 1 grado la temperatura del liquido
    #define HEATER_IS_ON2 0 // Stato iniziale del riscaldatore (spento)

    // Parametri del sistema
    #define EVAP_COEFF 0.001
    // Capacità termica per litro (acqua ~ 4184 J/(kg·°C) e densità ~1 kg/L)
    #define HEAT_CAPACITY_PER_L 4184.0
    
    //Controlli scenario default
    #define VOLUME_ENABLED 1 // Abilitato il controllo del volume
    #define CONCENTRATION_ENABLED 0 // Controllo concentrazione disabilitato
    #define TEMPERATURE_ENABLED 0 // Controllo temperatura disabilitato
    #define EMPTYING_ENABLED 0 // Svuotamento non abilitato
    #define DIVISION_ENABLED 0 // divisione tra i serbatoi abilitato
    #endif

    #endif // SCENARIO_CONFIG_H


