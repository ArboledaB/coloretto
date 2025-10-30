#include "Jugador.h"
#include <algorithm>
using namespace std;

// Constructor: inicializa el jugador con su nombre
Jugador::Jugador(const string& n) : nombre(n) {}

// Agrega una carta al vector de cartas del jugador
void Jugador::agregarCarta(const Carta& c) {
    cartas.push_back(c);
}

// Devuelve un mapa con la cantidad de cartas por color
map<string, int> Jugador::obtenerConteoColores() const {
    map<string, int> conteo;
    for (const auto& c : cartas)
        conteo[c.color]++;
    return conteo;
}

// Calcula el puntaje total del jugador segun las reglas del juego
int Jugador::calcularPuntaje() {
    map<string, int> conteo;
    for (const auto& c : cartas)
        conteo[c.color]++;

    vector<int> cantidades;
    for (const auto& p : conteo)
        cantidades.push_back(p.second);

    sort(cantidades.begin(), cantidades.end(), greater<int>());

    int total = 0;
    for (int i = 0; i < (int)cantidades.size(); ++i) {
        int n = cantidades[i];
        int puntos = 0;

        if (n == 1) puntos = 1;
        else if (n == 2) puntos = 3;
        else if (n == 3) puntos = 6;
        else if (n == 4) puntos = 10;
        else if (n == 5) puntos = 15;
        else if (n >= 6) puntos = 21;

        // Solo las tres mejores combinaciones suman, el resto resta
        if (i < 3)
            total += puntos;
        else
            total -= puntos;
    }

    puntaje = total;
    return total;
}
