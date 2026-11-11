# Phase 0 - Mise en place de l'environnement

**Statut** : ✅ Validée
**Date** : 10 Novembre 2026

---

## 📋 Table des matières

1. [Objectif](#objectif)
2. [Outils requis](#outils-requis)
3. [Installation](#installation)
4. [Structure du projet](#structure-du-projet)
5. [Compilation](#compilation)
6. [Tests](#tests)
7. [Ce qu'on a appris](#ce-quon-a-appris)

---

## Objectif

Mettre en place l'environnement de développement pour le projet EmbeddedSense : compilateur, CMake, GoogleTest, et structure du projet.

---

## Outils requis

### Sur PC (Windows)

- **CMake** >= 3.14
- **GCC** (MSYS2/MinGW) >= 13.0
- **GoogleTest** (téléchargé automatiquement par CMake via FetchContent)
- **Git** pour le versioning

### Pour STM32 (Phase 8+)

- **arm-none-eabi-gcc** (toolchain ARM)
- **STM32CubeMX** (génération de code)
- **ST-Link** ou **OpenOCD** (flashage)

---

## Installation

### 1. Installer MSYS2

Télécharger MSYS2 depuis https://www.msys2.org/ et installer.

### 2. Installer GCC et CMake

```bash
# Dans MSYS2 (terminal MINGW64)
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-ninja
```

### 3. Vérifier l'installation

```bash
gcc --version
# gcc (GCC) 13.3.0

cmake --version
# cmake version 3.28.0
```

### 4. Configurer VSCode (optionnel)

Ajouter dans `.vscode/settings.json` :

```json
{
    "terminal.integrated.env.windows": {
        "PATH": "C:\\msys64\\mingw64\\bin;C:\\msys64\\usr\\bin;${env:PATH}"
    }
}
```

---

## Structure du projet

```
EmbeddedSense/
├── app/                 # Code applicatif
│   ├── acquisition/     # Capteurs, buffer, gestionnaire
│   ├── communication/   # UART, communication manager
│   ├── control/         # Machine à états
│   └── processing/      # Données partagées, tâches
├── drivers/             # Drivers (mock + STM32)
│   ├── interfaces/      # ISensor
│   ├── mock/            # Capteurs simulés
│   └── stm32/           # Drivers STM32 (Phase 9)
├── platform/            # Abstraction plateforme
│   ├── abstraction/     # ITask, IMutex, ITimer
│   ├── host/            # Implémentations PC
│   └── stm32/           # Implémentations STM32 (Phase 9)
├── services/            # Services (logger)
├── tests/               # Tests unitaires (GoogleTest)
├── docs/                # Documentation par phase
├── examples/            # Démo
├── CMakeLists.txt       # Build system
├── README.md            # Documentation principale
└── .clang-format        # Style de code
```

---

## Compilation

### Sur PC

```bash
cmake -G "MSYS Makefiles" -S . -B build
cmake --build build
```

### Pour STM32 (Phase 8+)

```bash
cmake -G "MSYS Makefiles" \
  -DCMAKE_TOOLCHAIN_FILE="platform/stm32/toolchain-arm.cmake" \
  -DCROSS_COMPILE=ON -S . -B build_stm32
cmake --build build_stm32
```

---

## Tests

```bash
cd build && ./embeddedsense_tests.exe
```

Résultat attendu :
```
[==========] Running 72 tests from 17 test suites.
[  PASSED  ] 72 tests.
```

---

## Ce qu'on a appris

1. **CMake** : configuration, génération de build files
2. **MSYS2/MinGW** : environnement GCC sur Windows
3. **GoogleTest** : framework de tests unitaires
4. **Structure de projet** : organisation en dossiers (app, drivers, platform, services, tests)
5. **Convention de nommage** : fichiers .hpp/.cpp, tests test_*.cpp

---

*Règle d'or : Ne passe pas à la phase suivante sans valider la checklist de la phase en cours.*