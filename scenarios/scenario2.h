#ifndef SCENARIO2_H
#define SCENARIO2_H

// SCENARIO 2: Controllo termico di un processo sensibile: i serbatoi devono mantenere la temperatura entro un certo range
// I fluidi in ingresso sono più freddi, quindi i riscaldatori devono attivarsi per mantenere la temperatura
// L'obiettivo è sia evitare raffreddamento eccessivo sia sovratemperatura
#define ID 1 // Identificativo serbatoio
#define INITIAL_VOLUME 20.0 // Volume iniziale gia presente nel serbatoio
#define INITIAL_CONCENTRATION 0.3 // Valore di concentrazione del liquido già presente nel serbatoio
#define CAPACITY 100.0 // Capacità MASSIMA del serbatoio
#define TANK_SCOPE 100.0 // Volume obiettivo del serbatoio
#define INITIAL_TEMPERATURE 26.0  // Temperatura iniziale del liquido già presente nel serbatoio
#define TEMP_MIN1 22.0 // Valore MINIMO del range di temperatura
#define TEMP_MAX1 25.0 // Valore MASSIMO del range di temperatura

#define ID2 2 // Identificativo serbatoio
#define INITIAL_VOLUME2 20.0 // Volume iniziale gia presente nel serbatoio
#define INITIAL_CONCENTRATION2 0.4 // Valore di concentrazione del liquido già presente nel serbatoio
#define CAPACITY2 100.0 // Capacità MASSIMA del serbatoio
#define TANK_SCOPE2 100.0 // Volume obiettivo del serbatoio
#define INITIAL_TEMPERATURE2 20.0 // Temperatura iniziale del liquido già presente nel serbatoio
#define TEMP_MIN2 21.0 // Valore MINIMO del range di temperatura
#define TEMP_MAX2 24.0 // Valore MASSIMO del range di temperatura

// Valori di temperatura fluidi in ingresso (più freddi per richiedere riscaldamento)
#define INLET_TEMPERATURE1 18.0 // Temperatura fluidi in ingresso al TANK1
#define INLET_TEMPERATURE2 17.0 // Temperatura fluidi in ingresso al TANK2

// Valori di simulazione iniziali prima pompa in ingresso al serbatoio 1
#define PUMP_ID1 1 // Identificativo pompa
#define PUMP_FROM_TANK1 -1 // Fonte esterna
#define PUMP_TO_TANK1 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW1 15.0 // Flusso massimo della pompa
#define PUMP_IS_ON1 1 // Stato iniziale della pompa (ATTIVA)  
#define CONCENTRATION_IN1 0.3 // Concentrazione del primo fluido entrante nel TANK 1

// Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 1
#define PUMP_ID2 2 // Identificativo pompa
#define PUMP_FROM_TANK2 -1 // Fonte esterna
#define PUMP_TO_TANK2 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW2 12.0 // Flusso massimo della pompa
#define PUMP_IS_ON2 1 // Stato iniziale della pompa (ATTIVA)
#define CONCENTRATION_IN2 0.4 // Concentrazione del secondo fluido entrante nel TANK 1

// Pompe di collegamento DISABILITATE
#define PUMP_ID3 3 // Identificativo pompa
#define PUMP_FROM_TANK3 1 // Serbatoio di partenza
#define PUMP_TO_TANK3 2 // Serbatoio di arrivo
#define PUMP_MAX_FLOW3 0.0 // Flusso massimo della pompa (DISABILITATA)
#define PUMP_IS_ON3 0 // Stato iniziale della pompa (SPENTA)

#define PUMP_ID4 4 // Identificativo pompa
#define PUMP_FROM_TANK4 2 // Serbatoio di partenza
#define PUMP_TO_TANK4 1 // Serbatoio di arrivo
#define PUMP_MAX_FLOW4 0.0 // Flusso massimo della pompa (DISABILITATA)
#define PUMP_IS_ON4 0 // Stato iniziale della pompa (SPENTA)

// Valori di simulazione iniziali pompa di scarico serbatoio 1
#define PUMP_ID5 5 // Identificativo pompa
#define PUMP_FROM_TANK5 1 // Serbatoio di partenza
#define PUMP_TO_TANK5 -1 // Scarico esterno
#define PUMP_MAX_FLOW5 0.0 // Flusso massimo della pompa di scarico (DISABILITATA)
#define PUMP_IS_ON5 0 // Stato iniziale della pompa (SPENTA)

// Valori di simulazione iniziali pompa di scarico serbatoio 2
#define PUMP_ID6 6 // Identificativo pompa
#define PUMP_FROM_TANK6 2 // Serbatoio di partenza
#define PUMP_TO_TANK6 -2 // Scarico esterno
#define PUMP_MAX_FLOW6 0.0 // Flusso massimo della pompa di scarico (DISABILITATA)
#define PUMP_IS_ON6 0 // Stato iniziale della pompa (SPENTA)

// Valori di simulazione iniziali prima pompa in ingresso al serbatoio 2
#define PUMP_ID7 7 // Identificativo pompa numero 7
#define PUMP_FROM_TANK7 -2 // Fonte esterna
#define PUMP_TO_TANK7 2 // serbatoio di arrivo
#define PUMP_MAX_FLOW7 4.0 // Flusso massimo
#define PUMP_IS_ON7 0 // Inizialmente spenta
#define CONCENTRATION_IN7 0.5 // Concentrazione del primo fluido entrante in TANK 2

// Valori di simulazione iniziali seconda pompa in ingresso al serbatoio 2
#define PUMP_ID8 8 // Identificativo pompa numero 8
#define PUMP_FROM_TANK8 -2 // Fonte esterna diversa
#define PUMP_TO_TANK8 2 // serbatoio di arrivo
#define PUMP_MAX_FLOW8 3.0 // Flusso massimo
#define PUMP_IS_ON8 0 // Inizialmente spenta
#define CONCENTRATION_IN8 0.6 // Concentrazione del secondo fluido entrante in TANK 2

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

#define EVAP_COEFF 0.0005 // Ridotta

// Abilitazioni funzionamenti
#define VOLUME_ENABLED 1 // Abilitato il controllo del volume
#define CONCENTRATION_ENABLED 0 // Disabilitato il controllo della concentrazione
#define TEMPERATURE_ENABLED 1 // Abilitato il controllo della temperatura
#define EMPTYING_ENABLED 0 // Disabilitato lo svuotamento
#define DIVISION_ENABLED 0 // Disabilitato il bilanciamento dei volumi

#endif // SCENARIO2_H