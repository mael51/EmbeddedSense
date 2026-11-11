# Phase 1 - Simulation des capteurs

**Objectif** : Créer des classes C++ qui simulent des capteurs (température, humidité, pression) sans matériel réel.

---

## 🧠 Notions clés

- **Interface (classe abstraite)** : Un contrat que chaque capteur doit respecter. Oblige tous les capteurs à avoir les mêmes méthodes (`read()`, `getName()`).
- **Mock** : Un faux capteur qui génère des valeurs prédictibles pour les tests.
- **Générateur aléatoire** : Simule le bruit réel d'un capteur physique avec `std::mt19937`.

---

## 🛠️ Étapes à réaliser

### 1. Définir l'interface `ISensor`

Créer `drivers/interfaces/ISensor.hpp` :

```cpp
#pragma once
#include <string>

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual double read() = 0;
    virtual std::string getName() = 0;
};
```

### 2. Implémenter `MockTemperatureSensor`

Créer `drivers/mock/MockTemperatureSensor.hpp` et `.cpp` :

```cpp
#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include <random>

class MockTemperatureSensor : public ISensor {
private:
    double baseTemp;
    std::mt19937 gen;
    std::normal_distribution<double> noise;
public:
    MockTemperatureSensor(double baseTemp = 25.0);
    double read() override;
    std::string getName() override;
};
```

### 3. Implémenter `MockHumiditySensor`

Même principe, plage 0-100%, `getName()` retourne `"Humidity"`.

### 4. Implémenter `MockPressureSensor`

Même principe, plage 900-1100 hPa, `getName()` retourne `"Pressure"`.

### 5. Écrire les tests unitaires

Créer `tests/test_sensors.cpp` :

| Test | Vérification |
|------|-------------|
| `MockTemp.read()` | Valeur dans la plage [baseTemp - 5, baseTemp + 5] |
| `MockTemp.getName()` | Retourne `"Temperature"` |
| `MockHumidity.read()` | Valeur dans [0, 100] |
| `MockPressure.read()` | Valeur dans [900, 1100] |

### 6. Mettre à jour CMakeLists.txt

Ajouter les sources et les tests au build.

---

## ✅ Checklist de validation

- [ ] `ISensor.hpp` compile sans erreur
- [ ] 3 classes Mock implémentées (Température, Humidité, Pression)
- [ ] `MockTemperatureSensor::read()` retourne une valeur cohérente sur 1000 appels
- [ ] 4+ tests unitaires passent
- [ ] `cmake --build build && cd build && ctest` : tous les tests passent

---

## 📁 Fichiers à créer/modifier

| Fichier | Action |
|---------|--------|
| `drivers/interfaces/ISensor.hpp` | Créer |
| `drivers/mock/MockTemperatureSensor.hpp` | Créer |
| `drivers/mock/MockTemperatureSensor.cpp` | Créer |
| `drivers/mock/MockHumiditySensor.hpp` | Créer |
| `drivers/mock/MockHumiditySensor.cpp` | Créer |
| `drivers/mock/MockPressureSensor.hpp` | Créer |
| `drivers/mock/MockPressureSensor.cpp` | Créer |
| `tests/test_sensors.cpp` | Créer |
| `CMakeLists.txt` | Modifier |
