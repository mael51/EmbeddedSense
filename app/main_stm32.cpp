#include <cstdint>

// Fonction principale pour STM32
int main() {
    // Simulation simple : boucle infinie
    volatile uint32_t counter = 0;
    while (true) {
        ++counter;
    }
    return 0;
}