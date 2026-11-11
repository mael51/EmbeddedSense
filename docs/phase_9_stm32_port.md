# Phase 9 - Portage STM32 réel

**Objectif** : Remplacer les implémentations PC par les vraies implémentations FreeRTOS et HAL STM32, et faire tourner le système sur une carte STM32 réelle.

---

## 🧠 Notions clés

- **STM32 HAL** : La bibliothèque officielle ST pour interagir avec le hardware (GPIO, ADC, UART, Timer).
- **FreeRTOS sur STM32** : Le vrai RTOS, avec ses tasks, semaphores, queues.
- **ADC (Analog-to-Digital Converter)** : Convertit le signal analogique du capteur en nombre numérique.
- **Flash & Debug** : Écrire le programme dans la mémoire Flash du STM32 et déboguer via ST-Link.

---

## 🛠️ Étapes à réaliser

### 1. Générer le projet STM32CubeMX

1. Ouvrir STM32CubeMX
2. Sélectionner ta carte (ex: STM32F407 Discovery)
3. Configurer :
   - **GPIO** : LED (sortie), bouton (entrée)
   - **UART** : PA2 (TX), PA3 (RX) à 115200 baud
   - **ADC** : PA0 pour température, PA1 pour humidité
   - **Timer** : TIM2 pour le tick FreeRTOS
4. Activer FreeRTOS (CMSIS_V2)
5. Générer le code

### 2. Remplacer les implémentations Host

| Fichier Host | Fichier STM32 | Changement |
|-------------|---------------|------------|
| `HostTask.hpp/.cpp` | `STM32Task.hpp/.cpp` | std::thread → xTaskCreate |
| `HostMutex.hpp/.cpp` | `STM32Mutex.hpp/.cpp` | std::mutex → xSemaphoreCreateMutex |
| `HostTimer.hpp/.cpp` | `STM32Timer.hpp/.cpp` | std::chrono → xTimer |

### 3. Implémenter les drivers réels

| Fichier | Description |
|---------|-------------|
| `drivers/stm32/STM32TempSensor.hpp/.cpp` | Lit l'ADC, convertit en °C |
| `drivers/stm32/STM32HumiditySensor.hpp/.cpp` | Lit l'ADC, convertit en % |
| `drivers/stm32/STM32UART.hpp/.cpp` | Communication série réelle |
| `drivers/stm32/STM32LED.hpp/.cpp` | Contrôle LED verte/rouge/bleue |

### 4. Configurer FreeRTOS

Modifier `FreeRTOSConfig.h` :

```c
#define configUSE_PREEMPTION        1
#define configCPU_CLOCK_HZ         168000000
#define configTICK_RATE_HZ         1000
#define configMAX_PRIORITIES        5
#define configMINIMAL_STACK_SIZE    128
#define configTOTAL_HEAP_SIZE       32768
```

### 5. Flash et test sur la carte

```bash
# Compiler
cmake -DCROSS_COMPILE=ON -S . -B build_stm32
cmake --build build_stm32

# Flasher via st-flash
st-flash write build_stm32/embeddedsense_firmware.bin 0x08000000

# Ou via OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "program build_stm32/embeddedsense_firmware.elf verify reset exit"
```

### 6. Tester sur la carte

| Test | Résultat attendu |
|------|-----------------|
| Allumer LED verte | LED verte s'allume |
| Appuyer sur bouton | État change en SURVEILLANCE |
| Température > seuil | LED rouge + ALERTE |
| Reset | Retour au DORMANT |
| Envoyer "STATUS" via UART | Réponse avec les valeurs |

---

## ✅ Checklist de validation

- [ ] Le code compile pour STM32 sans erreur
- [ ] Le code est flashé sur la carte
- [ ] Les LEDs réagissent selon la machine à états
- [ ] Les capteurs ADC retournent des valeurs réalistes
- [ ] La communication UART fonctionne (terminal série)
- [ ] FreeRTOS tourne sans crash après 10 minutes

---

## 📁 Fichiers à créer/modifier

| Fichier | Action |
|---------|--------|
| `drivers/stm32/STM32TempSensor.hpp/.cpp` | Créer |
| `drivers/stm32/STM32HumiditySensor.hpp/.cpp` | Créer |
| `drivers/stm32/STM32UART.hpp/.cpp` | Créer |
| `drivers/stm32/STM32LED.hpp/.cpp` | Créer |
| `platform/stm32/STM32Task.hpp/.cpp` | Créer |
| `platform/stm32/STM32Mutex.hpp/.cpp` | Créer |
| `platform/stm32/STM32Timer.hpp/.cpp` | Créer |
| `platform/stm32/FreeRTOSConfig.h` | Créer |
| `CMakeLists.txt` | Modifier |
