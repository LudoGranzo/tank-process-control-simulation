#!/bin/bash

# Script per automatizzare i test degli scenari della simulazione serbatoi

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== SIMULAZIONE SERBATOI - TEST SCENARI MULTIPLI ===${NC}"

# Funzione per compilare e testare uno scenario
test_scenario() {
    local scenario_num=$1
    local scenario_name=$2
    
    echo -e "${YELLOW}--- Compilazione Scenario $scenario_num: $scenario_name ---${NC}"
    
    if make scenario$scenario_num > /dev/null 2>&1; then
        echo -e "${GREEN}✓ Compilazione riuscita${NC}"
        
        echo -e "${YELLOW}--- Esecuzione Scenario $scenario_num ---${NC}"
        ./simulation_scenario$scenario_num > results_scenario$scenario_num.txt
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓ Esecuzione completata${NC}"
            echo -e "   Risultati salvati in: ${BLUE}results_scenario$scenario_num.txt${NC}"
            
            # Mostra un breve sommario
            echo -e "${YELLOW}   Sommario risultati:${NC}"
            tail -10 results_scenario$scenario_num.txt | head -5
        else
            echo -e "${RED}✗ Errore durante l'esecuzione${NC}"
        fi
    else
        echo -e "${RED}✗ Errore di compilazione${NC}"
    fi
    
    echo ""
}

# Menu interattivo
show_menu() {
    echo ""
    echo -e "${BLUE}Scegli un'opzione:${NC}"
    echo "1) Testa Scenario 1 - Riempimento Veloce"
    echo "2) Testa Scenario 2 - Stress Test Alta Evaporazione"  
    echo "3) Testa Scenario 3 - Test Precisione"
    echo "4) Testa tutti gli scenari"
    echo "5) Confronta risultati degli scenari"
    echo "6) Pulisci file compilati"
    echo "7) Mostra aiuto Makefile"
    echo "0) Esci"
    echo ""
}

# Funzione per confrontare i risultati
compare_results() {
    echo -e "${YELLOW}=== CONFRONTO RISULTATI SCENARI ===${NC}"
    
    for i in 1 2 3; do
        if [ -f "results_scenario$i.txt" ]; then
            echo -e "${BLUE}--- Scenario $i ---${NC}"
            # Mostra le ultime righe di ogni scenario
            tail -15 results_scenario$i.txt | head -10
            echo ""
        else
            echo -e "${RED}Risultati Scenario $i non trovati${NC}"
        fi
    done
}

# Controllo se siamo nella directory corretta
if [ ! -f "simulation.c" ]; then
    echo -e "${RED}Errore: Esegui lo script dalla directory del progetto${NC}"
    exit 1
fi

# Loop principale
while true; do
    show_menu
    read -p "Inserisci la tua scelta: " choice
    
    case $choice in
        1)
            test_scenario 1 "Riempimento Veloce"
            ;;
        2)
            test_scenario 2 "Stress Test Alta Evaporazione"
            ;;
        3)
            test_scenario 3 "Test Precisione"
            ;;
        4)
            echo -e "${YELLOW}=== ESECUZIONE TUTTI GLI SCENARI ===${NC}"
            test_scenario 1 "Riempimento Veloce"
            test_scenario 2 "Stress Test Alta Evaporazione"
            test_scenario 3 "Test Precisione"
            echo -e "${GREEN}✓ Tutti gli scenari completati!${NC}"
            ;;
        5)
            compare_results
            ;;
        6)
            echo -e "${YELLOW}Pulizia file compilati...${NC}"
            make clean
            rm -f results_scenario*.txt
            echo -e "${GREEN}✓ Pulizia completata${NC}"
            ;;
        7)
            make help
            ;;
        0)
            echo -e "${GREEN}Arrivederci!${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}Opzione non valida. Riprova.${NC}"
            ;;
    esac
    
    echo ""
    read -p "Premi ENTER per continuare..."
done
