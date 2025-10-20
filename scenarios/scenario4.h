#ifndef SCENARIO4_H
#define SCENARIO4_H

// SCENARIO 4: Riempimento di un serbatoio e gestione della divisione del fluido tra i serbatoi con scarico.
// Il sistema deve dividere equamente il fluido all'interno dei serbatoi, successivamente scaricare.
// Le variazioni di concentrazione e temperatura sono visualizzate ma non è attivo il controllo (limitazione in range).

// Valori di simulazione iniziali serbatoio 1
#define ID 1 // Identificativo serbatoio
#define INITIAL_VOLUME 0.0 // Volume iniziale 
#define INITIAL_CONCENTRATION 0.5 // Valore di concentrazione liquido già presente nel serbatoio (0 perchè non c'è liquido).
#define CAPACITY 100.0 // Capacità MASSIMA del serbatoio
#define TANK_SCOPE 100.0 // Volume obiettivo del serbatoio
#define INITIAL_TEMPERATURE 23.0 // Valore di temperatura liquido già presente nel serbatoio
#define TEMP_MIN1 22.0 // Valore MINIMO del range di temperatura
#define TEMP_MAX1 25.0 // Valore MASSIMO del range di temperatura

// Valori di simulazione iniziali serbatoio 2
#define ID2 2 // Identificativo serbatoio
#define INITIAL_VOLUME2 0.0 // Volume iniziale gia presente nel serbatoio
#define INITIAL_CONCENTRATION2 0.0 // Valore di concentrazione del liquido già presente nel serbatoio
#define CAPACITY2 200.0 // Capacità MASSIMA del serbatoio
#define TANK_SCOPE2 0.0 // Volume obiettivo del serbatoio
#define INITIAL_TEMPERATURE2 26.0 // Valore di temperatura liquido già presente nel serbatoio
#define TEMP_MIN2 25.0 // Valore MINIMO del range di temperatura
#define TEMP_MAX2 28.0 // Valore MASSIMO del range di temperatura

// Valori di temperature fluidi in ingresso
#define INLET_TEMPERATURE1 23.0 // Temperatura fluidi in ingresso al TANK1
#define INLET_TEMPERATURE2 26.0 // Temperatura fluidi in ingresso al TANK2

// Valori di simulazione iniziali prima pompa in ingresso al serbatoio 1
#define PUMP_ID1 1 // Identificativo pompa numero 1
#define PUMP_FROM_TANK1 -1 // Fonte esterna
#define PUMP_TO_TANK1 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW1 10.0 // Flusso massimo della pompa
#define PUMP_IS_ON1 0 // Stato iniziale della pompa (spenta)
#define CONCENTRATION_IN1 0.6  // Concentrazione del primo fluido entrante nel TANK 1

// Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 1
#define PUMP_ID2 2 // Identificativo pompa numero 2
#define PUMP_FROM_TANK2 -1 // Serbatoio di partenza
#define PUMP_TO_TANK2 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW2 10.0 // Flusso massimo della pompa
#define PUMP_IS_ON2 0 // Stato iniziale della pompa (spenta)
#define CONCENTRATION_IN2 0.3  // Concentrazione del secondo fluido entrante in TANK 1

// Valori di simulazione iniziali pompa di collegamento tra tank1 e tank2
#define PUMP_ID3 3 // Identificativo pompa numero 3
#define PUMP_FROM_TANK3 1 // Serbatoio di partenza
#define PUMP_TO_TANK3 2 // Serbatoio di arrivo
#define PUMP_MAX_FLOW3 3.0 // Flusso massimo della pompa
#define PUMP_IS_ON3 0 // Stato iniziale della pompa (spenta)

// Valori di simulazione iniziali pompa di collegamento tra tank2 e tank1
#define PUMP_ID4 4 // Identificativo pompa numero 4
#define PUMP_FROM_TANK4 2 // Serbatoio di partenza
#define PUMP_TO_TANK4 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW4 3.0 // Flusso massimo della pompa
#define PUMP_IS_ON4 0 // Stato iniziale della pompa (spenta)

// Valori di simulazione pompa di scarico serbatoio 1
#define PUMP_ID5 5 // Identificativo pompa numero 5
#define PUMP_FROM_TANK5 1 // Serbatoio di partenza
#define PUMP_TO_TANK5 -1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW5 8.0 // Flusso massimo della pompa
#define PUMP_IS_ON5 0 // Stato iniziale della pompa (spenta)

// Valori di simulazione pompa di scarico serbatoio 2
#define PUMP_ID6 6 // Identificativo pompa numero 6
#define PUMP_FROM_TANK6 2 // Serbatoio di partenza
#define PUMP_TO_TANK6 -2 // Serbatoio di arrivo
#define PUMP_MAX_FLOW6 8.0 // Flusso massimo della pompa
#define PUMP_IS_ON6 0 // Stato iniziale della pompa (spenta)

// Valori di simulazione iniziali prima pompa in ingresso al serbatoio 2
#define PUMP_ID7 7 // Identificativo pompa numero 7
#define PUMP_FROM_TANK7 -2 // Fonte esterna diversa
#define PUMP_TO_TANK7 2 // Verso serbatoio 2
#define PUMP_MAX_FLOW7 10.0 // Flusso massimo
#define PUMP_IS_ON7 0 // Inizialmente spenta
#define CONCENTRATION_IN7 0.5 // Concentrazione del primo fluido entrante in TANK 2

// Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 2
#define PUMP_ID8 8 // Identificativo pompa numero 8
#define PUMP_FROM_TANK8 -2 // Fonte esterna diversa
#define PUMP_TO_TANK8 2 // Verso serbatoio 2
#define PUMP_MAX_FLOW8 10.0 // Flusso massimo
#define PUMP_IS_ON8 0 // Inizialmente spenta
#define CONCENTRATION_IN8 0.6 // Concentrazione del secondo fluido entrante in TANK 2

// Parametri del sistema
#define EVAP_COEFF 0.001

//Controlli scenario 1: Solo volume
#define VOLUME_ENABLED 1        // Abilitato il controllo del volume
#define CONCENTRATION_ENABLED 0 // Abilitato il controllo della concentrazione
#define TEMPERATURE_ENABLED 0   // Abilitato il controllo della temperatura
#define EMPTYING_ENABLED 1      // Svuotamento abilitato
#define DIVISION_ENABLED 1      //divisione tra i serbatoi abilitato

#endif // SCENARIO4_H

