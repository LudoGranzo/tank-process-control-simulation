#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int id;
    double capacity;
    double volume;
    double temperature;
    double concentration;
    double prev_volume; // Volume al passo precedente
    double prev_concentration;
    double prev_temperature;
    int target_reached; // Flag per indicare se ha raggiunto l'obiettivo
} Tank;

typedef struct {
    int id;
    int from_tank;
    int to_tank;
    double max_flow;
    int is_on;
} Pump;

typedef struct {
    int id;
    int from_tank;
    int to_tank;
    double max_flow;
    int is_on;
} Valve;

typedef struct {
    int id;
    int tank_id;
    double power;
    double watt_per_degree;
    int is_on;
} Heater;


#endif // TYPES_H
