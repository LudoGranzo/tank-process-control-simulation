#ifndef CONSTANTS_H
#define CONSTANTS_H

//Valori di simulazione iniziali serbatoio 1
#define ID 1
#define INITIAL_VOLUME 0
#define INITIAL_CONCENTRATION 0.2
#define INITIAL_TEMPERATURE 25.0
#define CAPACITY 100.0
#define TANK_SCOPE 80.0 //Volume obiettivo del serbatoio 

//Valori inziali serbatoio 2
#define ID2 2
#define INITIAL_VOLUME2 30.0
#define INITIAL_CONCENTRATION2 0.6
#define INITIAL_TEMPERATURE2 20.0
#define CAPACITY2 100.0
#define TANK_SCOPE2 80.0 //Volume obiettivo del serbatoio

//Valori di simulazione iniziali valvola 1
#define VALVE_ID 1
#define VALVE_FROM_TANK 1 // Serbatoio di partenza
#define VALVE_TO_TANK 2 // Serbatoio di arrivo
#define VALVE_MAX_FLOW 10.0 // Flusso massimo della valvola
#define VALVE_IS_ON 0 // Stato iniziale della valvola (spenta)
#define MIN_CONCENTRATION 0.3
#define MAX_CONCENTRATION 0.5

// Parametri del sistema
#define EVAP_COEFF 0.001

#endif // CONSTANTS_H

