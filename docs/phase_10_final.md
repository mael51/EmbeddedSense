# Phase 10 - Finalisation

**Objectif** : Nettoyer le code, documenter le projet, préparer la démo finale et créer un README complet pour montrer le résultat.

---

## 🧠 Notions clés

- **Code review** : Relire tout le code pour trouver les incohérences, les doublons, les bugs potentiels.
- **Doxygen** : Outil qui génère automatiquement la documentation HTML à partir des commentaires dans le code.
- **Démo** : Présentation live du système qui tourne sur la carte STM32.

---

## 🛠️ Étapes à réaliser

### 1. Nettoyage du code

| Action | Détail |
|--------|--------|
| Supprimer le code mort | Fonctions jamais appelées, `#include` inutiles |
| Uniformiser le style | `clang-format` avec `.clang-format` |
| Renommer les variables | Noms clairs, pas d'abbreviations cryptiques |
| Ajouter des commentaires | Chaque fonction publique documentée |
| Vérifier les headers | Partout des `#pragma once` |

### 2. Documentation Doxygen

Créer `Doxyfile` et ajouter des commentaires dans le code :

```cpp
/**
 * @brief Acquiert les données de tous les capteurs
 * 
 * Lit chaque capteur et stocke les valeurs dans les
 * buffers circulaires correspondants.
 * 
 * @pre Au moins un capteur doit être ajouté
 * @post Les buffers contiennent les nouvelles valeurs
 */
void SensorManager::acquire();
```

### 3. Optimisations finales

| Optimisation | Pourquoi |
|-------------|----------|
| `const` partout | Les méthodes qui ne modifient pas l'état |
| `[[nodiscard]]` | Empêche d'ignorer le retour de fonctions importantes |
| Static analysis | Compiler avec `-Wall -Wextra -Werror` |
| Heap check | Pas de fuite mémoire |

### 4. Créer la démo

**Scénario de démo :**

1. Allumer la carte → LED verte (DORMANT)
2. Envoyer "START" via UART → LED verte clignote (SURVEILLANCE)
3. Chauffer le capteur (souffler dessus ou allume-gaz) → LED rouge (ALERTE)
4. Envoyer "RESET" → retour LED verte (DORMANT)
5. Montrer les logs sur le terminal série

### 5. README complet

Mettre à jour `README.md` avec :

- Description du projet
- Photo de la carte
- Diagramme d'architecture
- Liste des fonctionnalités
- Comment compiler et flasher
- Comment lancer les tests

### 6. Git final

```bash
git add .
git commit -m "Phase 10 - Finalisation du projet EmbeddedSense"
git tag v1.0.0
```

---

## ✅ Checklist de validation

- [ ] Tous les fichiers ont des commentaires Doxygen
- [ ] `clang-format` appliqué sur tout le code
- [ ] `-Wall -Wextra -Werror` passe sans warning
- [ ] `cmake --build build && cd build && ctest` : tous les tests passent
- [ ] README.md complet avec instructions
- [ ] Démo fonctionne sur la carte STM32
- [ ] Git tag `v1.0.0` créé

---

## 📊 Récapitulatif du projet

| Phase | Statut | Description |
|-------|--------|-------------|
| 0 | ✅ | Environnement (CMake, GCC, GoogleTest) |
| 1 | ⬜ | Simulation des capteurs |
| 2 | ⬜ | Acquisition et traitement |
| 3 | ⬜ | Machine à états |
| 4 | ⬜ | Logs et communication |
| 5 | ⬜ | Multithreading (PC) |
| 6 | ⬜ | FreeRTOS simulé |
| 7 | ⬜ | Tests d'intégration |
| 8 | ⬜ | Préparation STM32 |
| 9 | ⬜ | Portage STM32 réel |
| 10 | ⬜ | Finalisation |
