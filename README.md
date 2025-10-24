# assegnamento_finale

Descrizione
-----------

Questo progetto implementa una simulazione di serbatoi con più scenari di test. Il codice sorgente è scritto in C e permette di compilare diversi eseguibili, ognuno corrispondente a uno scenario specifico:
- Scenario 1: Gestione di una fase batch con riempimento e scarico.
 Il sistema deve riempire i serbatoi fino a un certo livello, miscelare, poi scaricare.
 Le variazioni di concentrazione e temperatura sono visualizzate ma non è attivo il controllo (limitazione nel range).
- Scenario 2: Controllo termico di un processo sensibile.
 I serbatoi devono mantenere la temperatura entro un certo range.  I fluidi in ingresso sono più freddi, quindi i riscaldatori devono attivarsi per mantenere la temperatura. L'obiettivo è sia evitare raffreddamento eccessivo sia sovratemperatura.
- Scenario 3: Controllo della concentrazione con due fonti.
 I serbatoi vengono alimentati costantemente da due fonti con diverse concentrazioni. L'obiettivo è raggiungere il volume scopo, mantenere il volume (a meno di perdite dovute al coefficiente di evaporazione) e nel frattempo mantenere la concentrazione entro il range specificato. Il sistema deve decidere quale pompa attivare in modo da mantenere la concentrazione entro il range desiderato.
- Scenario 4: Riempimento di un serbatoio e gestione della divisione del fluido tra i serbatoi con  scarico. 
 Il sistema deve dividere equamente il fluido all'interno dei serbatoi, successivamente scaricare. Le variazioni di concentrazione e temperatura sono visualizzate ma non è attivo il controllo (limitazione nel range).

Struttura dei file
------------------

- `main.c` - Punto di ingresso del programma.
- `simulation.c` - Logica della simulazione.
- `scenarios/` - Directory contenente eventuali header o file di configurazione per gli scenari (inclusa tramite `-Iscenarios` nel Makefile).
- `Makefile` - Script di compilazione che fornisce target per compilare e lanciare ciascuno scenario.

Requisiti
---------

- GCC (o altro compilatore compatibile C) installato.
- Ambiente Linux/Unix con `make` disponibile.
- (Opzionale) Strumenti di debug come `gdb` se `CFLAGS` include `-g`.

Istruzioni di compilazione
-------------------------

Il progetto include un `Makefile` con i seguenti target principali:

- `make` o `make all` - Compila l'eseguibile di default `simulation`.
- `make scenario1` - Compila l'eseguibile `simulation_scenario1` con la macro `SCENARIO_1` attivata (Test Riempimento Veloce).
- `make scenario2` - Compila l'eseguibile `simulation_scenario2` con la macro `SCENARIO_2` attivata (Test Stress Alta Evaporazione).
- `make scenario3` - Compila l'eseguibile `simulation_scenario3` con la macro `SCENARIO_3` attivata (Test Precisione).
- `make scenario4` - Compila l'eseguibile `simulation_scenario4` con la macro `SCENARIO_4` attivata (Test Divisione e Svuotamento).
- `make all_scenarios` - Compila tutti gli eseguibili degli scenari.
- `make run_scenario1|run_scenario2|run_scenario3|run_scenario4` - Compila (se necessario) ed esegue lo scenario selezionato.
- `make test_all` - Compila tutti gli scenari, li esegue in sequenza e salva l'output rispettivamente in `results_scenario1.txt`, `results_scenario2.txt`, `results_scenario3.txt` e `results_scenario4.txt`.
- `make clean` - Rimuove gli oggetti compilati e gli eseguibili generati.

Esempi di comandi
-----------------

Compilare lo scenario di default:

```bash
make
```

Compilare ed eseguire lo scenario 1 (riempimento veloce):

```bash
make run_scenario1
```

Compilare tutti gli scenari e salvare i risultati:

```bash
make test_all
```

Note tecniche
-------------

- Il `Makefile` usa la variabile `CFLAGS` per definire flag di compilazione (`-Wall -Wextra -std=c99 -g`).
- Per ogni scenario, il Makefile aggiunge una macro di preprocessore (`-DSCENARIO_X`) prima della compilazione per abilitare il comportamento specifico nello stesso codice sorgente.
- Il target `clean_objects` rimuove solo i file oggetto `.o` per evitare conflitti quando si ricompilano eseguibili con macro diverse.

# assegnamento_finale