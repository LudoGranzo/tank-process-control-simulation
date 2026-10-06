# Multi-Tank Process Control Simulation (C)

Discrete-time simulation of an industrial multi-tank process with **volume, temperature and concentration control**, written in C.
Team project for the *Industrial Informatics Laboratory* (BSc Mechatronics Engineering, University of Padua, a.y. 2024/25).

**Team:** Ludovico Granzotto, Simone Cordioli — all parts developed jointly (process model, control logic, build/test setup).

---

## What it does

The plant is made of two tanks connected by pumps and valves, fed by external sources and equipped with heaters. At every time step the program:

1. **updates the process model** – mass balance with pump flows and evaporation, mixing equations for concentration and temperature:
   - `V(t+1) = V(t) + (Q_in − Q_out)·Δt − k_evap·V(t)`
   - `C(t+1) = [C(t)·V(t) + Σ Q_in·C_in·Δt] / V(t+1)`
   - `T(t+1) = [T(t)·V(t) + Σ Q_in·T_in·Δt + Q_heat·Δt] / V(t+1)`
2. **runs the control logic** – decides which pumps, valves and heaters are ON/OFF;
3. **prints the plant state** (volumes, concentrations, temperatures, actuator states).

## Scenarios

The same source code is compiled into four executables; each scenario is selected with a preprocessor macro (`-DSCENARIO_X`).

| # | Scenario | Control objective |
|---|----------|-------------------|
| 1 | **Batch fill & discharge** | Fill the tanks to the target level, mix, then empty them. Concentration and temperature are monitored. |
| 2 | **Thermal control** | Inlet fluids are colder than the process: heaters use on/off control with hysteresis to keep each tank inside its temperature band, avoiding both under- and over-temperature. |
| 3 | **Concentration control with two sources** | Each tank is fed by two sources with different concentrations. The controller selects which inlet pump to run to reach the target volume while keeping the concentration within its range (evaporation losses included). |
| 4 | **Fluid splitting & emptying** | One tank is filled, then a transfer pump balances the volume between the two tanks (5 L tolerance) before both are emptied. |

## Project structure

```
main.c              entry point and simulation loop
simulation.c        process model and control logic
simulation.h        function prototypes
types.h             data types (Tank, Pump, Valve, Heater)
constants.h         shared constants
scenario_config.h   selects the active scenario configuration
scenarios/          per-scenario configuration headers
Makefile            build, run and test targets
test_scenarios.sh   interactive script to build, run and compare scenarios
```

## Build & run

Requirements: GCC (C99) and `make` on Linux/Unix.

```bash
make                 # default build
make run_scenario1   # build and run one scenario (1–4)
make all_scenarios   # build all four executables
make test_all        # run every scenario and save results_scenarioX.txt
make clean
```

Compiler flags: `-Wall -Wextra -std=c99 -g`.

## Skills involved

C programming · discrete-time process modelling · on/off and hysteresis control · actuator logic (pumps, valves, heaters) · conditional compilation · Makefile · Git team workflow
