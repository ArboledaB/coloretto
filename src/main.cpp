#include <iostream>
#include <ctime>
#include "Juego.h"
using namespace std;

int main() {
    srand((unsigned)time(nullptr));
    char jugarOtraVez;

    do {
        cout << "===================================\n";
        cout << "     BIENVENIDO A COLORETTO     \n";
        cout << "===================================\n\n";

        int n;
        do {
            cout << "Ingrese numero de jugadores (3-5): ";
            cin >> n;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                n = 0;
            }
        } while (n < 3 || n > 5);

        // Se crea el juego con memoria dinamica para los jugadores
        Juego partida(n);
        partida.iniciarJuego();

        cout << "\nDesea jugar otra vez? (s/n): ";
        cin >> jugarOtraVez;
        jugarOtraVez = tolower(jugarOtraVez);
        cout << "\n";

    } while (jugarOtraVez == 's');

    cout << "Gracias por jugar Coloretto. Hasta la proxima!\n";
    cout << "Presione ENTER para salir...";
    cin.ignore();
    cin.get();

    return 0;
}
