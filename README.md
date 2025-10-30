COLORETTO

DESCRIPCION DEL PROYECTO
Este proyecto implementa el juego **Coloretto** en lenguaje **C++**, aplicando **programacion orientada a objetos (POO)** y **uso de memoria dinamica**.  
El objetivo principal es modelar la logica del juego de cartas de manera estructurada, con clases y funciones que representen el flujo real del juego.

El programa permite que entre **3 y 5 jugadores** participen, gestionando:
- Rondas automaticas.
- Reparto de cartas.
- Toma de filas.
- Calculo de puntajes.
- Deteccion del final de la partida.

---

OBJETIVO
Este proyecto tiene como proposito:
- Implementar **memoria dinamica** para la creacion y manejo de objetos.
- Estructurar el codigo en archivos `.h` y `.cpp` para un diseño escalable.

---

ESTRUCTURA DEL PROYECTO

```

coloretto/
│
├── include/
│   ├── Carta.h
│   ├── Jugador.h
│   └── Juego.h
│
├── src/
│   ├── main.cpp
│   ├── Jugador.cpp
│   └── Juego.cpp
│
├── compilar.bat
└── README.md

````

---

COMO COMPILAR

1. Abre **Developer Command Prompt for VS Insiders**.
2. Cambia el directorio al proyecto:
3. Ejecuta el archivo:
   compilar.bat
4. Si la compilacion es exitosa, aparecera el mensaje:
   COMPILACION EXITOSA
5. Luego ejecuta manualmente:
   coloretto.exe

---
REGLAS BASICAS DEL JUEGO

1. Cada jugador, en su turno, puede:

   * **Robar una carta** del mazo y colocarla en una fila.
   * **Tomar una fila** de cartas y quedar inactivo en esa ronda.
2. Si aparece la carta **"Fin"**, esa sera la **ultima ronda** del juego.
3. Al final de cada ronda:

   * El jugador que tomo **ultimo una fila** sera el primero en la siguiente ronda.
4. Al finalizar el juego:

   * Se calculan los puntajes segun las combinaciones de colores.
   * Solo las **tres mejores combinaciones** suman puntos; las demas restan.

---

CALCULO DEL PUNTAJE

| Cantidad de cartas del mismo color | Puntos |
| ---------------------------------: | :----: |
|                                  1 |    1   |
|                                  2 |    3   |
|                                  3 |    6   |
|                                  4 |   10   |
|                                  5 |   15   |
|                            6 o mas |   21   |

* Las **tres combinaciones mas altas** suman al total.
* El resto **resta** del puntaje total.

---

USO DE MEMORIA DINAMICA

* Los jugadores se crean de manera **dinamica** dentro de la clase `Juego`:

  ```cpp
  jugadores.push_back(new Jugador(nombre));
  ```
* Al finalizar el juego, todos los objetos son liberados con:

  ```cpp
  delete p;
  ```

Esto garantiza una gestion correcta de la memoria.

---

REQUISITOS

* Compilador compatible con **C++17 o superior**.
* Entorno de compilacion: **Visual Studio Developer Command Prompt**.
