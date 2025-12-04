# Guida alla Compilazione

Questo documento descrive la procedura necessaria per compilare il progetto **RaceMetrics** dai file sorgente. Il progetto è sviluppato in **C++** utilizzando il framework **Qt**. Compatibile con **Windows** e **macOS**.

------------------------------------------------------------------------

## 🛠 Specifiche dell'Ambiente Originale

Il progetto è stato originariamente sviluppato e testato con la seguente configurazione:

-   **Framework:** Qt 6.2.4
-   **Compilatore:** GCC 11.3.0
-   **Build System:** QMake 3.1
-   **Make:** GNU Make 3.81

>⚠️ Nota per utenti macOS - Apple Silicon: Se si riscontrano errori di compilazione/linking (es. `framework 'AGL' not found`) si consiglia di utilizzare Qt 6.9.2 (o superiore), che supporta nativamente gli SDK macOS più recenti.
------------------------------------------------------------------------

## 📌 Prerequisiti

Prima di procedere, assicurarsi che l'ambiente di sviluppo disponga dei seguenti strumenti installati e configurati correttamente nel `PATH`:

- **Qt Development Framework**: Framework su cui si basa tutta l'interfaccia grafica. **Importante**: durante l'installazione assicurarsi di installare anche il modulo aggiuntivo **Qt Charts**, richiesto dal progetto
- **Compilatore C++**
- **QMake**: Serve a generare il Makefile. Viene installato insieme al framework Qt
- **Make**: Serve ad automatizzare la compilazione. Su macOS può essere già presente, su Windows va installato

------------------------------------------------------------------------

## 📁 Struttura del Progetto

La root del progetto deve presentarsi come segue:

    src/
    ├── assets/          # Risorse statiche (immagini, dati)
    ├── gui/             # Sorgenti interfaccia grafica (.cpp, .h)
    ├── logic_model/     # Logica applicativa (.cpp, .h)
    ├── main.cpp         # Entry point dell'app
    └── resources.qrc    # File risorse Qt

------------------------------------------------------------------------

## ⚙️ Procedura di Build

### 1) Inizializzazione del progetto

Aprire il terminale nella root del progetto ed eseguire:

    qmake -project

Questo comando genererà un file `.pro`.

### 2) Configurazione dei Moduli

Aprire il file `.pro` generato e aggiungere manualmente le dipendenze in
un'unica riga:

    QT += widgets charts

Il file dovrebbe apparire come segue:

    TEMPLATE = app
    TARGET = src
    INCLUDEPATH += .
    # --- AGGIUNGERE LA SEGUENTE RIGA ---
    QT += widgets charts
    # -----------------------------------
    HEADERS += ...
    SOURCES += ...

Salvare e chiudere il file.

### 3) Generazione Makefile

    qmake

### 4) Compilazione:

    make

### 5) Esecuzione

- **Windows**: 
```
  ./release/src.exe
```
- **MacOS**: Qt genera un bundle `.app`. Per avviare l'applicazione da terminale:
```
  open src.app
```
------------------------------------------------------------------------

## 🔄 Manutenzione e Ricompilazione

- **Ricompilazione rapida**: Se modifichi solo il contenuto dei file esistenti:
```
    make
```
- **Clean Build**: Se aggiungi/elimini file o cambi versione di Qt:
```
    qmake && make
```
