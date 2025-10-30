#pragma once
#include <vector>
#include <string>
#include "Jugador.h"
#include "Carta.h"
using namespace std;

// Clase que maneja toda la logica del juego Coloretto
class Juego {
private:
    vector<Jugador*> jugadores;            // Jugadores (creados dinamicamente con new)
    vector<vector<Carta>> tablero;         // Matriz de cartas en el tablero
    vector<Carta> mazo;                    // Mazo de cartas
    vector<bool> activoRonda;              // Si un jugador esta activo en la ronda
    vector<bool> filaDisponible;           // Filas disponibles para tomar
    int turnoActual;                       // Indice del jugador actual
    int filas;                             // Numero de filas
    int columnas;                          // Numero de columnas
    int cartasRobadas;                     // Contador de cartas robadas
    bool ultimaRonda;                      // Si ya salio la carta de fin
    bool mensajeFinVisible;                // Si se muestra el mensaje de ultima ronda

    // --- Nuevos campos para controlar el orden de inicio de la siguiente ronda ---
    vector<int> ordenTomaron;   // En la ronda actual: indices de jugadores en el orden en que tomaron fila (push_back cuando toman)
    vector<int> ordenTurnos;    // Orden de turnos que se usara para la ronda actual (generalmente reverse(ordenTomaron) de la ronda anterior)
    int posTurnoIndice = 0;     // Posicion actual dentro de ordenTurnos

public:
    // Constructor y destructor
    explicit Juego(int numJugadores);
    ~Juego();

    // Inicia todo el juego (multiples rondas)
    void iniciarJuego();

    // Inicia una ronda especifica
    void iniciarRonda(int numeroRonda);

private:
    // --- Metodos auxiliares ---
    void inicializarMazo();
    void barajarEInsertarFin();
    void limpiarPantalla();
    bool todosInactivos() const;
    bool filasLlenas() const;
    void mostrarPuntajes() const;
    void mostrarCartasJugadores() const;
    void mostrarTablero() const;
    void mostrarResumenFinal();
};
