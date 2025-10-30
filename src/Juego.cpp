#include "Juego.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

// -------------------------------------------------------------
// Constructor: Crea los jugadores y prepara todo el entorno
// -------------------------------------------------------------
Juego::Juego(int numJugadores) {
    filas = numJugadores;
    columnas = 3;
    turnoActual = 0;
    cartasRobadas = 0;
    ultimaRonda = false;
    mensajeFinVisible = false;

    posTurnoIndice = 0;
    ordenTomaron.clear();
    ordenTurnos.clear();

    // Pedir nombres de los jugadores
    for (int i = 1; i <= numJugadores; ++i) {
        string nombre;
        cout << "Ingrese el nombre del jugador " << i << ": ";
        cin >> ws;
        getline(cin, nombre);
        jugadores.push_back(new Jugador(nombre)); // Uso de memoria dinamica
    }

    inicializarMazo();
    barajarEInsertarFin();
    tablero.resize(filas, vector<Carta>(columnas, Carta(" ")));
    activoRonda.resize(filas, true);
    filaDisponible.resize(filas, true);
}

// -------------------------------------------------------------
// Destructor: Libera la memoria dinamica de los jugadores
// -------------------------------------------------------------
Juego::~Juego() {
    for (auto p : jugadores)
        delete p;
}

// -------------------------------------------------------------
// Inicializa el mazo con los colores disponibles
// -------------------------------------------------------------
void Juego::inicializarMazo() {
    vector<string> colores = {"Rojo", "Verde", "Azul", "Amarillo", "Rosa", "Naranja", "Marron"};
    for (auto& col : colores)
        for (int k = 0; k < 7; ++k)
            mazo.push_back(Carta(col));
}

// -------------------------------------------------------------
// Baraja el mazo y agrega la carta "Fin" en una posicion aleatoria
// -------------------------------------------------------------
void Juego::barajarEInsertarFin() {
    mt19937 rng((unsigned)time(nullptr));
    shuffle(mazo.begin(), mazo.end(), rng);

    int posicionDeseada = 50 - 34;
    if (posicionDeseada < 0) posicionDeseada = 0;
    if (posicionDeseada > (int)mazo.size()) posicionDeseada = (int)mazo.size();

    mazo.insert(mazo.begin() + posicionDeseada, Carta("Fin"));
}

// -------------------------------------------------------------
// Limpia la pantalla segun el sistema operativo
// -------------------------------------------------------------
void Juego::limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// -------------------------------------------------------------
// Verifica si todos los jugadores estan inactivos en la ronda
// -------------------------------------------------------------
bool Juego::todosInactivos() const {
    for (bool b : activoRonda)
        if (b) return false;
    return true;
}

// -------------------------------------------------------------
// Verifica si todas las filas estan llenas
// -------------------------------------------------------------
bool Juego::filasLlenas() const {
    for (int i = 0; i < filas; ++i)
        if (filaDisponible[i])
            for (int j = 0; j < columnas; ++j)
                if (tablero[i][j].color == " ")
                    return false;
    return true;
}

// -------------------------------------------------------------
// Muestra los puntajes actuales
// -------------------------------------------------------------
void Juego::mostrarPuntajes() const {
    cout << "\n===== PUNTAJES =====\n";
    for (int i = 0; i < (int)jugadores.size(); ++i) {
        cout << jugadores[i]->nombre << ": " << jugadores[i]->puntaje;
        if (!activoRonda[i]) cout << " (ya tomo fila)";
        cout << "\n";
    }
    cout << "====================\n";
}

// -------------------------------------------------------------
// Muestra las cartas que tiene cada jugador
// -------------------------------------------------------------
void Juego::mostrarCartasJugadores() const {
    cout << "\n===== CARTAS DE LOS JUGADORES =====\n";
    for (auto j : jugadores) {
        cout << j->nombre << " -> ";
        auto conteo = j->obtenerConteoColores();
        if (conteo.empty()) cout << "(sin cartas)";
        else {
            bool primero = true;
            for (auto& p : conteo) {
                if (!primero) cout << ", ";
                cout << p.first << ": " << p.second;
                primero = false;
            }
        }
        cout << "\n";
    }
    cout << "===================================\n";
}

// -------------------------------------------------------------
// Muestra el tablero actual
// -------------------------------------------------------------
void Juego::mostrarTablero() const {
    cout << "\n====== TABLERO ======\n";
    for (int i = 0; i < filas; ++i) {
        cout << "Fila " << i + 1 << ": ";
        for (int j = 0; j < columnas; ++j)
            cout << "[" << tablero[i][j].color << "] ";
        if (!filaDisponible[i]) cout << " (INHABILITADA)";
        cout << "\n";
    }
    cout << "=====================\n";
}

// -------------------------------------------------------------
// Muestra los resultados finales y determina el ganador
// -------------------------------------------------------------
void Juego::mostrarResumenFinal() {
    cout << "\n===== RESULTADOS FINALES =====\n";
    for (auto j : jugadores) {
        j->calcularPuntaje();
        cout << j->nombre << " -> " << j->puntaje << " puntos\n";
    }

    Jugador* ganador = jugadores[0];
    for (auto p : jugadores)
        if (p->puntaje > ganador->puntaje)
            ganador = p;

    cout << "\nGANADOR: " << ganador->nombre
         << " con " << ganador->puntaje << " puntos\n";
    cout << "==============================\n";
}

// -------------------------------------------------------------
// Inicia una ronda del juego
// -------------------------------------------------------------
void Juego::iniciarRonda(int numeroRonda) {
    // Reiniciar tablero y banderas
    for (int i = 0; i < filas; ++i)
        for (int j = 0; j < columnas; ++j)
            tablero[i][j] = Carta(" ");
    fill(activoRonda.begin(), activoRonda.end(), true);
    fill(filaDisponible.begin(), filaDisponible.end(), true);

    // Preparar ordenTurnos: si hay orden previo (ordenTomaron almacenado), usar su inverso
    // Nota: ordenTomaron contiene el orden en que los jugadores tomaron la fila en la ronda anterior
    if ((int)ordenTomaron.size() == filas) {
        ordenTurnos = ordenTomaron;               // copia
        reverse(ordenTurnos.begin(), ordenTurnos.end()); // invertir: ultimo que tomo -> primero en la lista
    } else {
        // Si no hay informacion previa (primera ronda), usar orden natural 0..n-1
        ordenTurnos.clear();
        for (int i = 0; i < filas; ++i) ordenTurnos.push_back(i);
    }

    posTurnoIndice = 0;
    // Vaciar el registro para esta ronda (se llenara en el momento que cada jugador tome una fila)
    ordenTomaron.clear();

    bool finRonda = false;

    while (!finRonda && !mazo.empty() && !todosInactivos()) {
        limpiarPantalla();

        if (mensajeFinVisible)
            cout << "\n*** Se revelo la carta de FIN DE RONDA: esta sera la ultima ronda del juego ***\n";

        cout << "\nCartas robadas: " << cartasRobadas
             << " / " << (int)mazo.size() + cartasRobadas << "\n";
        mostrarPuntajes();
        mostrarCartasJugadores();
        mostrarTablero();

        // Encontrar el siguiente jugador activo segun ordenTurnos
        if (ordenTurnos.empty()) break; // defensa (no deberia pasar)
        int intentos = 0;
        while (intentos < (int)ordenTurnos.size() && !activoRonda[ ordenTurnos[posTurnoIndice] ]) {
            posTurnoIndice = (posTurnoIndice + 1) % ordenTurnos.size();
            ++intentos;
        }
        if (intentos == (int)ordenTurnos.size()) break; // todos inactivos

        turnoActual = ordenTurnos[posTurnoIndice];
        Jugador* jugador = jugadores[turnoActual];
        cout << "\nTurno de " << jugador->nombre << endl;

        bool obligadoTomar = filasLlenas();
        if (obligadoTomar)
            cout << "Todas las filas estan llenas. Debes tomar una fila.\n";

        int opcion = 0;
        do {
            if (!obligadoTomar) {
                cout << "1) Robar carta\n2) Tomar una fila\nElige: ";
                cin >> opcion;
            } else {
                opcion = 2;
            }

            if (cin.fail() || (opcion != 1 && opcion != 2)) {
                cout << "Entrada invalida.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                opcion = 0;
            }
        } while (opcion != 1 && opcion != 2);

        if (opcion == 1) {
            if (mazo.empty()) break;
            Carta c = mazo.back();
            mazo.pop_back();
            cartasRobadas++;

            if (c.color == "Fin") {
                cout << "\n*** Carta FIN: ultima ronda del juego ***\n";
                ultimaRonda = true;
                mensajeFinVisible = true;
            } else {
                cout << "Has robado: [" << c.color << "]\n";
                int fila = 0;
                bool colocado = false;
                do {
                    cout << "En que fila colocar (1-" << filas << ")? ";
                    cin >> fila;
                    if (cin.fail() || fila < 1 || fila > filas) {
                        cout << "Fila invalida.\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                        continue;
                    }

                    int idx = fila - 1;
                    if (!filaDisponible[idx]) {
                        cout << "Esa fila esta inhabilitada.\n";
                        continue;
                    }

                    for (int k = 0; k < columnas; ++k) {
                        if (tablero[idx][k].color == " ") {
                            tablero[idx][k] = c;
                            colocado = true;
                            break;
                        }
                    }

                    if (!colocado) cout << "La fila esta llena.\n";
                } while (!colocado);
            }
        } else if (opcion == 2) {
            int fila = 0;
            bool valido = false;
            bool todasVacias = true;

            for (int f = 0; f < filas; ++f)
                for (int c = 0; c < columnas; ++c)
                    if (tablero[f][c].color != " " && filaDisponible[f])
                        todasVacias = false;

            if (todasVacias) {
                cout << "No hay filas con cartas. Debes robar una carta.\n";
                continue;
            }

            do {
                cout << "Que fila deseas tomar (1-" << filas << ")? ";
                cin >> fila;
                if (cin.fail() || fila < 1 || fila > filas) {
                    cout << "Fila invalida.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }

                int idx = fila - 1;
                if (!filaDisponible[idx]) {
                    cout << "Esa fila ya fue tomada.\n";
                    continue;
                }

                bool vacia = true;
                vector<Carta> tomadas;
                for (int k = 0; k < columnas; ++k) {
                    if (tablero[idx][k].color != " ") {
                        vacia = false;
                        tomadas.push_back(tablero[idx][k]);
                        tablero[idx][k] = Carta(" ");
                    }
                }

                if (vacia) {
                    cout << "Esa fila esta vacia.\n";
                } else {
                    for (auto& tc : tomadas)
                        jugador->agregarCarta(tc);

                    jugador->calcularPuntaje();
                    activoRonda[turnoActual] = false;
                    filaDisponible[idx] = false;
                    cout << jugador->nombre << " tomo la fila.\n";

                    // Registrar que este jugador tomo en este orden
                    ordenTomaron.push_back(turnoActual);

                    valido = true;
                }
            } while (!valido);
        }

        // Avanzar al siguiente posicion en ordenTurnos para el proximo turno
        posTurnoIndice = (posTurnoIndice + 1) % ordenTurnos.size();

        // Si ahora todos estan inactivos, el while principal terminará
    }

    limpiarPantalla();
    cout << "\n*** Fin de la ronda " << numeroRonda << " ***\n";
    mostrarPuntajes();
    mostrarCartasJugadores();

    // Al final de la ronda: si ordenTomaron contiene la secuencia de toma,
    // la dejamos para la siguiente ronda (se usara invertida al iniciar).
    // Si ordenTomaron no tiene tamaño 'filas' por alguna razon, la ronda siguiente usara orden natural.
    if ((int)ordenTomaron.size() != filas) {
        // No se completo el registro correctamente, limpiar para forzar orden natural
        ordenTomaron.clear();
    }

    if (ultimaRonda) {
        mostrarResumenFinal();
    } else {
        cout << "\nPresiona ENTER para iniciar la siguiente ronda...";
        cin.ignore();
        cin.get();
    }
}

// -------------------------------------------------------------
// Inicia el juego completo (controla las rondas)
// -------------------------------------------------------------
void Juego::iniciarJuego() {
    int nro = 1;
    while (!mazo.empty()) {
        iniciarRonda(nro++);
        if (ultimaRonda) return;
    }

    limpiarPantalla();
    cout << "\nMazo vacio. Fin del juego.\n";
    mostrarResumenFinal();
}
