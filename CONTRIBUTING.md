# Contribuer au projet EmbeddedSense

Merci de t'intéresser au projet. Voici comment contribuer.

## Prérequis

- Windows avec MSYS2 (MinGW GCC 13+) ou Linux/macOS avec GCC 13+
- CMake >= 3.14
- Git

## Cloner et compiler

```bash
git clone https://github.com/<user>/EmbeddedMonitoringSystem.git
cd EmbeddedMonitoringSystem
cmake -G "MSYS Makefiles" -S . -B build
cmake --build build
```

## Lancer les tests

```bash
cd build && ./emcs_tests.exe
```

Les 72 tests doivent passer. Si un test échoue, ne soumets pas de PR.

## Cross-compilation STM32

```bash
cmake -G "MSYS Makefiles" \
  -DCMAKE_TOOLCHAIN_FILE="platform/stm32/toolchain-arm.cmake" \
  -DCROSS_COMPILE=ON -S . -B build_stm32
cmake --build build_stm32
```

Note : la cross-compilation doit se faire depuis MSYS2 bash, pas PowerShell.

## Style de code

Le projet utilise clang-format (style Google, indent 4). Avant de committer :

```bash
clang-format -i app/**/*.cpp drivers/**/*.cpp platform/**/*.cpp services/**/*.cpp
```

## Convention de commits

Format : `type(scope): description`

Exemples :
- `feat(sensors): add STM32 pressure sensor driver`
- `fix(acquisition): handle empty buffer in SensorManager`
- `docs(phase8): add cross-compilation details`
- `test(robustness): add NaN handling test`

Types : feat, fix, docs, test, refactor, chore, build, ci

## Soumettre une Pull Request

1. Fork le repo
2. Crée une branche : `git checkout -b feature/ma-feature`
3. Commit avec des messages clairs
4. Push : `git push origin feature/ma-feature`
5. Ouvre une PR sur GitHub
6. Remplis le template de PR

La CI doit passer (build + tests) avant review.