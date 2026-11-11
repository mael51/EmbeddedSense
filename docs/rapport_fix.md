# Rapport de fix — EmbeddedSense

**Date** : 11 novembre 2026
**Version** : 0.9.0-rc
**Auteur** : Maël AGAMBIMAWO

---

## 1. Contexte

Suite à l'audit technique (`Rapport_amelioration_EmbeddedSense.md`), plusieurs écarts ont été identifiés entre les déclarations du dépôt et le code réel. Ce rapport documente les corrections appliquées.

---

## 2. Corrections appliquées

### 2.1. Typage des capteurs (plus de comparaison de chaînes)

**Problème** : `SensorManager::acquire()` classait les capteurs en comparant `sensor->getName()` avec des chaînes littérales (`"Temperature"`, `"Humidity"`, `"Pressure"`). Fragile face à une faute de frappe.

**Correction** :
- Ajout de l'enum `SensorType { TEMPERATURE, HUMIDITY, PRESSURE, UNKNOWN }` dans `ISensor.hpp`
- Ajout de `virtual SensorType getType() const = 0` dans `ISensor`
- Implémentation dans tous les drivers (mock + STM32)
- `SensorManager::acquire()` utilise `switch(sensor->getType())`

**Fichiers modifiés** :
- `drivers/interfaces/ISensor.hpp`
- `drivers/mock/MockTemperatureSensor.hpp`
- `drivers/mock/MockHumiditySensor.hpp`
- `drivers/mock/MockPressureSensor.hpp`
- `drivers/stm32/STM32TempSensor.hpp`
- `drivers/stm32/STM32HumiditySensor.hpp`
- `app/acquisition/SensorManager.cpp`
- `tests/test_robustness.cpp` (NaNMockSensor)

### 2.2. StateMachine (plus de déduction par nom)

**Problème** : `getCurrentState()` reconstruisait l'état en comparant `currentState->getName()` avec des chaînes.

**Correction** :
- Ajout du membre `State currentStateEnum` dans `StateMachine.hpp`
- `getCurrentState()` retourne directement `currentStateEnum`
- `handleEvent()` met à jour `currentStateEnum` lors des transitions

**Fichiers modifiés** :
- `app/control/StateMachine.hpp`
- `app/control/StateMachine.cpp`

### 2.3. Version honnête

**Problème** : Le README annonçait `v1.0.0` avec "10 phases validées" alors que le portage STM32 est un stub.

**Correction** :
- README : `v0.9.0-rc` — "Simulation PC validée. Cross-compilation ARM vérifiée. Portage STM32 matériel en cours."
- CHANGELOG : `[0.9.0-rc]`
- Tag Git : `v0.9.0-rc` (ancien `v1.0.0` supprimé)

### 2.4. Toolchain portable

**Problème** : Le toolchain file modifiait globalement `PATH` via `set(ENV{PATH} ...)`.

**Correction** :
- Suppression de la modification de `PATH`
- Documentation des prérequis d'environnement dans le fichier

**Fichier modifié** : `platform/stm32/toolchain-arm.cmake`

### 2.5. Tests renforcés

**Problème** : `Scenario3_CommunicationFullFlow` utilisait `EXPECT_GE(txAvailable(), 0u)` qui ne vérifie rien d'utile.

**Correction** :
- Remplacé par `EXPECT_EQ(uart.txAvailable(), 1u)` pour vérifier qu'un message a bien été envoyé

**Fichier modifié** : `tests/test_integration.cpp`

### 2.6. CMake restructuré

**Problème** : Cibles dupliquées avec listes de fichiers répétées.

**Correction** :
- Création de `embeddedsense_core` (bibliothèque statique, code portable)
- Création de `embeddedsense_platform_host` (plateforme PC)
- Création de `embeddedsense_drivers_mock` (capteurs simulés)
- `embeddedsense_demo`, `embeddedsense_tests`, `embeddedsense_firmware` lient les bibliothèques
- Activation des warnings (`-Wall -Wextra -Wpedantic`)

**Fichier modifié** : `CMakeLists.txt`

### 2.7. Demo comme point d'entrée

**Problème** : `app/main.cpp` affichait juste "Phase 0 : OK!".

**Correction** :
- `app/main.cpp` lance maintenant un vrai flux : acquisition → traitement → affichage des mesures

### 2.8. Nommage cohérent

**Problème** : Les exécutables s'appelaient `emcs_app`, `emcs_tests`, `emcs_demo`.

**Correction** :
- Renommés en `embeddedsense_firmware`, `embeddedsense_tests`, `embeddedsense_demo`
- CI, docs, README mis à jour

### 2.9. Renommage du projet

**Problème** : "EMCS" est un acronyme générique avec 2000+ résultats sur GitHub.

**Correction** :
- Projet renommé en **EmbeddedSense**
- Tous les fichiers, le README, le CMakeLists, les docs mis à jour

---

## 3. Validation

| Vérification | Résultat |
|-------------|----------|
| Build PC | ✅ 0 erreur, 0 warning |
| 72 tests | ✅ Tous passés |
| Cross-compilation STM32 | ✅ `embeddedsense_firmware.elf` 100 bytes |
| Démo | ✅ Fonctionne |
| Git | ✅ Commit propre + tag `v0.9.0-rc` |
| GitHub | ✅ Poussé avec CI déclenché |

---

## 4. Points restants (hors scope de ce fix)

| Priorité | Action | Statut |
|----------|--------|--------|
| P1 | Tests de concurrence déterministes (horloges injectables) | À faire |
| P1 | Ajouter tests limites, stress et débordement buffer | À faire |
| P2 | Analyse statique (clang-tidy, cppcheck) | À faire |
| P2 | Couverture de code (lcov/gcov) | À faire |
| P3 | Portage STM32 réel (HAL, FreeRTOS, flash) | À faire |

---

## 5. Fichiers modifiés

| Fichier | Type de modification |
|---------|---------------------|
| `drivers/interfaces/ISensor.hpp` | Ajout SensorType enum + getType() |
| `drivers/mock/MockTemperatureSensor.hpp` | Implémentation getType() |
| `drivers/mock/MockHumiditySensor.hpp` | Implémentation getType() |
| `drivers/mock/MockPressureSensor.hpp` | Implémentation getType() |
| `drivers/stm32/STM32TempSensor.hpp` | Implémentation getType() |
| `drivers/stm32/STM32HumiditySensor.hpp` | Implémentation getType() |
| `app/acquisition/SensorManager.cpp` | Switch sur SensorType |
| `app/control/StateMachine.hpp` | currentStateEnum membre |
| `app/control/StateMachine.cpp` | Utilisation de l'enum |
| `platform/stm32/toolchain-arm.cmake` | Suppression modification PATH |
| `tests/test_robustness.cpp` | getType() + NaNMockSensor |
| `tests/test_integration.cpp` | Assertion renforcée |
| `app/main.cpp` | Demo réelle |
| `CMakeLists.txt` | Restructuration avec bibliothèques |
| `README.md` | Version honnête + nom EmbeddedSense |
| `CHANGELOG.md` | Version 0.9.0-rc |
| `.github/workflows/ci.yml` | Noms mis à jour |
| `.github/workflows/release.yml` | Noms mis à jour |
| `docs/*.md` | Noms mis à jour |

---

*Rapport généré le 11 novembre 2026.*