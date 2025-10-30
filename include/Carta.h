#pragma once
#include <string>
using namespace std;

// Representa una carta del juego Coloretto.
struct Carta {
    string color;

    // Constructor con valor por defecto (puede crear una carta vacía o con color específico)
    Carta(const string& c = "") : color(c) {}
};
