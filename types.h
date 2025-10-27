#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int id; // Identificativo serbatoio
    double capacity; // Capacità MASSIMA del serbatoio
    double volume;  // Volume attuale del serbatoio
    double temperature; // Temperatura attuale del serbatoio
    double concentration; // Concentrazione attuale del serbatoio
    double prev_volume; // Volume al passo precedente
    double prev_concentration; // Concentrazione al passo precedente
    double prev_temperature; // Temperatura al passo precedente
    int target_reached; // Flag per indicare se ha raggiunto l'obiettivo
} Tank;

typedef struct {
    int id; // Identificativo pompa 
    int from_tank; // Serbatoio di partenza
    int to_tank; // Serbatoio di arrivo
    double max_flow; // Flusso massimo della pompa
    int is_on; // Stato della pompa (accesa/spenta)
} Pump;

typedef struct {
    int id; // Identificativo valvola
    int from_tank; // Serbatoio di partenza
    int to_tank; // Serbatoio di arrivo
    double max_flow; // Flusso massimo della valvola
    int is_on; // Stato della valvola (accesa/spenta)
} Valve;

typedef struct {
    int id; // Identificativo riscaldatore
    int tank_id; // Serbatoio associato
    double power; // Potenza del riscaldatore
    double watt_per_degree; // Watt per grado di temperatura
    int is_on; // Stato del riscaldatore (acceso/spento)
} Heater;


#endif // TYPES_H
