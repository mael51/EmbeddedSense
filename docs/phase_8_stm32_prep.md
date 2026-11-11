# Phase 8 - Préparation STM32

**Objectif** : Installer les outils de cross-compilation pour STM32, configurer un toolchain ARM, et valider qu'on peut compiler pour le microcontrôleur.

---

## 🧠 Notions clés

- **Cross-compilation** : Compiler sur PC (x86) pour une autre architecture (ARM Cortex-M). Le code ne tourne pas sur ton PC, il tourne sur la carte.
- **Toolchain ARM** : `arm-none-eabi-gcc` — le compilateur GCC adapté pour les microcontrôleurs ARM.
- **STM32CubeMX** : L'outil graphique STMicroelectronics qui génère le code d'initialisation (GPIO, Horloges, UART...).
- **Linker script (.ld)** : Définit comment la mémoire est organisée dans la puce (RAM, Flash).

---

## 🛠️ Étapes à réaliser

### 1. Installer le toolchain ARM

**Option A — MSYS2 (recommandé) :**

```bash
pacman -S mingw-w64-x86_64-arm-none-eabi-gcc
pacman -S mingw-w64-x86_64-arm-none-eabi-newlib
```

**Option B — STM32CubeIDE :**

Télécharger depuis st.com — inclut le toolchain complet.

### 2. Vérifier l'installation

```bash
arm-none-eabi-gcc --version
```

### 3. Installer STM32CubeMX

Télécharger depuis st.com (gratuit, nécessite un compte ST).

### 4. Créer un linker script minimal

Créer `platform/stm32/stm32f4xx.ld` :

```ld
MEMORY
{
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
}
```

### 5. Créer le fichier d'initialisation

Créer `platform/stm32/startup_stm32f4xx.s` : minimal startup assembly.

### 6. Configurer CMake pour la cross-compilation

Créer `platform/stm32/toolchain-arm.cmake` :

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
```

Ajouter au `CMakeLists.txt` :

```cmake
if(CROSS_COMPILE)
    include(platform/stm32/toolchain-arm.cmake)
endif()
```

### 7. Valider le build

```bash
cmake -DCROSS_COMPILE=ON -S . -B build_stm32
cmake --build build_stm32
# Doit produire un .elf et un .bin
```

---

## ✅ Checklist de validation

- [ ] `arm-none-eabi-gcc --version` fonctionne
- [ ] STM32CubeMX installé et fonctionnel
- [ ] Linker script créé
- [ ] `cmake -DCROSS_COMPILE=ON` configure sans erreur
- [ ] Le build produit un fichier `.elf`
- [ ] Le `.elf` peut être flashé sur une vraie carte (quand disponible)

---

## 📁 Fichiers à créer/modifier

| Fichier | Action |
|---------|--------|
| `platform/stm32/toolchain-arm.cmake` | Créer |
| `platform/stm32/stm32f4xx.ld` | Créer |
| `platform/stm32/startup_stm32f4xx.s` | Créer |
| `CMakeLists.txt` | Modifier |
