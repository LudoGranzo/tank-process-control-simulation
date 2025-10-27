# Makefile per la simulazione dei serbatoi con scenari multipli

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -I. -Iscenarios

# File sorgenti comuni
SOURCES = main.c simulation.c
OBJECTS = $(SOURCES:.c=.o)

# Target di default (scenario originale)
TARGET = simulation
TARGET_S1 = simulation_scenario1
TARGET_S2 = simulation_scenario2  
TARGET_S3 = simulation_scenario3
TARGET_S4 = simulation_scenario4
LOG_DIR = logs

# Regola di default
all: $(TARGET)

# Crea la cartella log se non esiste
$(LOG_DIR):
	mkdir -p $(LOG_DIR)

# Compilazione scenario di default (usa constants.h)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compilazione Scenario 1 - Test Riempimento Veloce
scenario1: $(TARGET_S1)
$(TARGET_S1): CFLAGS += -DSCENARIO_1
$(TARGET_S1): clean_objects
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SOURCES)

# Compilazione Scenario 2 - Test Stress Alta Evaporazione  
scenario2: $(TARGET_S2)
$(TARGET_S2): CFLAGS += -DSCENARIO_2
$(TARGET_S2): clean_objects
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SOURCES)

# Compilazione Scenario 3 - Test Precisione
scenario3: $(TARGET_S3)
$(TARGET_S3): CFLAGS += -DSCENARIO_3
$(TARGET_S3): clean_objects
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SOURCES)

# Compilazione Scenario 4 - Test Divisione e Svuotamento
scenario4: $(TARGET_S4)
$(TARGET_S4): CFLAGS += -DSCENARIO_4
$(TARGET_S4): clean_objects
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SOURCES)

# Compilazione di tutti gli scenari
all_scenarios: $(TARGET_S1) $(TARGET_S2) $(TARGET_S3) $(TARGET_S4)

# Regole per i file oggetto
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Pulizia file oggetto (per evitare conflitti tra scenari)
clean_objects:
	rm -f $(OBJECTS)

# Pulizia completa
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET_S1) $(TARGET_S2) $(TARGET_S3) $(TARGET_S4)

# Pulizia completa inclusi i log
clean_all: clean
	rm -rf $(LOG_DIR) results_scenario*.txt
# Esecuzione rapida degli scenari
run_scenario1: $(TARGET_S1)
	./$(TARGET_S1)

run_scenario2: $(TARGET_S2)
	./$(TARGET_S2)

run_scenario3: $(TARGET_S3)
	./$(TARGET_S3)

run_scenario4: $(TARGET_S4)
	./$(TARGET_S4)

# Test di tutti gli scenari
test_all: all_scenarios | $(LOG_DIR)
	@echo "=== ESECUZIONE SCENARIO 1: Gestione di una fase batch con riempimento e scarico. ==="
	./$(TARGET_S1) > $(LOG_DIR)/scenario1_output.log 2>&1 || true
	@echo "=== ESECUZIONE SCENARIO 2: Controllo termico di un processo sensibile ==="
	./$(TARGET_S2) > $(LOG_DIR)/scenario2_output.log 2>&1 || true
	@echo "=== ESECUZIONE SCENARIO 3: Controllo della concentrazione con due fonti ==="
	./$(TARGET_S3) > $(LOG_DIR)/scenario3_output.log 2>&1 || true
	@echo "=== ESECUZIONE SCENARIO 4: Controllo divisione e svuotamento ==="
	./$(TARGET_S4) > $(LOG_DIR)/scenario4_output.log 2>&1 || true
	@echo "Tutti i test completati. Risultati salvati nella cartella $(LOG_DIR)/"
	@ls -la $(LOG_DIR)/


# Aiuto
help:
	@echo "Comandi disponibili:"
	@echo "  make                  - Compila scenario di default"
	@echo "  make scenario1        - Compila scenario 1 (riempimento veloce)"
	@echo "  make scenario2        - Compila scenario 2 (stress test)"
	@echo "  make scenario3        - Compila scenario 3 (test precisione)"
	@echo "  make scenario4        - Compila scenario 4 (divisione e svuotamento)"
	@echo "  make all_scenarios    - Compila tutti gli scenari"
	@echo "  make run_scenario1    - Compila ed esegue scenario 1"
	@echo "  make run_scenario2    - Compila ed esegue scenario 2"
	@echo "  make run_scenario3    - Compila ed esegue scenario 3"
	@echo "  make run_scenario4    - Compila ed esegue scenario 4"
	@echo "  make test_all         - Esegue tutti gli scenari e salva i risultati"
	@echo "  make clean            - Pulisce i file compilati"
	@echo "  make clean_all        - Pulisce tutto inclusi i log"
	@echo "  make help             - Mostra questo aiuto"

.PHONY: all scenario1 scenario2 scenario3 scenario4 all_scenarios clean clean_all clean_objects run_scenario1 run_scenario2 run_scenario3 run_scenario4 test_all help
