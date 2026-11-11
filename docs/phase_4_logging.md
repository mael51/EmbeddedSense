# Phase 4 - Logs et communication

**Objectif** : Systématiser les messages de log (debug, info, erreur) et simuler la communication série (UART) entre le système et un terminal distant.

---

## 🧠 Notions clés

- **Logger** : Un service centralisé qui écrit des messages avec un niveau (DEBUG, INFO, WARN, ERROR) et un horodatage.
- **UART** : Protocole de communication série utilisé dans 90% des systèmes embarqués. On le simule côté PC.
- **Buffer de transmission** : Les messages sont mis en file d'attente et envoyés progressivement.

---

## 🛠️ Étapes à réaliser

### 1. Créer l'enum `LogLevel`

Créer `services/logger/LogLevel.hpp` :

```cpp
#pragma once
#include <cstdint>

enum class LogLevel : uint8_t { DEBUG, INFO, WARN, ERROR };
```

### 2. Créer `Logger`

Créer `services/logger/Logger.hpp` et `.cpp` :

```cpp
#pragma once
#include "LogLevel.hpp"
#include <vector>
#include <string>

class Logger {
private:
    std::vector<std::string> logBuffer;
    LogLevel minLevel;
public:
    Logger(LogLevel minLevel = LogLevel::INFO);
    void log(LogLevel level, const std::string& message);
    std::string format(LogLevel level, const std::string& msg);
    const std::vector<std::string>& getLogs() const;
    size_t getLogCount() const;
    void clear();
};
```

Format de sortie : `[2026-08-26 14:30:15] [INFO] Temperature OK: 24.5°C`

### 3. Créer `MockUART`

Créer `app/communication/MockUART.hpp` et `.cpp` :

```cpp
#pragma once
#include <queue>
#include <string>

class MockUART {
private:
    std::queue<std::string> txBuffer;
    std::queue<std::string> rxBuffer;
    int baudRate;
public:
    MockUART(int baudRate = 115200);
    void send(const std::string& data);
    std::string receive();
    size_t txAvailable() const;
    size_t rxAvailable() const;
    void feedRx(const std::string& data);
};
```

### 4. Créer `CommunicationManager`

Créer `app/communication/CommunicationManager.hpp` et `.cpp` :

```cpp
#pragma once
#include "MockUART.hpp"
#include "services/logger/Logger.hpp"

class CommunicationManager {
private:
    MockUART& uart;
    Logger& logger;
public:
    CommunicationManager(MockUART& uart, Logger& logger);
    void sendLog(LogLevel level, const std::string& message);
    void processIncoming();
};
```

### 5. Écrire les tests

| Test | Vérification |
|------|-------------|
| `Logger.log` | Le message est stocké avec le bon niveau |
| `Logger.minLevel` | Les messages < minLevel sont ignorés |
| `Logger.format` | Le format `[DATE] [LEVEL] message` est correct |
| `MockUART.send/receive` | Données envoyées = données reçues |
| `MockUART.buffers` | txAvailable/rxAvailable retournent la bonne taille |
| `CommMgr.sendLog` | Envoie via UART + log le message |
| `CommMgr.processIncoming` | Commande "RESET" déclenche l'action |

### 6. Mettre à jour CMakeLists.txt

---

## ✅ Checklist de validation

- [ ] Logger filtre par niveau
- [ ] Le format de log est lisible et horodaté
- [ ] MockUART simule une communication série
- [ ] CommunicationManager lie Logger + UART
- [ ] 7+ tests unitaires passent
- [ ] `cmake --build build && cd build && ctest` : tous les tests passent

---

## 📁 Fichiers à créer/modifier

| Fichier | Action |
|---------|--------|
| `services/logger/LogLevel.hpp` | Créer |
| `services/logger/Logger.hpp` | Créer |
| `services/logger/Logger.cpp` | Créer |
| `app/communication/MockUART.hpp` | Créer |
| `app/communication/MockUART.cpp` | Créer |
| `app/communication/CommunicationManager.hpp` | Créer |
| `app/communication/CommunicationManager.cpp` | Créer |
| `tests/test_logging.cpp` | Créer |
| `CMakeLists.txt` | Modifier |
