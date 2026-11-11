# Phase 7 - Tests d'intégration

**Objectif** : Valider que toutes les couches (capteurs, acquisition, états, logs, communication, multithreading) fonctionnent ensemble sans bugs.

---

## 🧠 Notions clés

- **Test unitaire** : Teste une seule fonction/classe isolément (Phases 1-6).
- **Test d'intégration** : Teste l'interaction entre plusieurs composants ensemble.
- **Test bout-en-bout (E2E)** : Simule le scénario complet de bout en bout.
- **Fixture** : Une configuration de test réutilisable (setup + teardown).

---

## Scénarios d'intégration

### Scénario 1 : Acquisition → État

1. Créer MockSensors + StateMachine
2. Simuler 50 acquisitions avec valeurs normales
3. Vérifier : FSM reste en SURVEILLANCE
4. Simuler une température > seuil
5. Vérifier : FSM passe en ALERTE

### Scénario 2 : Erreur → Récupération

1. Démarrer le système
2. Simuler une erreur capteur
3. Vérifier : FSM en ERREUR, Logger contient "ERROR"
4. Envoyer RESET
5. Vérifier : FSM retourne en DORMANT

### Scénario 3 : Communication complète

1. Créer Logger + MockUART + CommunicationManager
2. Acquérir des données
3. Envoyer via CommunicationManager
4. Vérifier : MockUART contient les données formatées

### Scénario 4 : Multithreading complet

1. Lancer les 3 threads (Acquisition, Processing, Communication)
2. Attendre 2 secondes
3. Vérifier : aucune exception, aucun deadlock
4. Arrêter proprement
5. Vérifier : tous les threads sont stoppés

---

## 🛠️ Étapes à réaliser

### 1. Créer un test par scénario

Créer `tests/test_integration.cpp` avec des fixtures GoogleTest :

```cpp
class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override { /* Créer tous les composants */ }
    void TearDown() override { /* Nettoyer */ }
};
```

### 2. Tester le flux complet avec des commandes UART

| Commande UART | Résultat attendu |
|---------------|-----------------|
| `"STATUS"` | Répond avec les valeurs capteurs |
| `"RESET"` | FSM retourne en DORMANT |
| `"LOG_LEVEL DEBUG"` | Logger accepte DEBUG |
| `"UNKNOWN"` | Répond "ERR:UNKNOWN_CMD" |

### 3. Tests de robustesse

| Test | Objectif |
|------|---------|
| `StressTest` | 10000 itérations sans crash |
| `ConcurrentAccess` | Accès simultané SharedData |
| `InvalidSensors` | Capteur qui retourne NaN |

### 4. Mettre à jour CMakeLists.txt

---

## ✅ Checklist de validation

- [ ] 4 scénarios d'intégration passent
- [ ] 3 tests de robustesse passent
- [ ] Le flux complet Acquisition → Traitement → Communication fonctionne
- [ ] Les erreurs sont gérées proprement (pas de crash)
- [ ] `cmake --build build && cd build && ctest` : tous les tests passent

---

## 📁 Fichiers à créer/modifier

| Fichier | Action |
|---------|--------|
| `tests/test_integration.cpp` | Créer |
| `tests/test_robustness.cpp` | Créer |
| `CMakeLists.txt` | Modifier |
