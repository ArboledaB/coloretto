#pragma once
#include <string>
#include <vector>
#include <map>
#include "Carta.h"
using namespace std;

// Clase que representa a un jugador en el juego.
class Jugador {
public:
    string nombre;            // Nombre del jugador
    vector<Carta> cartas;     // Cartas que posee el jugador
    int puntaje = 0;          // Puntos acumulados

    // Constructor
    explicit Jugador(const string& n);

    // Agrega una carta a la colección del jugador
    void agregarCarta(const Carta& c);

    // Devuelve un mapa con la cantidad de cartas por color
    map<string, int> obtenerConteoColores() const;

    // Calcula y actualiza el puntaje total del jugador
    int calcularPuntaje();
};
