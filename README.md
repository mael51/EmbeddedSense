# Projet EmbeddedSense - Embedded Monitoring & Control System

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Tests](https://img.shields.io/badge/tests-72%20passed-brightgreen)
![Platform](https://img.shields.io/badge/platform-PC%20%7C%20STM32-orange)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

Système embarqué de monitoring et contrôle avec simulation PC et portage STM32.

## 📋 Description

EmbeddedSense est un projet pédagogique en 10 phases pour apprendre le développement embarqué **sans matériel au début** (simulation sur PC), puis le porter sur une carte STM32 réelle.

### Fonctionnalités

- **Simulation de capteurs** : Température, humidité, pression avec bruit réaliste
- **Acquisition de données** : Buffer circulaire optimisé mémoire
- **Machine à états** : Dormant → Surveillance → Alerte → Erreur
- **Logging** : Messages avec niveaux et horodatage
- **Communication série** : Simulation UART
- **Multithreading** : Acquisition, traitement et communication en parallèle
- **FreeRTOS** : Couche d'abstraction pour portage STM32
- **Tests** : 72 tests unitaires et d'intégration

---

## 🏗️ Architecture

```
EmbeddedSense/
├── app/
│   ├── acquisition/     # CircularBuffer, SensorManager, AcquisitionTask
│   ├── communication/   # MockUART, CommunicationManager, CommunicationTask
│   ├── control/         # StateMachine, 4 états, Types
│   └── processing/      # SharedData, ProcessingTask
├── drivers/
│   ├── interfaces/      # ISensor
│   ├── mock/            # MockTemperature/Humidity/PressureSensor
│   └── stm32/           # STM32TempSensor, STM32HumiditySensor, STM32UART, STM32LED
├── platform/
│   ├── abstraction/     # ITask, IMutex, ITimer
│   ├── host/            # HostTask, HostMutex, HostTimer (PC)
│   └── stm32/           # STM32Task, STM32Mutex, STM32Timer, FreeRTOSCompat, FreeRTOSConfig
├── services/
│   └── logger/          # Logger, LogLevel
├── tests/               # 72 tests (GoogleTest)
├── docs/                # Documentation par phase
└── CMakeLists.txt       # Build system
```

---

## 🚀 Compilation

### Sur PC (MSYS2/MinGW)

```bash
# Configurer
cmake -G "MSYS Makefiles" -S . -B build

# Compiler
cmake --build build

# Lancer les tests
cd build && ./embeddedsense_tests.exe
```

### Pour STM32 (cross-compilation)

```bash
# Configurer
cmake -G "MSYS Makefiles" \
  -DCMAKE_TOOLCHAIN_FILE="platform/stm32/toolchain-arm.cmake" \
  -DCROSS_COMPILE=ON -S . -B build_stm32

# Compiler
cmake --build build_stm32

# Résultat
# build_stm32/embeddedsense_firmware.elf
# build_stm32/embeddedsense_firmware.bin
```

---

## 🧪 Tests

```bash
cd build && ./embeddedsense_tests.exe
```

```
[==========] Running 72 tests from 17 test suites.
[  PASSED  ] 72 tests.
```

| Suite | Tests | Description |
|-------|-------|-------------|
| Phase0Test | 1 | Sanity check |
| SensorTest | 7 | Capteurs simulés |
| CircularBufferTest | 6 | Buffer circulaire |
| SensorManagerTest | 3 | Gestionnaire de capteurs |
| StateMachineTest | 11 | Machine à états |
| LoggerTest | 6 | Logger |
| MockUARTTest | 4 | UART simulé |
| CommManagerTest | 7 | CommunicationManager |
| SharedDataTest | 4 | Données partagées |
| AcquisitionTaskTest | 2 | Tâche d'acquisition |
| ProcessingTaskTest | 1 | Tâche de traitement |
| HostTaskTest | 2 | HostTask (PC) |
| HostMutexTest | 2 | HostMutex (PC) |
| HostTimerTest | 3 | HostTimer (PC) |
| FreeRTOSCompatTest | 3 | Compatibilité FreeRTOS |
| IntegrationTest | 6 | Tests d'intégration |
| RobustnessTest | 6 | Tests de robustesse |

---

## 📚 Documentation

Toute la documentation détaillée se trouve dans le dossier `docs/`.

| Phase | Fichier | Description |
|-------|---------|-------------|
| 0 | `phase_0_setup.md` | Environnement |
| 1 | `phase_1_sensors.md` | Simulation des capteurs |
| 2 | `phase_2_acquisition.md` | Acquisition et traitement |
| 3 | `phase_3_state_machine.md` | Machine à états |
| 4 | `phase_4_logging.md` | Logs et communication |
| 5 | `phase_5_multithreading.md` | Multithreading (PC) |
| 6 | `phase_6_freertos.md` | FreeRTOS simulé |
| 7 | `phase_7_integration.md` | Tests d'intégration |
| 8 | `phase_8_stm32_prep.md` | Préparation STM32 |
| 9 | `phase_9_stm32_port.md` | Portage STM32 réel |
| 10 | `phase_10_final.md` | Finalisation |

---

## 🔧 Outils requis

### Sur PC
- **CMake** >= 3.14
- **GCC** (MSYS2/MinGW) >= 13.0
- **GoogleTest** (téléchargé automatiquement par CMake)

### Pour STM32
- **arm-none-eabi-gcc** (toolchain ARM)
- **STM32CubeMX** (génération de code)
- **ST-Link** ou **OpenOCD** (flashage)

---

## 📝 Style de code

Le projet utilise `clang-format` avec le style Google (indentation 4 espaces).

```bash
clang-format -i app/**/*.cpp drivers/**/*.cpp platform/**/*.cpp services/**/*.cpp
```

---

## 🏷️ Version

**v0.9.0-rc** — Simulation PC validée (72 tests, démo fonctionnelle). Cross-compilation ARM vérifiée. Portage STM32 matériel en cours.

## Auteur

Maël AGAMBIMAWO

---

*Règle d'or : Ne passe pas à la phase suivante sans valider la checklist de la phase en cours.*
