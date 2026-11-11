# Phase 2 - Acquisition et traitement

**Objectif** : Acquérir les données de plusieurs capteurs, les stocker dans une structure et appliquer un traitement de base (moyenne, min, max).

---

## 🧠 Notions clés

- **Acquisition** : Lire les capteurs à intervalle régulier.
- **Buffer circulaire** : Un tableau qui écrase les anciennes valeurs quand il est plein (optimisé mémoire, parfait pour l'embarqué).
- **Moyenne glissante** : Lissage du signal pour réduire le bruit.

---

## 🛠️ Étapes à réaliser

### 1. Créer `CircularBuffer<T>`

Créer `app/acquisition/CircularBuffer.hpp` :

```cpp
#pragma once
#include <cstddef>

template<typename T, size_t Capacity>
class CircularBuffer {
private:
    T buffer[Capacity];
    size_t head = 0;
    size_t count = 0;
public:
    void push(T value);
    T get(size_t index) const;
    size_t size() const;
    T average() const;
    T min() const;
    T max() const;
};
```

### 2. Créer `SensorManager`

Créer `app/acquisition/SensorManager.hpp` et `.cpp` :

```cpp
#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include "CircularBuffer.hpp"
#include <vector>
#include <memory>

class SensorManager {
private:
    std::vector<std::unique_ptr<ISensor>> sensors;
    CircularBuffer<double, 64> tempBuffer;
    CircularBuffer<double, 64> humidityBuffer;
    CircularBuffer<double, 64> pressureBuffer;
public:
    void addSensor(std::unique_ptr<ISensor> sensor);
    void acquire();
    double getTemperatureAvg() const;
    double getHumidityAvg() const;
    double getPressureAvg() const;
};
```

### 3. Implémenter `SensorManager::acquire()`

```cpp
void SensorManager::acquire() {
    for (auto& sensor : sensors) {
        double value = sensor->read();
        std::string name = sensor->getName();
        if (name == "Temperature") tempBuffer.push(value);
        else if (name == "Humidity") humidityBuffer.push(value);
        else if (name == "Pressure") pressureBuffer.push(value);
    }
}
```

### 4. Écrire les tests

| Test | Vérification |
|------|-------------|
| `CircularBuffer.push/get` | Valeurs lues dans l'ordre FIFO |
| `CircularBuffer.average` | Moyenne correcte sur 5 valeurs |
| `CircularBuffer.overflow` | Après Capacity+1 pushes, les plus anciennes sont écrasées |
| `SensorManager.acquire` | Les buffers contiennent les données |
| `SensorManager.getAvg` | Moyenne cohérente après 10 acquisitions |

### 5. Mettre à jour CMakeLists.txt

---

## ✅ Checklist de validation

- [ ] `CircularBuffer` fonctionne pour un type `int` et `double`
- [ ] Overflow correct : les anciennes valeurs sont bien écrasées
- [ ] `SensorManager` gère 3 capteurs simultanément
- [ ] 5+ tests unitaires passent
- [ ] `cmake --build build && cd build && ctest` : tous les tests passent

---

## 📁 Fichiers à créer/modifier

| Fichier | Action |
|---------|--------|
| `app/acquisition/CircularBuffer.hpp` | Créer |
| `app/acquisition/SensorManager.hpp` | Créer |
| `app/acquisition/SensorManager.cpp` | Créer |
| `tests/test_acquisition.cpp` | Créer |
| `CMakeLists.txt` | Modifier |
