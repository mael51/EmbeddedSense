# Changelog

Toutes les modifications notables de ce projet sont documentées ici.
Format basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.1.0/).

## [0.9.0-rc] - 2026-11-11

### Ajouté

- Simulation de capteurs (température, humidité, pression) avec bruit
- Acquisition de données via buffer circulaire
- Machine à états (Dormant, Surveillance, Alerte, Erreur)
- Logger avec niveaux et horodatage
- Communication série simulée (MockUART)
- Multithreading PC (acquisition, traitement, communication)
- Couche d'abstraction FreeRTOS (ITask, IMutex, ITimer)
- Implémentations Host (PC) et STM32
- Drivers STM32 (température, humidité, UART, LED)
- FreeRTOSConfig.h pour STM32F4
- Cross-compilation ARM (toolchain, linker script, startup)
- 72 tests unitaires et d'intégration
- Documentation complète par phase (10 phases)