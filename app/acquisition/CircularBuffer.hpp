#pragma once
#include <cstddef>
#include <stdexcept>

template<typename T, size_t Capacity>
class CircularBuffer {
private:
    T buffer[Capacity];
    size_t head = 0;   // Position d'écriture (prochaine case à écraser)
    size_t count = 0;  // Nombre d'éléments actuellement stockés

public:
    // Ajoute une valeur. Si le buffer est plein, écrase la plus ancienne.
    void push(T value) {
        buffer[head] = value;
        head = (head + 1) % Capacity;
        if (count < Capacity) {
            ++count;
        }
    }

    // Retourne la valeur à l'index donné (0 = plus ancienne)
    T get(size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Index hors limites du CircularBuffer");
        }
        // L'élément le plus ancien est à (head - count + Capacity) % Capacity
        size_t oldest = (head + Capacity - count) % Capacity;
        return buffer[(oldest + index) % Capacity];
    }

    // Nombre d'éléments stockés
    size_t size() const {
        return count;
    }

    // Capacité maximale
    static constexpr size_t capacity() {
        return Capacity;
    }

    // Moyenne de tous les éléments
    T average() const {
        if (count == 0) {
            return T{};
        }
        T sum = T{};
        for (size_t i = 0; i < count; ++i) {
            sum += get(i);
        }
        return sum / static_cast<T>(count);
    }

    // Valeur minimale
    T min() const {
        if (count == 0) {
            return T{};
        }
        T m = get(0);
        for (size_t i = 1; i < count; ++i) {
            if (get(i) < m) {
                m = get(i);
            }
        }
        return m;
    }

    // Valeur maximale
    T max() const {
        if (count == 0) {
            return T{};
        }
        T m = get(0);
        for (size_t i = 1; i < count; ++i) {
            if (get(i) > m) {
                m = get(i);
            }
        }
        return m;
    }

    // Vide le buffer
    void clear() {
        head = 0;
        count = 0;
    }
};