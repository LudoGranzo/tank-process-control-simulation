#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int id;
    double capacity;
    double volume;
    double temperature;
    double concentration;
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
    int tank_id;
    double volume_reading;
    double temperature_reading;
    double concentration_reading;
} Sensors;


#endif // TYPES_H
